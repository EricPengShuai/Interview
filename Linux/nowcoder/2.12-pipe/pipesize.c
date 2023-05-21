#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BUF_SIZE 65544

int main()
{
    int pipeFd[2];
    if (pipe(pipeFd) == -1)
    {
        perror("pipe");
        exit(-1);
    }

    // 设置管道为非阻塞
    for (int i = 0; i < 2; ++i)
    {
        int flags = fcntl(pipeFd[i], F_GETFL);
        if (fcntl(pipeFd[i], F_SETFL, flags | O_NONBLOCK) == -1)
            perror("fcntl");
    }

    int pid = fork();
    if (pid > 0)
    {
        sleep(1); // 确保子进程先写。管道才有数据能读
        // 父进程读
        close(pipeFd[1]);
        char buf[BUF_SIZE + 1] = {0};
        printf("父进程读到了：%ld字节\n", read(pipeFd[0], buf, BUF_SIZE));
    }
    else if (pid == 0)
    {
        // 子进程
        close(pipeFd[0]);
        // in.txt有65544字节的数据
        int fd = open("in.txt", O_RDONLY);
        char buf[BUF_SIZE + 1] = {0};
        printf("从文件中读到: %ld字节\n", read(fd, buf, BUF_SIZE));
        printf("子进程写入了：%ld字节\n", write(pipeFd[1], buf, BUF_SIZE));
    }
    else
    {
        perror("fork");
        exit(-1);
    }
    return 0;
}