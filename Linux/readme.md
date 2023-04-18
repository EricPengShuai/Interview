## Linux 高性能服务器编程

### 第五章 Linux 网络编程基础 API

#### 5.1 socket 地址 API

##### 5.1.1 主机字节序和网络字节序

- 大端字节序（网络字节序）：高位低地址
- 小端字节序（主机字节序）：高位高地址

```cpp
union {
    short value;
    char union_bytes[sizeof(short)];
} test;
test.value = 0x0102;
test.union_bytes[0] == 1 && test.union_bytes[1] == 2; // 大端
test.union_bytes[0] == 2 && test.union_bytes[1] == 1; // 小端
```



一般网络编程中，发送端会将发送的数据转换成大端字节序数据后再发送，接收端这边根据自身采用的字节序决定是否对接受的数据进行转换（小端机转换，大端机不转），Linux 提供一下函数：

```cpp
// h: host; n: network; l: long; s: short
// long: 往往用来转换 IP; short: 往往用来转换 port
#include <netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htons(unsigned short int hostshort);
unsigned long int ntohl(unsigned long int netlong);
unsigned short int ntohs(unsigned long int netshort);
```



##### 5.1.2 通用 socket 地址

sockaddr 结构体表示 socket 地址

```cpp
#include <bits/socket.h>	// <sys/scoket.h>
struct sockaddr {
    sa_family_t sa_family;	// 常见的地址簇有: AF_UNIX/AF_INET/AF_INET6
    char sa_data[14];	// 存放 socket 地址值，不同的地址簇地址值长度不同
}
```

地址簇（address family）和协议簇（protocol family）一一对应，两者是一样的，经常混用

- AF_*: AF_UNIX / AF_INET / AF_INET6
- PF_*: PF_UNIX / PF_INET / PF_INET6

UNIX 本地域协议簇和 TCP/IPv6 协议簇的地址值长度远远超过 14 字节，因为引入了一个新的通用 socket 地址结构体 sockaddr_storage，具体参考 P72



##### 5.1.3 专用 socket 地址

上面的 sa_data 将 IP 地址和端口号混在一起，实际使用中经常使用 sockaddr_in 和 sockaddr_in6 结构体，分别针对于 AF_INET 和 AF_INET6 地址簇

但是所有使用专用 socket 地址以及 sockaddr_storage 类型的变量在实际中又需要**强制转换**成通用的 socket 地址类型 sockaddr，因为所有的 socket 编程结构的地址参数类型都是 sockaddr



##### 5.1.4 IP 地址转换函数

Linux 提供三个「点分十进制字符串表示的 IPv4 地址和用网络字节序整数表示的 IPv4 地址之间转换」的接口

```cpp
#include <arpa/inet.h>
in_addr_t inet_addr(const char* strptr); // 失败返回 INADDR_NONE
int inet_aton(const char* cp, struct in_addr* inp); // 失败返回 0，成功返回 1

char* inet_ntoa(struct in_addr in); // 逆向转换，注意该函数的不可重入性，具体参考 P73
```

更适用与 IPv4/IPv6 的函数为：

```cpp
#include <arpa/inet.h>
int inet_pton(int af, const char* src, void* dst);	// af 是地址簇

// 逆向转换，len 可以为 INET_ADDRSTRLEN(16, IPv4) / INET6_ADDRSTRLEN(46, IPv6)
const char* inet_ntop(int af, const void* src, char* dst, socklen_t len); // 失败返回 NULL
```



#### 5.2 创建 socket

UNIX/Linux 一切皆文件的思想也囊括 socket，它是一个可读、可写、可控制、可关闭的文件描述符，创建的一个 socket 的系统调用为：

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// domain: 协议簇，包括 PF_INET、PF_INET6、PF_UNIX
// type: 服务类型，包括 SOCK_STREAM（TCP流服务）、SOCK_DGRAM（UDP数据报服务）
// protocol: 前两个参数已经决定了协议，一般设置为 0 即可
int socket(int domain, int type, int protocol);	// 失败返回 -1
```

type 在 Linux 2.6.17 版本可以接受 SOCK_NONBLOCK 和 SOCK_CLOEXEC 宏，具体含义参考 P75



#### 5.3  命名 socket

将 socket 与具体的 IP 地址绑定称为命名 socket，服务端程序中只有命名 socket 之后客户端才能知道该如何连接它，客户端通常不需要命名 socket，而是采用匿名方式。

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// 将 my_addr 所指的 socket 地址分配给 未命名的 sockfd 文件描述符
int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen); // 失败返回 -1
```



#### 5.4 监听 socket

socket 被命名之后还需要创建**监听队列**以存放待处理的客户端连接，才能接受客户端连接

```cpp
#include <sys/socket.h>

// backlog 是内核监听队列的最大长度，表示服务端完全连接状态 ESTABLISHED 数量的上限+1
int listen(int sockfd, int backlog); // 失败返回 -1，成功返回 0
```

> 具体测试参考 P77



#### 5.5 接受连接

从 listen 监听队列中接受一个连接的系统调用为：

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// sockfd 是执行过 listen 系统调用的监听 socket，处于 LISTEN 状态
// addr 用来获取被接受连接的远端 socket 地址
int accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen);
```

accept 成功调用会返回一个新的**连接 socket**（处于 ESTABLISHED 状态），该套接字唯一标识这个被接受的连接，服务器可以通过读写该 socket 来与客户端通信

> 注意：accept 并不关心任何网络状况的变化，只是从监听队列中取出连接，而不论连接处于何种状态，参考 P80



#### 5.6 发起连接

服务端是通过 listen 被动接受连接，客户端就需要通过 connect 系统调用主动发起与服务端的连接：

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// sockfd 是客户端自己创建的套接字
// serv_addr 是服务器监听的 socket 地址
int connect(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen);
```

成功调用返回 0，客户端的 sockfd 就唯一标识这个连接，客户端就可以通过读写 sockfd 来与服务器通信



#### 5.7 关闭连接

通过减少引用计数关闭连接，并不是真正关闭连接，只有当引用计数为 0 时才真正关闭

```cpp
#include <unistd.h>
int close(int fd);	
```

立即关闭连接可以使用：

```cpp
#include <sys/socket.h>

// howto: 关闭读 SHUT_RD、关闭写 SHUT_WR、关闭读写 SHUT_RDWR
int shutdown(int sockfd, int howto); // 成功返回 0，失败 -1
```



#### 5.8 数据读写

##### 5.8.1 TCP 数据读写

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// buf 和 len 分别是读缓冲区的位置和大小，flags 一般为 0
// recv 成功时返回实际读取的数据长度，可能小于 len
// 返回 0 表示对方已经关闭连接，-1 表示出错
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

// send 成功时返回实际写入的数据长度，失败时返回 -1
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

> flags 选项可以设置 send 或者 recv 读取或者发送数据的方式，比如处理紧急数据（带外数据）等



##### 5.8.2 UCP 数据读写

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// 和 recv 不同的是需要指定发送端的 socket 地址，因为 UDP 没有连接的概念
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen);

// 指定接收端的 socket 地址
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, struct sockaddr* dest_addr, socklen_t* addrlen);
```

这两个函数也可以用于 TCP 接受数据，只需要将后面两个参数设置为 NULL 表示忽略发送端/接收端地址



##### 5.8.3 通用数据读写函数

不区分 TCP 和 UDP，更加通用的数据读写函数为：

```cpp
#include <sys/socket.h>
ssize_t recvmsg(int sockfd, struct msghdr* msg, int flags);
sszie_t sendmsg(int sockfd, struct msghdr* msg, int flags);
```

msghdr 结构体为：具体含义参考 P85

```cpp
struct msghdr {
    void* msg_name;	// socket 地址
    socklen_t msg_namelen;	// 地址长度
    struct iovec* msg_iov;	// 存放数据：分散读、集中写
    int msg_iovlen; // iovec 结构体个数
    void* msg_control;
    socklen_t msg_controllen;
    int msg_flags;	// 复制函数中 flags 参数，调用过程中更新
};
```



#### 5.9 带外标记

内核通知应用程序带外数据到达的两种方式：**「IO复用产生的异常事件」和 「SIGURG 信号」**

```cpp
#include <sys/socket.h>

// 判断 sockfd 是否处于带外标记，即下一个被读到的数据是否是带外数据
// 如果是返回 1，此时就可以设置 flags 为 MSG_OOB 标志的 recv 调用来接受带外数据；否则返回 0
int sockatmark(int socket);
```



#### 5.10 地址信息函数

```cpp
#include <sys/socket.h>

// 获取 sockfd 对应的本端 socket 地址，存在 addr 中
int getsockname(int sockfd, struct sockaddr* addr, socklen_t* addlen); // 成功返回 0，失败 -1

// 获取 sockfd 对应的远端 socket 地址，存在 addr 中
int getpeername(int sockfd, struct sockaddr* addr, socklen_t* addlen); // 成功返回 0，失败 -1
```



#### 5.11 socket 选项

```cpp
#include <sys/socket.h>

// level: 指定哪个协议，包括 SOL_SOCKET、IPPROTO_IP、IPPROTO_IPV6、IPPROTO_TCP
// option_name: 指定选项名称
// option_value 和 option_len 分别是选项的值和长度
int getsockopt(int sockfd, int level, int option_name, void* option_value,
               socketlen_t* restrict option_len); // 成功返回 0，失败 -1

int setsockopt(int sockfd, int level, int option_name, const void* option_value,
               socketlen_t option_len); // 成功返回 0，失败 -1
```

- SO_REUSEADDR：强制使用被处于 TIME_WAIT 状态的连接占用的 socket 地址
- SO_RCVBUF：修改 TCP 接受缓冲区的大小（翻倍设置），最小为 256 字节
- SO_SNDBUF：修改 TCP 发送缓冲区的大小（翻倍设置），最小为 2048 字节
- SO_RCVLOWAT 和 SO_SNDLOWAT：TCP 接受缓冲区和发送缓冲区的低水位标记，用于 IO 复用中判断何时可读写



#### 5.12 网络信息 API

```cpp
#include <netdb.h>

// 根据主机名称获取主机的完整信息
struct hostent* gethostbyname(const char* name);

// 根据 IP 地址获取主机的完整信息
struct hostent* gethostbyaddr(const void* addr, size_t len, int type);
```

返回的都是 hostent 结构体类型的指针，更多具体的 API 参考 P95
