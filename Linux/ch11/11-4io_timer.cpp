#include <stdio.h>
#include <time.h>
#include <sys/epoll.h>

#define TIMEOUT 5000

int main()
{
    int timeout = TIMEOUT;
    time_t start = time(NULL);
    time_t end = time(NULL);
    while (1)
    {
        printf("the timeout is now %d mill-seconds\n", timeout);
        start = time(NULL);
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, timeout);
        if ((number < 0) && (errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }
        // 返回值为 0 说明超时时间已到，可以处理定时任务，并处置定时事件
        if (number == 0)
        {
            timeout = TIMEOUT;
            continue;
        }

        end = time(NULL);
        // 返回值大于 0 说明本次 epoll_wait 调用持续时间为 (end-start)*1000
        // 此时需要将 timeout 减去这段时间，以获得下次 epoll_wait 调用的超时参数
        timeout -= (end - start) * 1000;

        // 重新计算之后 timeout <= 0 说明本次 epoll_wait 调用返回时，不仅有文件描述符就绪
        // 而且其超时时间也刚好到达，此时我们也要处理定时任务，并重置定时时间
        if (timeout <= 0)
        {
            timeout = TIMEOUT;
        }

        // handle connections
    }
}
