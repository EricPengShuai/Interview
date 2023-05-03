#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <libgen.h>

#define BUFFER_SIZE 1024
// 定义两种 HTTP 状态码和状态信息
static const char *status_line[2] = {"200 OK", "500 Internal server error"};

int main(int argc, char *argv[])
{
    if (argc <= 3)
    {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    // 将目标文件作为程序的第三个参数传入
    const char *file_name = argv[3];

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength);
    if (connfd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        // 用于保存 HTTP 应答的状态行、头部字段和一个空行缓冲区
        char header_buf[BUFFER_SIZE];
        memset(header_buf, '\0', BUFFER_SIZE);
        char *file_buf;         // 存放目标文件内容的应用程序缓存
        struct stat file_stat;  // 用于获取目标文件的属性，比如是否为目录、文件大小等
        bool valid = true;      // 记录目标文件是否是有效文件
        int len = 0;            // 缓存区 header_buf 目前已经使用了多少字节的空间

        if (stat(file_name, &file_stat) < 0)
        {
            // 目标文件不存在
            valid = false;
        }
        else
        {
            if (S_ISDIR(file_stat.st_mode))
            {
                // 目标文件是一个目录
                valid = false;
            }
            else if (file_stat.st_mode & S_IROTH)
            {
                // 当前用户有读取目标文件的权限
                // 动态分配缓存区 file_buf，并指定其大小为目标文件的大小 + 1
                // 然后将目标文件读入缓存区 file_buf
                int fd = open(file_name, O_RDONLY);
                file_buf = new char[file_stat.st_size + 1];
                memset(file_buf, '\0', file_stat.st_size + 1);
                if (read(fd, file_buf, file_stat.st_size) < 0)
                {
                    valid = false;
                }
            }
            else
            {
                valid = false;
            }
        }

        // 目标文件有效就发送正常的 HTTP 应答
        if (valid)
        {
            // HTTP 应答的状态行
            ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[0]);
            len += ret;
            // “Content-Length” 头部字段
            ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len,
                           "Content-Length: %d\r\n", file_stat.st_size);
            len += ret;
            // 空行
            ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "%s", "\r\n");

            // 利用 writev 将 head_buf 和 file_buf 内容一并写出
            struct iovec iv[2];
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;
            ret = writev(connfd, iv, 2);
        }
        else
        {
            // 目标文件无效则通知客户端服务器发生了“内部错误”
            ret = snprintf(header_buf, BUFFER_SIZE - 1, "%s %s\r\n", "HTTP/1.1", status_line[1]);
            len += ret;
            ret = snprintf(header_buf + len, BUFFER_SIZE - 1 - len, "%s", "\r\n");
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close(connfd);
        delete[] file_buf;
    }

    close(sock);
    return 0;
}
