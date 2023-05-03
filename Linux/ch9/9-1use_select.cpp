#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <libgen.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    printf("ip is %s and port is %d\n", ip, port);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    if (connfd < 0)
    {
        printf("errno is: %d\n", errno);
        close(listenfd);
    }

    char remote_addr[INET_ADDRSTRLEN];
    printf("connected with ip: %s and port: %d\n",
           inet_ntop(AF_INET, &client_address.sin_addr, remote_addr, INET_ADDRSTRLEN),
           ntohs(client_address.sin_port));

    char buf[1024];
    fd_set read_fds;
    fd_set exception_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);

    // 设置 SO_OOBINLINE 之后带外数据会被当成 normal data 一样读取
    // int nReuseAddr = 1;
    // setsockopt(connfd, SOL_SOCKET, SO_OOBINLINE, &nReuseAddr, sizeof(nReuseAddr));
    while (1)
    {
        memset(buf, '\0', sizeof(buf));
        // 每次调用 select 前都要重新在 read_fds 和 exception_fds 中设置文件描述符 connfd
        // 因为事件发生之后文件描述符集合将被内核修改
        FD_SET(connfd, &read_fds);
        FD_SET(connfd, &exception_fds);

        ret = select(connfd + 1, &read_fds, NULL, &exception_fds, NULL);
        printf("select one\n");
        if (ret < 0)
        {
            printf("selection failure\n");
            break;
        }

        // 可读事件采用普通的 recv 函数读取数据
        if (FD_ISSET(connfd, &read_fds))
        {
            ret = recv(connfd, buf, sizeof(buf) - 1, 0);
            if (ret <= 0)
            {
                printf("normal data recv < 0\n");
                break;
            }
            printf("get %d bytes of normal data: %s\n", ret, buf);
        }
        // 异常事件，采用 MSG_OOB 标志的 recv 函数读取带外数据
        else if (FD_ISSET(connfd, &exception_fds))
        {
            ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
            if (ret <= 0)
            {
                printf("oob data recv < 0\n");
                break;
            }
            printf("get %d bytes of oob data: %s\n", ret, buf);
        }
    }

    close(connfd);
    close(listenfd);
    return 0;
}
