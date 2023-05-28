#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
#include "lst_timer.h"

#define MAX_FD 65536           // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000 // 监听的最大的事件数量
#define TIME_SLOT 5            // 定时器触发时间

static int pipefd[2];
static sort_timer_lst timer_lst;
static int epollfd;

// 添加文件描述符
extern void addfd(int epollfd, int fd, bool one_shot);
// extern void removefd(int epollfd, int fd);
// extern void modfd(int epollfd, int fd, int ev);
extern void setnonblocking(int fd);

// 添加信号捕捉
void addsig(int sig, void(handler)(int))
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void sig_handler(int sig)
{
    int save_errno = errno;
    int msg = sig;
    send(pipe[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

void timer_handler()
{
    // 定时处理任务，实际就是调用 tick() 函数
    timer_lst.tick();
    // 因为一次 alarm 调用只会引起一次 SIGALRM 信号，所以我们要重新定时，以不断触发 SIGALRM 信号
    alarm(TIME_SLOT);
}

// 定时器回调函数，它删除非活动连接socket上的注册事件，并关闭之
void call_back(client_data* user_data)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    // assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count --; 
    printf("close fd %d, now %d users left\n", user_data->sockfd, http_conn::m_user_count);
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("usage: %s port_number\n", basename(argv[0]));
        return 1;
    }
    int port = atoi(argv[1]);

    threadpool<http_conn> *pool = NULL;
    try {
        pool = new threadpool<http_conn>;
    } catch (...) {
        return 1;
    }

    http_conn *users = new http_conn[MAX_FD];

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // 端口复用
    int reuse = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }
    ret = listen(listenfd, 5);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 创建epoll对象，和事件数组，添加
    epoll_event events[MAX_EVENT_NUMBER];
    epollfd = epoll_create(5);
    // 添加到epoll对象中，监听套接字不能设置 epolloneshot 事件
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    // 创建管道
    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret != -1);
    setnonblocking(pipefd[1]);
    addfd(epollfd, pipefd[0], false); // todo: 要不要 epolloneshot

    // 对 SIGPIE 信号处理，忽略它
    addsig(SIGPIPE, SIG_IGN);
    // 对 SIGALRM、SIGTERM 设置信号处理函数
    addsig(SIGALRM, sig_handler);
    addsig(SIGTERM, sig_handler);

    bool timeout = false;
    bool stop_server = false;
    client_data* client_users = new client_data[MAX_FD]; 
    alarm(TIME_SLOT);

    while (!stop_server)
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);

        if ((number < 0) && (errno != EINTR)) {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);

                if (connfd < 0) {
                    printf("errno is: %d\n", errno);
                    continue;
                }

                if (http_conn::m_user_count >= MAX_FD) {
                    close(connfd);
                    continue;
                }
                // 将新的客户数据初始化，放到数组中
                users[connfd].init(connfd, client_address);

                // 创建定时器，设置其回调函数与超时时间，然后绑定定时器与用户数据，最后将定时器添加到链表timer_lst中
                util_timer* timer = new util_timer;
                timer->cb_func = cb_func;
                time_t cur = time(NULL);
                timer->expire = cur + 3 * TIMESLOT;
                
                client_users[connfd].timer = timer;
                client_users[connfd].sockfd = connfd;
                client_users[connfd].address = client_address;
                timer->user_data = &client_users[connfd];
                
                timer_lst.add_timer(timer);
            }
            else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN))
            {
                // 处理信号
                int sig;
                char signals[1024];
                int ret = recv(pipefd[0], signals, sizeof(signals), 0);
                if (ret == -1) {
                    continue;
                } else if (ret == 0) {
                    continue;
                } else {
                    for (int i = 0; i < ret; ++i) {
                        switch (signals[i])
                        {
                        case SIGALRM:
                        {
                            // 用timeout变量标记有定时任务需要处理，但不立即处理定时任务
                            // 这是因为定时任务的优先级不是很高，我们优先处理其他更重要的任务。
                            timeout = true;
                            break;
                        }
                        case SIGTERM:
                        {
                            stop_server = true;
                        }
                        }
                    }
                }
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                users[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN) {
                util_timer *timer = client_users[sockfd].timer;
                time_t cur = time(NULL);
                timer->expire = cur + 3 * TIME_SLOT;
                timer_lst.adjust_timer(timer);

                // 一次性读取数据失败就要关闭连接并删除定时器
                if (users[sockfd].read()) {
                    pool->append(users + sockfd);
                } else {
                    timer_lst.del_timer(timer);
                    users[sockfd].close_conn();
                }
            } else if (events[i].events & EPOLLOUT) {
                if (!users[sockfd].write()) {
                    users[sockfd].close_conn();
                }
            }
        }
        // 最后处理定时事件，因为I/O事件有更高的优先级。
        // 当然，这样做将导致定时任务不能精准的按照预定的时间执行。
        if (timeout) {
            timer_handler();
            timeout = false;
        }
    }

    close(epollfd);
    close(listenfd);
    delete[] users;
    delete pool;
    return 0;
}