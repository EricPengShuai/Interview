#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>

#define BUFFER_SIZE 4096

// 主状态机的三种可能状态：分析请求行、头部字段、body 字段
enum CHECK_STATE
{
    CHECK_STATE_REQUESTLINE = 0,
    CHECK_STATE_HEADER,
    CHECK_STATE_CONTENT
};

// 从状态机的三种可能状态：读取到一个完整的行、行出错、行数据不完整
enum LINE_STATUS
{
    LINE_OK = 0,
    LINE_BAD,
    LINE_OPEN
};

// 服务器处理 HTTP 请求的结果：
// 1. NO_REQUEST 请求不完整，需要继续读取客户数据 1xx
// 2. GET_REQUEST 获得了一个完整的客户请求 2xx
// 3. BAD_REQUEST 客户请求语法错误 4xx
// 4. FORBIDDEN_REQUEST 客户对资源没有足够的访问权限 4xx
// 5. INTERNAL_ERROR 服务器内部错误 5xx
// 6. CLOSED_CONNECTION 客户端已经关闭连接
enum HTTP_CODE
{
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
    FORBIDDEN_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
};

// 会送给客户端的信息
static const char *szret[] = {"I get a correct result\n", "Something wrong\n"};

// 从状态机：解析一行的内容
// checked_index 是当前正在分析字节的下标，read_index 是 buffer 尾部的下一个字节
// 逐个检查 buffer[checked_index...read_index-1] 字节
LINE_STATUS parse_line(char *buffer, int &checked_index, int &read_index)
{
    char temp;
    for (; checked_index < read_index; ++checked_index)
    {
        temp = buffer[checked_index];
        if (temp == '\r')
        {
            if (checked_index + 1 == read_index)
            {
                return LINE_OPEN;
            }
            else if (buffer[checked_index + 1] == '\n')
            {
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if (temp == '\n')
        {
            if (checked_index > 1 && buffer[checked_index - 1] == '\r')
            {
                buffer[checked_index - 1] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    // 所有内容分析完毕也没有遇到回车，表示还需要继续读取客户数据
    return LINE_OPEN;
}

// 分析请求行
HTTP_CODE parse_requestline(char *szTemp, CHECK_STATE &checkstate)
{
    // 寻找分割符
    char *szURL = strpbrk(szTemp, " \t");
    if (!szURL) // 请求行中不含有空白字符或"\t"字符
    {
        return BAD_REQUEST;
    }
    *szURL++ = '\0';

    char *szMethod = szTemp;
    if (strcasecmp(szMethod, "GET") == 0)
    {
        printf("The request method is GET\n");
    }
    else
    {
        return BAD_REQUEST;
    }

    szURL += strspn(szURL, " \t"); // 跳过 " \t" 字符
    char *szVersion = strpbrk(szURL, " \t");
    if (!szVersion)
    {
        return BAD_REQUEST;
    }
    *szVersion++ = '\0';
    szVersion += strspn(szVersion, " \t");
    if (strcasecmp(szVersion, "HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }

    if (strncasecmp(szURL, "http://", 7) == 0)
    {
        szURL += 7;
        szURL = strchr(szURL, '/');
    }

    if (!szURL || szURL[0] != '/')
    {
        return BAD_REQUEST;
    }

    // URLDecode( szURL );
    printf("The request URL is: %s\n", szURL);
    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

// 分析头部字段
HTTP_CODE parse_headers(char *szTemp)
{
    if (szTemp[0] == '\0')
    {
        return GET_REQUEST;
    }
    else if (strncasecmp(szTemp, "Host:", 5) == 0) // 只处理 Host 字段
    {
        szTemp += 5;
        szTemp += strspn(szTemp, " \t");
        printf("the request host is: %s\n", szTemp);
    }
    else
    {
        printf("I can not handle this header\n");
    }

    return NO_REQUEST;
}

// 分析 HTTP 请求的入口函数
HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate, int &read_index, int &start_line)
{
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode = NO_REQUEST;

    // 主状态机，从 buffer 中取出所有完整的行
    while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {
        char *szTemp = buffer + start_line;
        start_line = checked_index;
        switch (checkstate)
        {
        case CHECK_STATE_REQUESTLINE:
        {
            retcode = parse_requestline(szTemp, checkstate);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        }
        case CHECK_STATE_HEADER:
        {
            retcode = parse_headers(szTemp);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if (retcode == GET_REQUEST)
            {
                return GET_REQUEST;
            }
            break;
        }
        default:
        {
            return INTERNAL_ERROR;
        }
        }
    }
    if (linestatus == LINE_OPEN)
    {
        return NO_REQUEST;
    }
    else
    {
        return BAD_REQUEST;
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int fd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    if (fd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        int data_read = 0;
        int read_index = 0;
        int checked_index = 0;
        int start_line = 0;
        CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
        while (1)
        {
            data_read = recv(fd, buffer + read_index, BUFFER_SIZE - read_index, 0);
            if (data_read == -1)
            {
                printf("reading failed\n");
                break;
            }
            else if (data_read == 0)
            {
                printf("remote client has closed the connection\n");
                break;
            }

            read_index += data_read;
            HTTP_CODE result = parse_content(buffer, checked_index, checkstate, read_index, start_line);
            if (result == NO_REQUEST)
            {
                continue;
            }
            else if (result == GET_REQUEST)
            {
                send(fd, szret[0], strlen(szret[0]), 0);
                break;
            }
            else
            {
                send(fd, szret[1], strlen(szret[1]), 0);
                break;
            }
        }
        close(fd);
    }

    close(listenfd);
    return 0;
}
