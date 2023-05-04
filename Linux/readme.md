## Linux 高性能服务器编程

### Ch.5 Linux 网络编程基础 API

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

> 参考代码：[5-1byteorder.cpp](ch5/5-1byteorder.cpp)

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

// backlog 是内核监听队列的最大长度，表示服务端完全连接状态 ESTABLISHED 数量的上限（backlog+1）
// Mac 环境中测试是监听上限就是 backlog
int listen(int sockfd, int backlog); // 失败返回 -1，成功返回 0
```

> 代码参考：[5-3testlisten.cpp](ch5/5-3testlisten.cpp)



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

> 注意：accept 并不关心任何网络状况的变化，只是从监听队列中取出连接，而不论连接处于何种状态
>
> 代码参考：[5-5testaccept.cpp](ch5/5-5testaccept.cpp)



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
>
> 带外数据处理代码参考：[客户端-5-6oobsend.cpp](ch5/5-6oobsend.cpp)，[服务端-5-7oobrecv.cpp](ch5/5-7oobrecv.cpp)



##### 5.8.2 UDP 数据读写

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

  > 代码参考：[5-9reuse_address.cpp](ch5/5-9reuse_address.cpp)

- SO_RCVBUF：修改 TCP 接受缓冲区的大小（翻倍设置），最小为 256 字节

  > 代码参考：[5-11set_recv_buffer.cpp](ch5/5-11set_recv_buffer.cpp)

- SO_SNDBUF：修改 TCP 发送缓冲区的大小（翻倍设置），最小为 2048 字节

  > 代码参考：[5-10set_send_buffer.cpp](ch5/5-10set_send_buffer.cpp)

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

> 代码参考：[5-12access_daytime.cpp](ch5/5-12access_daytime.cpp)



### Ch.6 高级 I/O 函数

Linux 提供很多高级 IO 函数，没有 read/open 等基础的常用，但是特定地方使用性能较高，一般有三类

- **创建文件描述符**的函数：pipe、dup/dup2
- **读写数据**的函数：包括 readv/writev、sendfile、mmap/munmap、splice、tee 等
- **控制 IO 行为和属性**的函数：fcntl



#### 6.1 pipe 函数

```cpp
#include <unistd.h>

// 往 fd[1] 写入的数据可以从 fd[0] 读出，不能反过来
int pipe(int fd[2]); // 成功返回 0，并将一对打开的文件描述符填入其参数指向的数组
```

- 管道内部传输的数据是字节流
- 管道有容量限制，默认大小为 65536 字节
- socketpair 函数可以在本地域 AF_UNIX 创建双向管道



#### 6.2 dup/dup2 函数

```cpp
#include <unistd.h>

// 返回一个新的文件描述符，和旧的文件描述符 old_fd 指向相同的文件、管道或者网络连接
// 返回系统当前可用的最小整数值
int dup(int old_fd);

// 和 dup 类似，返回的文件描述符不小于 limit_fd
int dup2(int old_fd, int limit_fd);
```

P102 例子中，写了一个简单的服务端程序，与客户端通信的 socket 记为 connfd，先关闭标准输出 STDOUT_FILENO (其值为1)，**然后调用 dup(connfd) 返回 1**，这样标准输出就和 connfd 指向同样的文件，也就是 printf 的数据直接写入管道（不会出现在终端上），发送给客户端，这就是 Comman Gateway Interface（CGI）服务器的基本工作原理

> 代码参考：[6-1testdup.cpp](ch6/6-1testdup.cpp)



#### 6.3 readv/writev 函数

简单来说 readv 是分散读，writev 是集中写，相当于 recvmsg/sendmsg

```cpp
#include <sys/uio.h>

// fd 是被操作的 socket，vector 是 iovec 结构数组，iovec 结构描述的是一块内存区，count 参数是 vector 数组长度
// 成功时返回读出/写入 fd 的字节数，失败返回 -1 并设置 errno 
ssize_t readv(int fd, const struct iovec* vector, int cnt);
ssize_t writev(int fd, const struct iovec* vector, int cnt);

strcut iovec {
    void *iov_base; // 内存起始地址
    size_t iov_len; // 内存长度
};
```

P105 例子中给了简单 HTTP 文件服务器，通过 writev 将 headbuf（状态行+头部字段+空行）和 filebuf（文档内容）集中写入 socket

> 代码参考：[6-2testwritev.cpp](ch6/6-2testwritev.cpp)



#### 6.4 sendfile 函数

sendfile 在两个文件描述符之间直接传递数据，完全在内核中操作，避免了内核缓冲区和用户缓冲区之间的数据拷贝，这就是**零拷贝**

```cpp
#include <sys/sendfile.h>

// in_fd --sendfile--> out_fd
// in_fd 表示待读出内容的文件描述符，out_fd 表示待写入内容的文件描述符
// offset 表示 in_fd 的起始位置，count 表示 in_fd 和 out_fd 之间传输的字节数
ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count); // 成功时返回传输的字节数，失败返回-1
```

- in_fd 必须是一个支持类似 mmap 函数的文件描述符，必须指向真实的文件，不能是 socket 和管道
- out_fd 必须是一个 socket

> sendfile 几乎是专门为在网络上传输文件而设计的，注意 MacOS 的 sendfile 参数和 Linux 不太一样，参考 [sendfile.2](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/sendfile.2.html)

P107 例子使用 sendfile 将服务器上的一个文件传输给客户端，其中没有**为目标文件分配任何用户空间的缓存，也没有执行读取文件的操作**，相比于之前的 通过 HTTP 传输文件的效率要高得多

> 代码参考：[6-3testsendfile.cpp](ch6/6-3testsendfile.cpp)



#### 6.5 mmap/munmap 函数

mmap 用于申请一段内存空间，munmap 则释放由 mmap 创建的这段内存空间

```cpp
#include <sys/mman.h>

// start: 待分配内存的起始地址，如果为 null 则系统自动分配一个地址
// length: 指定内存段的长度；prot: 设置内存段的访问权限，可以按位或取 PROT_READ|PROT_WRITE|PROT_EXEC|PROT_NONE
// fd 是被映射文件对应的文件描述符，一般通过 open 获得
// [return] 成功时返回指向目标内存区域的指针，失败 -1
void* mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *start, size_t length); // 失败 -1，成功 0
```



#### 6.6 splice 函数

splice 函数用于两个文件描述符之间移动数据，是**零拷贝操作**

```cpp
#include <fcntl.h>

// fd_in 表示待输入数据的文件描述符
// len 指定移动数据的长度
// flags 控制数据如何移动，取异或值：SPLICE_F_MOVE|SPLICE_F_NONBLOCK|SPLICE_F_MORE
ssize_t splice(int fd_in, loff_t* off_in, int fd_out, loff_t* off_out, size_t len, unsigned int flags); // 成功时返回移动字节的数量，失败 -1
```

fd_in/fd_out 必须至少有一个是管道文件描述符

- fd_in 如果是管道文件描述符，off_in 参数必须被设置为 NULL
- fd_in 如果不是一个管道文件描述符，off_in 指定输入数据流的何处开始读取数据

P110 例子实现了一个简单的回射服务器，利用 splice 函数将客户端的内容读入管道写端 fd[1]，然后再使用 splice 函数从管道读端 fd[0] 读出该内容到客户端。整个过程没有执行 recv/send 操作，十分高效

> 代码参考：[6-4testsplice.cpp](ch6/6-4testsplice.cpp) （注：MacOS 并没有 splice 函数）



#### 6.7 tee 函数

tee 函数在两个管道文件描述符之间复制数据，是零拷贝操作

```cpp
#include <fcntl.h>

// fd_in 和 fd_out 必须都是管道文件描述符
// [return] 成功时返回复制的字节数，失败 -1
ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags);
```

P111 例子实现了一个简单的 tee 程序，利用 splice（标准输入输出<-->输入输出管道） 和 tee（输出管道<-->文件管道）同时输出数据到终端和文件

> 代码参考：[6-5testtee.cpp](ch6/6-5testtee.cpp)



#### 6.8 fcntl 函数

file control 函数提供对文件描述符的各种控制操作

```cpp
#include <fcntl.h>

// fd 是被操作的文件描述符，cmd 指定执行何种类型的操作
// 根据操作类型不同可能还需要第3个可选参数 arg
int fcntl(int fd, int cmd, ...); // 失败 -1
```

- F_GETFD/F_SETFD：获取和设置文件描述符的标志
- F_GETFL/F_SETFL：获取和设置文件描述符的状态标志

P113 代码清单中首先 F_GETFL 获取 fd 的旧状态标志，然后 F_SETFL 将 fd 设置为非阻塞状态



### Ch.7 Linux 服务器程序规范

#### 7.1 日志

##### 7.1.1 Linux 系统日志

rsyslogd 守护进程技能接收用户进程输出的日志，又能接收内核日志，通过调用 syslog 函数生成系统日志，该函数将日志输出到一个 UNIX 本地域 socket 类型 AF_UNIX 的 文件 /dev/log 中，具体参考 P115 Linux 的系统日志体系。



##### 7.1.2 syslog 函数

```cpp
#include <syslog.h>

// priority 是设施值（LOG_USER）与日志级别的按位或，7种日志级别参考 P115
void syslog(int priority, const char* message, ...);
```

openlog 可以改变 syslog 的默认输出方式，进一步结构化日志内容 

```cpp
#include <syslog.h>

// ident 参数指定的字符串被添加到日志消息的日期和时间之后，一般为程序的名字
void openlog(const char* ident, int logopt, int facility);
```

程序开发过程中需要输出很多调试信息，而发布之后又需要将这些调式信息关闭，这时候需要对日志进行过滤

```cpp
#include <syslog.h>

// 日志级别大于日志掩码的日志信息会被系统忽略
int setlogmask(int maskpri);

// 最后需要关闭日志
void closelog();
```



#### 7.2 用户信息

##### 7.2.1 uid/euid/gid/egid

```cpp
#include <sys/types.h>
#include <unistd.h>
uid_t getuid();		// 获取真实用户 id
uid_t geteuid();	// 获取有效用户 id
gid_t getgid();		// 获取真实组 id
gid_t getegid();	// 获取有效组 id
```

P117 代码清单 7-1 展示了 UID 和 EUID 的区别，代码参考：[7-1testeuid.cpp](ch7/7-1testeuid.cpp)



##### 7.2.2 切换用户

代码清单 7-2 展示了以 root 身份启动的进程切换为一个普通用户身份运行，没看懂o(╥﹏╥)o

> root 的 uid == 0 && guid == 0?
>
> 代码参考：[7-2switchuser.cpp](ch7/7-2switchuser.cpp)



#### 7.3 进程间关系

##### 7.3.1 进程组

每个进程都隶属于一个进程组，进程组有进程组 ID（PGID），首领进程的 PID 和 PGID 相同

```cpp
#include <unistd.h>
pid_t getpgid(pid_t pid); // 成功返回 pid 的进程组的 PGID，失败 -1
int setpgid(pid_t pid, pid_t pgid); // 设置 pid 的进程组的 PGID 为 pgid，成功 0，失败 -1
```

一个进程只能设置自己或者子进程的 PGID，并且子进程调用 exec 系列函数之后不能再在父进程中对它设置 PGID



##### 7.3.2 会话

一些关联的进程组形成一个会话 session，创建会话

```cpp
#include <unistd.h>
pid_t setsid(void); // 只能由非首领进程创建会话，调用进程成为会话的首领
pid_t getsid(pid_t pid); // 读取会话ID SID，Linux 系统认为 SID==PGID
```



##### 7.3.3 ps 查看进程关系

P119 图 7-2 很清晰的展示了进程之间的关系，不同进程组成进程组，不同进程组组成会话



#### 7.4 系统资源限制

Linux 系统有资源限制，比如物理设备限制、系统策略限制、具体实现的限制等等

```cpp
#include <sys/resource.h>
int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);

// rlim_t 是一个整数类型，描述资源级别
struct rlimit
{
    rlim_t rlim_cur; // 资源的软限制，建议性的、最好不要超越的限制，超过可能会发信号终止进程
    rlim_t rlin_max; // 资源的硬限制，软限制的上限，普通程序只能减少，只有 root 可以增加
}
```

ulimit 命令可以修改**当前 shell** 环境下的资源限制，也可以通过修改配置文件来改变系统的软限制和硬限制，这种修改是**永久的**。



#### 7.5 改变工作目录和根目录

有些服务器程序还需要改变工作目录和根目录

```cpp
#include <unistd.h>

// buf 指向内存用于存储进程当前工作目录的绝对路径名，size 指定其大小
// [return] 成功时返回一个指向目标存储区的指针
char* getcwd(char* buf, size_t size); // 失败返回 NULL 并设置 errno

// path 指定要切换到的目标目录
int chdir(const char* path); // 成功 0，失败 -1

// path 指定要切换到的目标根目录
int chroot(const char* path); // 成功 0，失败 -1
```



#### 7.6 服务器程序后台化

如何让进程以守护进程的方式运行？参考代码清单 7-3，实际上提供如下的系统调用：

```cpp
#include <unistd.h>

// nochdir 用于指定是否改变工作目录，0 --> 工作目录设置为 "/"，否则留在当前目录
// noclose 为 0 时标准输入输出以及错误输出都被重定向到 /dev/null 文件，否则依然使用原来的设备
int daemon(int nochdir, int noclose); // 成功 0，失败-1
```

> 代码参考：[7-3daemonize.cpp](ch7/7-3daemonize.cpp)



### Ch.8 高性能服务器程序框架

#### 8.1 服务器模型

##### 8.1.1 C/S 模型

服务器客户端模式

- 服务器：创建 socket --> bind 地址 --> listen --> select IO 复用 --> accept --> 逻辑单元（fork子进程、子线程或其他）
- 客户但：socket --> connect --> send --> recv

缺点：访问量过大时，服务器负载加大，客户端得到的响应变慢



##### 8.1.2 P2P 模型

点对点模式中主机即使服务端又是客户端，缺点是：用户之间传输的请求过多时，网络的负载加重

通常 P2P 模型带有一个专门的发现服务器，提供查找服务



#### 8.2 服务器编程框架

- IO 处理单元：服务器管理客户连接的模块
- 逻辑单元：进程或线程，分析并处理数据，然后将结果传递给 IO 处理单元或者直接返回给客户端
- 网络存储单元：数据库、缓存或文件，可选的



#### 8.3 IO 模型

阻塞 I/O 和 非阻塞 I/O，P126 描述了阻塞的 connect 工作流程，无法立即完成被系统挂起，直到等待的时间发生为止

- socket 相关 API 中，可能被阻塞的系统调用包括 accept、send、recv 和 connect
- 非阻塞 I/O 执行的系统调用如果时间没有立即发生，返回 -1，这和出错的情况一样，此时需要根据 errno 来区分
- **I/O 复用函数本身是阻塞的**，它们能提高程序效率的原因在于它们具有同时**监听多个 I/O 事件的能力**

同步 I/O 向应用程序通知的是 I/O 就绪事件，异步 I/O 通知的是 I/O 完成事件



#### 8.4 两种高效的事件处理模式

同步 I/O 模型通常用于实现 Reactor 模式，异步 I/O 模型则用于实现 Proactor 模式

##### 8.4.1 Reactor 模式

- 主线程（I/O 处理单元）只负责监听文件描述上是否有时间发生，有的话立即通知工作线程
- 工作线程（逻辑单元）读写数据、接收新的连接以及处理客户端请求

P128 图 8-5 展示同步 I/O epoll_wait 实现的 Reactor 模式，主线程通过 epoll_wait 监听到 socket 上有数据可读或可写时，将这个可读或可写时间放入请求队列，工作线程从请求队列中取事件



##### 8.4.2 Proactor 模式

- 主线程（I/O 处理单元）负责所有的 I/O 操作
- 工作线程（逻辑单元）仅仅负责业务逻辑

图 8-6 展示了异步 I/O aio_read/aio_write 实现的 Proactor 模式，没看懂o(╥﹏╥)o



##### 8.4.3 模拟 Proactor 模式

使用同步 I/O epoll_wait 模拟 Proactor 模式，让主线程执行数据读写操作，读写完成之后，主线程向工作线程通知这一“完成事件”。那么从工作进程的角度上来看，它们就直接获得了数据的读写结果，接下来要做的就是对读写的结果进行逻辑处理。

> 流程依旧没看懂o(╥﹏╥)o



#### 8.5 两种高效的并发模式

##### 8.5.1 半同步/半异步模式

- 同步：程序完成按照代码序列的顺序执行
- 异步：程序的执行需要由系统事件来驱动，常见的系统事件包括中断、信号等

异步线程执行效率高、实时性强，但是难以调试和扩展，不适合大量并发；同步线程虽然执行效率较低、实时性差，但逻辑简单。服务器一般及要求较好的实时性，又要求能同时处理多个客户请求的应用程序，所以采用**「半同步/半异步模式」**实现



##### 8.5.2 领导者/追随者模式

多个工作线程轮流获得事件源集合，轮流监听、分发并处理时间的一种模式。在任意时间点，程序都仅有一个领导者线程，负责监听 I/O 事件。而其他线程都是追随者，它们休眠在线程池中等待成为新的领导者。

- 句柄集 HandleSet
- 线程集 ThreadSet
- 时间处理器 EventHandler



#### 8.6 有限状态机

程序清单 8-3 展示了 HTTP 请求的读取和分析中主从状态机是如何处理 HTTP 请求字段的

> 代码参考：[8-3httpparser.cpp](ch8/8-3httpparser.cpp)



#### 8.7 提高服务器性能的其他建议

##### 8.7.1 池

池通过空间换时间的思想来提高服务器的运行效率，相当于服务器管理系统资源的应用设施，避免了服务器对内核的频繁访问。

- 内存池通常用于 socket 的接收缓存和发送缓存
- 进程池和线程池通常用于并发编程，当需要一个工作进程或线程来处理新的客户请求时，可以直接从池中取得一个执行实体，这样就无需动态地调用 fork 或 pthread_create 等函数来创建进程和线程
- 连接池通常用于服务器或服务器机群的内部永久连接，连接池是服务器预先和数据库程序建立的一组连接的集合



##### 8.7.2 数据复制

内核直接处理从 socket 或者文件读入的数据，所以应用程序没必要将这些数据从内核缓冲区复制到应用程序缓冲区中，使用“零拷贝”函数 sendfile 等就可以将一个文件直接发送给客户端。

另外当两个工作进程之间需要传递大量的数据时，应考虑使用共享内存来共享这些数据，而不是使用管道或者消息队列来传递，这样就可以**避免不必要的数据复制**。



##### 8.7.3 上下文切换和锁

并发程序必须考虑上下文切换的问题，即进程切换或线程切换导致的系统开销；还需要考虑共享资源的加锁保护，如果服务器必须使用“锁”，则可以考虑减小锁的粒度。



### Ch.9 I/O 复用

#### 9.1 select 系统调用

select 系统调用的原型如下：

```cpp
#include <sys/select.h>

// nfds 指定被监听的文件描述符的总数，通常是监听的所有文件描述符中的最大值加1
// readfds, writefds, exceptfds 分别指向可读、可写和异常等事件对应的文件描述符集合
// timeout 设置 select 函数的超时时间，0 立即返回，NULL 一直阻塞
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);

// fd_set 结构体仅包含一个整形数组，该数组的每个元素的每一位 bit 标记一个文件描述符，容纳数量由 FD_SETSIZE 指定
FD_ZERO(fd_set* fdset);			// 清除 fdset 的所有位
FD_SET(int fd, fd_set *fdset);	// 设置 fdset 的位 fd
FD_CLR(int fd, fd_set *fdset);	// 清除 fdset 的位 fd
int FD_ISSET(int fd, fd_set *fdset); // 测试 fdset 的位 fd 是否被设置

struct timeval
{
    long tv_sec;	// 秒数
    long tv_usec;	// 微秒数
}
```

**select 成功时返回就绪文件描述符的总数，如果在超时时间内没有任何文件描述符就绪就返回 0，失败 -1 并设置 errno**



**文件描述符可读就绪条件**

- socket 接收缓冲区中的字节数大于或等于低水位标记 SO_RCVLOWAT 时可以无阻塞地读 socket
- socket 通信对方关闭连接时，对该 socket 的读操作返回 0
- 监听 socket 上有新的连接
- socket 上有未处理的错误

**文件描述符可写就绪条件**

- socket 发送缓冲区中的可用字节数大于或等于低水位标记 SO_RCVLOWAT 时可以无阻塞地写 socket
- socket 的写操作被关闭，对该 socket 的读操作返回 0
- socket 使用非阻塞 connect 连接成功或者失败（超时）之后
- socket 上有未处理的错误

代码参考：[9-1use_select.cpp](ch9/9-1use_select.cpp)



#### 9.2 poll 系统调用

poll 和 select 类似，也是在指定时间内轮询一定数量的文件描述符，测试其是否就绪

```cpp
#include <poll.h>

// fds 参数指定感兴趣的文件描述符上发生的可读、可写和异常事件
// nfds 参数指定被监听事件集合 fds 的大小，实际类型为 unsigned long int
// timeout 指定 poll 的超时值，单位是毫秒，-1 永远阻塞，0 直接返回
// [return] 和 select 一样
int poll(struct pollfd* fds, nfds_t nfds, int timeout);

// pollfd 结构体
struct pollfd
{
    int fd;	// 文件描述符
    short events; // 注册的事件，一系列 POLL 事件的按位或
    short revents; // 实际发生的事件，内核填充
}
```



#### 9.3 epoll 系列系统调用

> Mac 里没有 epoll 库，使用 kqueue 代替，[参考](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/kqueue.2.html)

epoll 把用户关心的文件描述符上的事件放入内核里的一个时间表中，无需像 select 和 poll 那样每次调用都要重复传入文件描述符集或事件集。epoll 需要一个额外的文件描述符来唯一标识内核中的这个事件表

```cpp
#include <sys/epoll.h>

// 创建标识内核中的事件表，size 参数并无实际作用
// [return] 返回的 fd 将作为其他所有 epoll 系统调用的第一个参数
int epoll_create(int size);

// 操作内核事件表
// op 参数指定操作类型，由 EPOLL_CTL_ADD|EPOLL_CTL_MOD|EPOLL_CTL_DEL 组成
// fd 参数是要操作的文件描述符
// event 参数指定事件
// [return] 成功 0，失败 -1 并设置 errno
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

struct epoll_event {
    _uint32_t events; // epoll 事件，和 poll 类型基本一致
    epoll_data_t data; // 用户数据
}

// 联合体
typedef union epoll_data {
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

// timeout 指定超时，maxevents 指定最多监听多少个事件
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```

epoll 相比于 poll 有两个额外的事件类型：EPOLLET | EPOLLONESHOT

epoll_wait 将所有就绪的事件从内核事件表（由 epfd 参数指定）中复制到 events 指定的数组中，**只用于输出 epoll_wait 检测到的就绪事件**，所以相比于 select 和 poll 极大提升性能



##### LT 和 ET 模式

- LT：Level Trigger，默认的，epoll_wait 检测到其上有事件发生并将此事件通知应用程序之后，应用程序可以不立即处理该事件，下次调用 epoll_wait 还可以再次向应用程序通告此事件
- ET：Edge Trigger，epoll_wait 检测到就绪事件之后必须处理，效率比 LT 模式要高，需要指定 EPOLLET 事件类型

> 代码参考：[LT vs ET 9-3mtlt.cpp](ch9/9-3mtlt.cpp)



**EPOLLONESHOT 事件**

即使在 ET 模式下，一个 socket 上的某个事件还是可能被触发多次，在并发程序中，一个线程读取完某个 socket 上的数据后开始处理这些数据，但是在处理过程中该 socket 上又有新数据可读（EPOLLIN 被再次出发），此时另一个线程被唤醒来读取这些新的数据。此时出现了两个线程同时操作一个 socket 的局面

为此需要 EPOLLONESHOT 事件

> 代码参考：[9-4oneshot.cpp](ch9/9-4oneshot.cpp)



#### 9.4 三组 I/O 复用函数比较

selecet、poll 和 epoll 都通过某种结构体变量来告诉内核监听哪些文件描述符上的哪些事件，并使用该结构体类型的参数来获取内核处理的结果

- select 参数类型 fd_set 没有将文件描述符和事件绑定，因此需要 3 个类型的参数分别区分可读、可写和异常事件，不能处理更多类型的事件，且下次调用时徐璈重置 3 个 fd_set 集合
- poll 通过参数类型 pollfd 将文件描述符和事件都定义在其中，支持更多的事件类型，且下次调用 poll 时无需重置 pollfd 类型的事件集参数，因为内核修改的仅仅是 revents 成员
- select 和 poll 调用返回整个用户注册的事件集合（包括就绪和未就绪的），索引就绪文件描述符的时间复杂度为 O(n)，epoll 通过 epoll_wait 直接从 epollfd 指定的内核事件表中取得用户注册的事件，且通过 events 参数仅仅用来返回就绪的事件，索引就绪的 fd 事件复杂度为 O(1)
- poll 和 epoll_wait 分别使用 nfds 和 maxevents 参数指定最多监听的 fd 和 事件，最大 65535，但是 select 一般是 1024
- poll 和 selecet 只能工作在相对低效的 LT 模式，epoll 可以在 ET 模式，且还支持 EPOLLONESHOT 事件

> 具体区别参考表格 9-2 



#### 9.5 I/O 复用的高级应用一：非阻塞 connect

connect 出错时有一个 errno 值：EINPROGRESS，**这种错误发生在非阻塞的 sockct 调用 connect，而连接又没有立即建立时**。根据 man 文档的解释，在这种情况下，我们可以调用 select、poll 等函数来监听这个连接失败的 socket 上的**可写事件**。当select、poll 等函数返回后，再利用 getsockopt 来读取错误码并清除该 socket 上的错误。如果错误码是0，表示连接成功建立，否则连接失败。

> 代码参考：[9-5unblockconnect.cpp](ch9/9-5unblockconnect.cpp)



#### 9.6 I/O 复用的高级应用二：聊天室程序

客户端程序有两个功能：

- 从标准输人终端读入用户数据，并将用户数据发送至服务器
- 往标准输出终端打印服务器发送给它的数据

> 代码参考：[9-6mytalk_client.cpp](ch9/9-6mytalk_client.cpp)

服务器的功能

- 接收客户数据
- 把客户数据发送给每一个登录到该服务器上的客户端（数据发送者除外)

> 代码参考：[9-7mytalk_server.cpp](ch9/9-7mytalk_server.cpp)



#### 9.7  I/O 复用的高级应用二：同时处理 TCP 和 UDP 服务

代码参考：[9-8multi_port.cpp](ch9/9-8multi_port.cpp)



#### 9.8 超级服务 xinetd

Linux 因特网服务 inetd 是超级服务。它同时管理着多个子服务，即监听多个遄口。现在 Linux 系统上使用的 inetd 服务程序通常是其升级版本 xinetd。 xinetd 程序的原理与 inetd 相同，但增加了一些控制选项，并提高了安全性。



### Ch.10 信号

信号是由用户、系统或者进程发送给目标进程的信息，以通知目标进程某个状态的改变或系统异常。Linux 信号可由如下条件产生：

- 对于前台进程，用户可以通过输人特殊的终端字符来给它发送信号。比如输入 Ctrl+C 通常会给进程发送一个中断信号
- 系统异常。比如浮点异常和非法内存段访问
- 系统状态变化。比如 alarm 定时器到期将引起 SIGALRM 信号
- 运行 kill 命令或调用 kill 函数



#### 10.1 Linux 信号概述

```cpp
#include <sys/types.h>
#include <signal.h>

// 把信号 sig 发给目标进程 pid
int kill(pid_t pid, int sig); // 成功返回 0，失败 -1 并设置 errno

// 信号处理函数原型
typedef void (*__sighandler_t)(int);
```

linux 信号有很多，和网络编程关系紧密的是：

- SIGHUP：控制终端挂起
- SIGPIPE：往读端被关闭的管道或者 socket 连接中写数据
- SIGURG：socket 连接上接受到紧急数据
- SIGALRM：由 alarm 或 setitimer 设置的实时闹钟超时引起
- SIGCHLD：子进程状态发生变化（退出或暂停）



#### 10.2 信号函数

##### signal 系统调用

```cpp
#include <signal.h>

// sig 参数指出要捕获的信号类型，
// _handler 参数是函数指针，用于指定信号 sig 的处理函数
_sighandler_t signal(int sig, _sighandler_t handler);
```



##### sigaction 系统调用

```cpp
#include <signal.h>

// sig 指出要捕获的信号类型
// act 参数指定新的信号处理方式
// oact 输出信号先前的处理方式（如果不为 NULL 的话）
// [return] 成功 0，失败 -1 并设置 errno
int sigaction(int sig, const struct sigaction* act, struct sigaction* oact);

struct sigaction {...}; // 参考 P181
```



#### 10.3 信号集

##### 信号集函数

```cpp
#include <bits/sigset.h>

# define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
tydedef struct {
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t; // 其实就是一个长整型数组
```



##### 进程信号掩码

```cpp
#include <signal.h>

// _set 参数指定新的信号掩码，_how 指定设置掩码方式 SIG_BLOCK|SIG_UNBLOCK|SIG_SETMASK
// _oset 参数输出原来的信号掩码（如果不为 NULL 的话）
// [return] 成功 0，失败 -1 并设置 errno
int sigprocmask(int _how, _const sigset_t* _set, sigset_t* _oset);
```



##### 被挂起的信号

**设置进程信号掩码后，被屏蔽的信号将不能被进程接收**。如果给进程发送一个被屏蔽的信号，则操作系统将该信号设置为进程的一个**被挂起的信号**。如果我们取消对被挂起信号的屏蔽，则它能立即被进程接收到。如下两数可以获得进程当前被挂起的信号集

```cpp
#include <signal.h>

// 获取进程当前被挂起的信号集
// [return] 成功 0，失败 -1 并设置 errno
int sigpending(sigset_t* set);
```



#### 10.4 统一事件源

信号是一种异步事件：信号处理两数和程序的主循环是两条不同的执行路线。很显然，信号处理函数需要尽可能快地执行完毕，以确保该信号不被屏蔽（前面提到过，为了避免一些竞态条件，信号在处理期间，系统不会再次触发它）太久。一种典型的解决方案是：

把信号的主要处理逻辑放到程序的主循环中，当信号处理函数被触发时，它只是简单地通知主循环程序接收到信号，并把信号值传递给主循环，主循环再根据接收到的信号值执行目标信号对应的逻辑代码。信号处理函数通常使用管道来将信号 “传递”给主循环：**信号处理函数往管道的写端写人信号值**，主循环则从管道的读端读出该信号值。**那么主循环怎么知道管道上何时有数据可读呢？**这很简单，我们只需要使用 I/O 复用系统调用来**监听管道的读端文件描述符**上的可读事件。如此一来，信号事件就能和其他 I/O 事件一样被处理，即统一事件源。

> 代码参考：[10-1unievent.cpp](ch10/10-1unievent.cpp)



#### 10.5 网络编程相关的信号

**SIGHUP**：书中展示了 xinetd 程序接收并处理 SIGHUP 信号的流程



**SIGPIPE**：往一个读端关闭的管道或 socket 连接中写数据会引发 SIGPIPE 信号，两种方式检测该信号：

- send 函数加入 MSG_NOSIGNAL 标志可以进制写操作触发 SIGPIPE 信号，否则就会失败并设置 errno，根据 errno 值来判断管道或者 socket 连接的读端是否已经关闭
- poll 系统调用在管道读端关闭时，写端文件描述符上的 POLLHUP 会被触发，socket 被对方关闭时，socket 上的 POLLRDHUP 事件会被触发



**SIGURG**：socket 接收到紧急数据时触发，两种方式检测带外数据：

- select 系统调用接收到带外数据返回时会向应用程序报告 socket 上的异常事件 exception_fds
- 检测 SIGURG 信号，设置该信号的处理函数



### Ch.11 定时器

