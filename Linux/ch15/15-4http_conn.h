#ifndef __HTTP_CONN_H
#define __HTTP_CONN_H

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>

#include "../14/14-2locker.h"

class http_conn
{
public:
    // 文件名的最大长度
    static const int FILENAME_LEN = 200;
    // 读缓冲区的大小
    static const int READ_BUFFER_SIZE = 2048;
    // 写缓冲区的大小
    static const int WRITE_BUFFER_SIZE = 1024;
    // HTTP请求方法
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATCH
    };
    // 解析客户请求时，主状态机所处的状态
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    // 服务器处理HTTP请求的可能结果
    // 1. NO_REQUEST 请求不完整，需要继续读取客户数据 1xx
    // 2. GET_REQUEST 获得了一个完整的客户请求 2xx
    // 3. BAD_REQUEST 客户请求语法错误 400
    // 4. NO_RESOURCE 资源不存在 404
    // 5. FORBIDDEN_REQUEST 客户对资源没有足够的访问权限 403
    // 6. FILE_REQUEST 请求文件 200
    // 7. INTERNAL_ERROR 服务器内部错误 500
    // 8. CLOSED_CONNECTION 客户端已经关闭连接 503
    enum HTTP_CODE
    {
        NO_REQUEST, GET_REQUEST, BAD_REQUEST,
        NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST,
        INTERNAL_ERROR, CLOSED_CONNECTION
    };
    // 行的读状态
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };

public:
    http_conn() {}
    ~http_conn() {}

public:
    // 初始化新接受的链接
    void init(int sockfd, const sockaddr_in &addr);
    // 关闭连接
    void close_conn(bool real_close = true);
    // 处理客户请求
    void process();
    // 非阻塞读操作
    bool read();
    // 非阻塞写操作
    bool write();

private:
    // 初始化连接
    void init();
    // 解析HTTP请求
    HTTP_CODE process_read();
    // 填充HTTP应答
    bool process_write(HTTP_CODE ret);

    // 下面这一组函数被 process_read 调用以分析HTTP请求
    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();
    char *get_line() { return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();

    // 下面这一组函数被process_write调用以填充HTTP应答
    void unmap();
    bool add_response(const char *format, ...);
    bool add_content(const char *content);
    bool add_status_line(int status, const char *title);
    bool add_headers(int content_length);
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_blank_line();

public:
    // 所有socket上的时间都被注册到同一个epoll内核事件表中，所以被epoll文件描述符设置为静态
    static int m_epollfd;
    // 统计用户数量
    static int m_user_count;

private:
    
    int m_sockfd;   // 该HTTP连接的socket，就是每个客户端连接时的 connfd，accept 返回值
    sockaddr_in m_address;  // 该HTTP连接的socket地址

    char m_read_buf[READ_BUFFER_SIZE]; // 读缓冲区
    char m_write_buf[WRITE_BUFFER_SIZE]; // 写缓冲区
    int m_start_line; // 当前正在解析的行的起始位置
    int m_read_idx; // 标识读缓冲区中已经读入的客户数据的最后一个字节的下一个位置
    int m_checked_idx; // 当前正在分析的字符在缓冲区中的位置
    int m_write_idx; // 写缓冲区待发送的字节数
   
    CHECK_STATE m_checked_state; // 主状态机当前所处的状态
    METHOD m_method; // 请求方法

    // 客户请求的目标文件的完整路径，其内容等于doc_root + m_url, doc_root是网站根目录
    char m_real_file[FILENAME_LEN];
    char *m_url; // 客户请求的目标文件的文件名
    char *m_version; // HTTP协议版本号，我们仅支持HTTP/1.1
    char *m_host; // 主机名
    int m_content_length; // HTTP请求的消息体的长度
    bool m_linger; // HTTP请求是否要求保持连接

    
    char *m_file_address; // 客户请求的目标文件被mmap到内存的起始位置
    // 目标文件的状态。通过它我们可以判断文件是否存在、是否为目录、是否可读，并获取文件大小等信息
    struct stat m_file_stat;
    // 我们将采用 writev 来执行写操作，所以定义下面两个成员，其中 m_iv_count 表示被写内存块的数量
    struct iovec m_iv[2];
    int m_iv_count;
};
#endif