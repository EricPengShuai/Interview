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
            /* 如果"\r"字符碰巧是目前buffer中的最后一个已经被读入的客户数据，
             * 那么分析没有读取到一个完整的行，返回LINE_OPEN以表示还需要继续
             * 读取客户数据才能进一步分析 */
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
HTTP_CODE parse_requestline(char *temp, CHECK_STATE &checkstate)
{
    char *url = strpbrk(temp, " \t"); // 寻找分隔符 todo: 不知道为啥我单独测试，这里 temp 如果不含 " \t" 字符会直接报错的
    if (!url)                         // 如果请求行中没有空白字符或“\t”字符，则HTTP请求必有问题
    {
        return BAD_REQUEST;
    }
    *url++ = '\0'; // *url = '\0', url++; todo: 不知道为啥我单独测试，直接这样复制会 bus error

    char *method = temp;
    if (strcasecmp(method, "GET") == 0) /* 仅支持GET方法 */
    {
        printf("The request method is GET\n");
    }
    else
    {
        return BAD_REQUEST;
    }

    url += strspn(url, " \t"); // 跳过分隔符
    char *version = strpbrk(url, " \t");
    if (!version)
    {
        return BAD_REQUEST;
    }

    *version++ = '\0';
    version += strspn(version, " \t");
    /* 仅支持HTTP/1.1 */
    if (strcasecmp(version, "HTTP/1.1") != 0)
    {
        return BAD_REQUEST;
    }

    /* 检查URL是否合法 */
    if (strncasecmp(url, "http://", 7) == 0)
    {
        url += 7;
        url = strchr(url, '/');
    }

    if (!url || url[0] != '/')
    {
        return BAD_REQUEST;
    }

    printf("The request URL is %s\n", url);
    /* HTTP 请求行处理完毕，状态转移到头部字段的分析 */
    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

/* 分析头部字段 */
HTTP_CODE parse_headers(char *temp)
{
    /* 遇到一个空行，说明我们得到了一个正确的HTTP请求 */
    if (temp[0] == '\0')
    {
        return GET_REQUEST;
    }
    else if (strncasecmp(temp, "Host:", 5) == 0) /* 处理"HOST"头部字段 */
    {
        temp += 5;
        temp += strspn(temp, " \t");
        printf("the request host is: %s\n", temp);
    }
    else /* 其他头部字段暂不处理 */
    {
        printf("I can not handle this header\n");
    }

    return NO_REQUEST;
}

/* 分析HTTP请求的入口函数 */
HTTP_CODE parse_content(char *buffer, int &checked_index, CHECK_STATE &checkstate,
                        int &read_index, int &start_line)
{
    LINE_STATUS linestatus = LINE_OK; /* 记录当前行的读取状态 */
    HTTP_CODE retcode = NO_REQUEST;   /* 记录HTTP请求的处理结果 */
    /* 主状态机，用于从buffer中取出所有完整的行 */
    while ((linestatus = parse_line(buffer, checked_index, read_index)) == LINE_OK)
    {
        char *temp = buffer + start_line; /* start_line是行在buffer中的起始位置 */
        start_line = checked_index;       /* 记录下一行的起始位置 */
        /* checkstate 记录主状态机当前的状态 */
        switch (checkstate)
        {
        case CHECK_STATE_REQUESTLINE: /* 第一个状态，分析请求行 */
        {
            retcode = parse_requestline(temp, checkstate);
            if (retcode == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            break;
        }
        case CHECK_STATE_HEADER: /* 第二个状态，分析头部字段 */
        {
            retcode = parse_headers(temp);
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

    /* 若没有读取到一个完整的行，则表示还需要继续读取客户数据才能进一步分析 */
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

            // printf("recv content: %s\n", buffer + read_index);
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
