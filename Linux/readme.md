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
    sa_family_t sa_family;	// 常见的地址族有: AF_UNIX/AF_INET/AF_INET6
    char sa_data[14];	// 存放 socket 地址值，不同的地址族地址值长度不同
}
```

地址族（address family）和协议族（protocol family）一一对应，两者是一样的，经常混用

- AF_*: AF_UNIX / AF_INET / AF_INET6
- PF_*: PF_UNIX / PF_INET / PF_INET6

UNIX 本地域协议族和 TCP/IPv6 协议族的地址值长度远远超过 14 字节，因为引入了一个新的通用 socket 地址结构体 sockaddr_storage，具体参考 P72



##### 5.1.3 专用 socket 地址

上面的 sa_data 将 IP 地址和端口号混在一起，实际使用中经常使用 sockaddr_in 和 sockaddr_in6 结构体，分别针对于 AF_INET 和 AF_INET6 地址族

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
int inet_pton(int af, const char* src, void* dst);	// af 是地址族

// 逆向转换，len 可以为 INET_ADDRSTRLEN(16, IPv4) / INET6_ADDRSTRLEN(46, IPv6)
const char* inet_ntop(int af, const void* src, char* dst, socklen_t len); // 失败返回 NULL
```



#### 5.2 创建 socket

UNIX/Linux 一切皆文件的思想也囊括 socket，它是一个可读、可写、可控制、可关闭的文件描述符，创建的一个 socket 的系统调用为：

```cpp
#include <sys/types.h>
#include <sys/socket.h>

// domain: 协议族，包括 PF_INET、PF_INET6、PF_UNIX
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

守护进程（Daemon Process），也就是通常说的 Daemon 进程（精灵进程），是 Linux 中的后台服务进程。它是一个生存期较长的进程，通常独立于控制终端并且周 期性地执行某种任务或等待处理某些发生的事件。一般采用以 d 结尾的名字。守护进程特点如下：

- 生命周期很长，守护进程会在系统启动的时候被创建并一直运行直至系统被关闭

- 它在后台运行并且不拥有控制终端。没有控制终端确保了内核永远不会为守护进程自动生成任何控制信号以及终端相关的信号（如 SIGINT、SIGQUIT）

代码参考：[daemon.c](nowcoder/daemon.c)

如何让进程以「**守护进程**」的方式运行？参考代码清单 7-3，实际上提供如下的系统调用：

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
- 客户端：socket --> connect --> send --> recv

缺点：访问量过大时，服务器负载加大，客户端得到的响应变慢



##### 8.1.2 P2P 模型

点对点模式中主机既是服务端又是客户端，缺点是：用户之间传输的请求过多时，网络的负载加重

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



#### 8.6 有限状态机 :fire:

程序清单 8-3 展示了 HTTP 请求的读取和分析中主从状态机是如何处理 HTTP 请求字段的

```cpp
// 代码中有一个 string.h 文件里面的库函数
const char* str = "hello world, friend of mine!";
const char* sep = " ,!";

str = strpbrk(str, sep); // 找分隔符，str = " world, friend of mine!"
str += strspn(str, sep); // 跳过分隔符，str = "world, friend of mine!"
str = strchr(str, 'f');  // 找到第一个出现的字符，str = "friend of mine!"
```



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



### Ch.9 I/O 复用 :fire:

需要指出的是，I/O 复用虽然能同时监听多个文件描述符，但它本身是阻塞的。并且当多个文件描述符同时就绪时，如果不采取额外的措施，程序就只能按顺序依次处理其中的每一个文件描述符，这使得服务器程序看起来像是串行工作的。**如果要实现并发，只能使用多进程或多线程等编程手段。**

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

**select 成功时返回就绪文件描述符的总数，如果在超时时间内没有任何文件描述符就绪就返回 0，失败 -1 并设置 errno**，如果在 select 等待期间，程序收到信号，就立即返回 -1，并设置 errno 为 EINTR



**文件描述符可读就绪条件**

- socket 接收缓冲区中的字节数大于或等于低水位标记 SO_RCVLOWAT 时可以无阻塞地读 socket
- socket 通信的对方关闭连接时，对该 socket 的读操作返回 0
- **监听 socket** 上有新的连接（注意「监听 socket」和「通信 socket」是两个概念）
- socket 上有未处理的错误

**文件描述符可写就绪条件**

- socket 发送缓冲区中的可用字节数大于或等于低水位标记 SO_SNDLOWAT 时可以无阻塞地写 socket
- socket 的写操作被关闭，对该 socket 的执行写操作将触发一个 SIGPIPE 信号
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
    short events; // fd 上注册的事件，一系列 POLL 事件的按位或
    short revents; // 实际发生的事件，内核填充
}
```



#### 9.3 epoll 系列系统调用

> MacOSX 里没有 epoll 库，使用 kqueue 代替，[参考](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man2/kqueue.2.html)

epoll 是 Linux 特有的 I/O 复用函数，它把用户关心的文件描述符上的事件放入内核里的一个事件表中，无需像 select 和 poll 那样每次调用都要重复传入文件描述符集或事件集。epoll 需要一个额外的文件描述符来唯一标识内核中的这个事件表

```cpp
#include <sys/epoll.h>

// 创建标识内核中的事件表，size 参数并无实际作用
// [return] 返回的 fd 将作为其他所有 epoll 系统调用的第一个参数
int epoll_create(int size);

// 操作内核事件表 epfd
// op 参数指定操作类型，由 EPOLL_CTL_ADD|EPOLL_CTL_MOD|EPOLL_CTL_DEL 组成
// fd 参数是要操作的文件描述符
// event 参数指定 fd 的监听事件
// [return] 成功 0，失败 -1 并设置 errno
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

struct epoll_event {
    _uint32_t events; // epoll 事件，和 poll 类型基本一致
    epoll_data_t data; // 用户数据
}

// 联合体：不能同时使用多个成员
typedef union epoll_data {
    void* ptr;
    int fd; // 使用最多，表示监听事件所属的目标文件描述符，和 epoll_ctl 中的 fd 相同
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

// timeout 指定超时，maxevents 指定最多监听多少个事件
// [return] 成功时返回就绪的文件描述符的个数，失败时返回 -1 并设置 errno
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```

epoll 相比于 poll 有两个额外的事件类型：EPOLLET | EPOLLONESHOT

epoll_wait 将所有就绪的事件从内核事件表（由 epfd 参数指定）中复制到 events 指定的数组中，**只用于输出 epoll_wait 检测到的就绪事件**，不像 select 和 poll 的数组参数那样既用于传入用户注册的时间又用于输出内核检测到的就绪事件，所以 epoll_wait 极大地提升了性能



##### LT 和 ET 模式

- LT：Level Trigger，默认的，epoll_wait 检测到其上有事件发生并将此事件通知应用程序之后，应用程序可以不立即处理该事件，下次调用 epoll_wait 还可以再次向应用程序通告此事件
- ET：Edge Trigger，epoll_wait 检测到就绪事件之后必须处理，效率比 LT 模式要高，需要指定 EPOLLET 事件类型

> 代码参考：[LT vs ET 9-3mtlt.cpp](ch9/9-3mtlt.cpp)



**EPOLLONESHOT 事件**

即使在 ET 模式下，一个 socket 上的某个事件还是可能被触发多次，在并发程序中，一个线程读取完某个 socket 上的数据后开始处理这些数据，但是在处理过程中该 socket 上又有新数据可读（EPOLLIN 被再次触发），此时另一个线程被唤醒来读取这些新的数据。此时出现了两个线程同时操作一个 socket 的局面

为此需要 EPOLLONESHOT 事件，one shot 不是对某一个事件的，而是针对某一个socket，也就是文件描述符，如果设置了epolloneshot，那么只会触发一次。防止一个线程在处理业务时，然后来数据了，又从线程池里拿一个线程来处理新的业务，这样不就乱套了么。

> 代码参考：[9-4oneshot.cpp](ch9/9-4oneshot.cpp)



#### 9.4 三组 I/O 复用函数比较

selecet、poll 和 epoll 都通过某种结构体变量来告诉内核监听哪些文件描述符上的哪些事件，并使用该结构体类型的参数来获取内核处理的结果

- select 参数类型 fd_set 没有将文件描述符和事件绑定，因此需要 3 个类型的参数分别区分可读、可写和异常事件，不能处理更多类型的事件，且下次调用时需要重置 3 个 fd_set 集合
- poll 通过参数类型 pollfd 将文件描述符和事件都定义在其中，支持更多的事件类型，且下次调用 poll 时无需重置 pollfd 类型的事件集参数，因为内核修改的仅仅是 revents 成员
- select 和 poll 调用返回整个用户注册的事件集合（包括就绪和未就绪的），通过**「轮询扫描」**索引就绪文件描述符的时间复杂度为 O(n)，epoll 通过 epoll_wait 直接从 epollfd 指定的内核事件表中取得用户注册的事件，内核检测到就绪的文件描述符将触发**「回调函数」**将该文件描述符上对应的事件插入「内核就绪队列」，且通过 events 参数仅仅用来返回就绪的事件，索引就绪的 fd 事件复杂度为 O(1)
- poll 和 epoll_wait 分别使用 nfds 和 maxevents 参数指定最多监听的 fd 和 事件，最大 65535，但是 select 一般是 1024
- poll 和 select 只能工作在相对低效的 LT 模式，epoll 可以在 ET 模式，且还支持 EPOLLONESHOT 事件

> 具体区别参考表格 9-2 



#### 9.5 I/O 复用的高级应用一：非阻塞 connect

connect 出错时有一个 errno 值：EINPROGRESS，**这种错误发生在非阻塞的 socket 调用 connect，而连接又没有立即建立时**。根据 man 文档的解释，在这种情况下，我们可以调用 select、poll 等函数来监听这个连接失败的 socket 上的**可写事件**。当select、poll 等函数返回后，再利用 getsockopt 来读取错误码并清除该 socket 上的错误。如果错误码是0，表示连接成功建立，否则连接失败。

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
// pid > 0 : 将信号发送给指定的进程
// pid = 0 : 将信号发送给当前的进程组
// pid = -1 : 将信号发送给每一个有权限接收这个信号的进程
// pid < -1 : 这个pid=某个进程组的ID取反 （-12345）
int kill(pid_t pid, int sig); // 成功返回 0，失败 -1 并设置 errno

// 信号处理函数原型
typedef void (*__sighandler_t)(int);
```

linux 信号有很多，和网络编程关系紧密的是：

- SIGHUP：控制终端挂起
- SIGPIPE：往读端被关闭的管道或者 socket 连接中写数据
- SIGURG：socket 连接上接受到紧急数据
- SIGALRM：由 [alarm.c](nowcoder/2.21-signal-alarm/alarm.c) 或 [setitimer.c](nowcoder/2.21-signal-alarm/setitimer.c) 设置的实时闹钟超时引起
- SIGCHLD：子进程状态发生变化（退出或暂停）



#### 10.2 信号函数

##### signal 系统调用

```cpp
#include <signal.h>

// sig 参数指出要捕获的信号类型，
// _handler 参数是函数指针，用于指定信号 sig 的处理函数
//	- SIG_IGN ： 忽略信号
// 	- SIG_DFL ： 使用信号默认的行为
//	- 回调函数 :  这个函数是内核调用，程序员只负责写，捕捉到信号后如何去处理信号
// [return] 成功，返回上一次注册的信号处理函数的地址。第一次调用返回 NULL
//          失败，返回 SIG_ERR，设置错误号
_sighandler_t signal(int sig, _sighandler_t handler);
```

回调函数：

- 需要程序员实现，提前准备好的，函数的类型根据实际需求，看函数指针的定义
- 不是程序员调用，而是当信号产生，由内核调用
- 函数指针是实现回调的手段，函数实现之后，将函数名放到函数指针的位置就可以了

代码参考：[signal.c](nowcoder/2.21-signal-alarm/signal.c)



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

代码参考：[sigaction.c](nowcoder/2.21-signal-alarm/sigaction.c)



#### 10.3 信号集

在 PCB 中有两个非常重要的信号集。一个称之为 「**阻塞信号集**」，另一个称之为 「**未决信号集**」 。这两个信号集都是内核使用位图机制来实现的。但操作系统不允许我 们直接对这两个信号集进行位操作。而需自定义另外一个集合，借助信号集操作函数 来对 PCB 中的这两个信号集进行修改。

- 信号的 “未决” 是一种状态，指的是从信号的产生到信号被处理前的这一段时间，未决信号集不能修改只能获取
- 信号的 “阻塞” 是一个开关动作，指的是阻止信号被处理，但不是阻止信号产生，阻塞信号集可以修改

##### 信号集函数

```cpp
#include <sigset.h>

# define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
tydedef struct {
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t; // 其实就是一个长整型数组

// 对「自定义」的信号集进行操作
int sigemptyset(sigset_t *set); // 清空信号集数据
int sigfillset(sigset_t *set);  // 将信号集中所有标志位置为 1
int sigaddset(sigset_t *set, int signum); // 设置信号集中的某一个信号对应的标志位为1，表示阻塞这个信号
int sigdelset(sigset_t *set, int signum); // 设置信号集中的某一个信号对应的标志位为0，表示不阻塞这个信号
int sigismember(const sigset_t *set, int signum); // 判断某个信号是否阻塞
```

代码参考：[sigset.c](nowcoder/2.24-sigset/sigset.c)



##### 进程信号掩码（阻塞信号集）

sigprocmask 可以将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）

```cpp
#include <signal.h>

// _set 参数指定新的信号掩码，
// _how 指定设置掩码方式，假设内核中默认的阻塞信号集是mask
//	- SIG_BLOCK: 将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变，mask | set
//	- SIG_UNBLOCK: 根据用户设置的数据，对内核中的数据进行解除阻塞，mask &= ~set
//	- SIG_SETMASK: 覆盖内核中原来的值
// _oset 参数输出原来的信号掩码（如果不为 NULL 的话）
// [return] 成功 0，失败 -1 并设置 errno
int sigprocmask(int _how, _const sigset_t* _set, sigset_t* _oset);
```

代码参考：[sigprocmask.c](nowcoder/2.24-sigset/sigprocmask.c)



##### 被挂起的信号（未决信号集）

**设置进程信号掩码后，被屏蔽的信号将不能被进程接收**。如果给进程发送一个被屏蔽的信号，则操作系统将该信号设置为进程的一个**被挂起的信号**。如果我们取消对被挂起信号的屏蔽，则它能立即被进程接收到。如下两数可以获得进程当前被挂起的信号集

```cpp
#include <signal.h>

// 获取进程当前被挂起的信号集（未决信号集）
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

#### 11.1 socket 选项 SO_RCVTIMEO/SO_SNDTIMEO

这两个参数分别表示 socket 接受数据超时时间和发送数据超时时间，这个选项只针对 send, sendmsg, recv, recvmsg, accept, connect 有效

> 代码参考：[11-1connect_timeout.cpp](ch11/11-1connect_timeout.cpp)



#### 11.2 SIGALRM 信号

第10章提到，由alarm 和 setitimer 函数设置的实时闹钟一旦超时，将触发 SIGALRM 信号。因此可以利用该信号的**信号处理函数来处理定时任务**。但是，如果要处理我个定时任务，我们就需要不断地触发 SIGALRM 信号，并在其信号处理函数中执行到期的任务。一般而言，SIGALRM 信号按照固定的频率生成，即由 alarm 或 setitimer 函数设置的定时周期 T 保持不变。如果某个定时任务的超时时间不是 T 的整数倍，那么它实际被执行的时间和预期的时间将略有偏差。



##### 11.2.1 基于升序链表的定时器

其核心函数 tick 相当于一个心搏函数，它每隔一段固定的时间就执行一次，以检测并处理到期的任务。判断定时任务到期的依据是定时器的 expire 值小于当前的系统时间。从执行效率来看，添加定时器的时向复杂度是 O(n)，删除定时器的时间复杂度是 O(1)，执行定时任务的时间复杂度是 O(1)

> 代码参考：[11-2lst_timer.h](ch11/11-2lst_timer.h)



##### 11.2.2 处理非活动连接

服务器程序通常要定期处理非活动连接：给客户端发一个重连请求，或者关闭该连接，或者其他。Linux 在内核中提供了对连接是否处于活动状态的定期检查机制，我们可以通过 socket 选项 KEEPALIVE 来激活它。不过使用这种方式将使得应用程序对连接的管理变得复杂。因此，我们可以考虑在应用层实现类似于 KEEPALIVE 的机制，以管理所有长时间处于非活动状态的连接。代码清单 11-3 利用alarm 函数周期性地触发 SIGALRM 信号，该信号的信号处理函数利用管道通知主循环执行定时器链表上的定时任务——关闭非活动的连接。

> 代码参考：[11-3nonactive_conn.cpp](ch11/11-3nonactive_conn.cpp)



#### 11.3 I/O 复用系统调用的超时参数

Linux 下的 3 组 I/O 复用系统调用都带有超时参数，因此它们不仅能统一处理信号和 I/O 事件，也能统一处理定时事件。但是由于 I/O 复用系统调用可能在超时时间到期之前就返回（有 I/O 事件发生），所以如果我们要利用它们来定时，就需要不断更新定时参数以反映剩余的时间

> 代码参考：[11-4io_timer.cpp](ch11/11-4io_timer.cpp)



#### 11.4 高性能定时器

**时间轮**

前面的基于排序链表的定时器添加定时器的效率偏低，为此考虑更高效的时间轮

基于排序链表的定时器使用唯一的一条链表来管理所有定时器，所以插人操作的效率随着定时器数目的增多而降低。而**时间轮使用哈希表的思想，将定时器散列到不同的链表上**。这样每条链表上的定时器数目都将明显少于原来的排序链表上的定时器数目，插人操作的效率基本不受定时器数目的影响。

> 代码参考：[11-5tw_timer.h](ch11/11-5tw_timer.h)



**时间堆**

前面讨论的定时方案都是以固定的频率调用心搏函数 tick，并在其中依次检测到期的定时器，然后执行到期定时器上的回调函数。设计定时器的另外一种思路是：**将所有定时器中超时时间最小的一个定时器的超时值作为心搏间隔**。这样，一旦心搏函数 tick 被调用，超时时间最小的定时器必然到期，我们就可以在 tick 函数中处理该定时器。然后，再次从剩余的定时器中找出超时时间最小的一个，并将这段最小时间设置为下一次心搏间隔。如此反复，就实现了较为精确的定时。

> 代码参考：[11-6time_heap.h](ch11/11-6time_heap.h) 没太看懂



### Ch.12 高性能 I/O 框架库 Libevent

#### 12.1 I/O 框架库概述

- 句柄：I/O 框架库要处理的对象，即 I/O 事件、信号和定时事件
- 事件多路分发器：I/O 框架库一般将系统支持的各种 I/O 复用系统调用封装成统一的接口
- 事件处理器：包含一个或多个 handle_event 回调函数
- 具体事件处理器：继承事件处理器的接口实现自己的事件处理器



#### 12.2 Libevent 源码分析

TODO 略



### Ch.13 多进程编程

#### 13.1 fork 系统调用

```cpp
#include <sys/types.h>
#include <unistd.h>

// 返回两次，父进程返回的子进程的 PID，子进程中返回 0，失败 -1
pid_t fork(void );
```

fork 函数复制当前进程，在内核进程表中创建一个新的进程表项。新的进程表项有很多属性和原进程相同，比如**堆指针、栈指针和标志寄存器的值**。但也有许多属性被赋予了新的值，比如该进程的 PPID 被设置成原进程的 PID，信号位图被清除（原进程设置的信号处理函数不再对新进程起作用)，fork() 之后的父子进程共享文件，此时的 fork() 产生的子进程与父进程相同的文件描述符指向相同的文件表，引用计数增加，共享文件偏移指针

**父子进程虚拟地址空间**

Linux 的 fork() 使用是通过**写时拷贝（copy-on-write）**实现。写时拷贝是一种**可以推迟甚至避免拷贝数据的技术**。内核并不复制整个进程的地址空间，而是让父子进程共享同一个地址空间。只有在写入时才会复制地址空间（重新开辟一块内存），从而使各个进程拥有自己的地址空间。即资源的复制只有在写入时才会进行，在此之前，只有以只读的方式进行，即所谓的**「读时共享，写时拷贝」**

**GDB 多进程调试**

gdb 默认只能跟踪一个进程，[视频](https://www.nowcoder.com/study/live/504/2/5)

```bash
# 设置调试父进程或者子进程
set follow-fork-mode [parent(默认)|child]

# on 表示调试当前进程时其他进程继续运行，off 表示调试当前进程时其他进程被 GDB 挂起
set detach-on-fork [on(默认)|off]

# 查看调试的进程
info inferiors

# 切换当前调试的进程
inferior id

# 使进程脱离 GDB 调试
detach inferiors id
```



#### 13.2 exec 系列系统调用

需要在子进程种执行其他程序，即替换当前进程映像，就需要使用 exec 系列函数 [参考](https://blog.csdn.net/u014530704/article/details/73848573)

```cpp
#include <unistd.h>
extern char** environ;

// path 参数指定可执行文件的完整路径
// file 参数接受文件名
// avg 接受可变参数，argv 接受参数数组
// envp 设置新程序的环境变量

// 传可变参数list，NULL 结尾，execlp("ps", "ps", "-l", NULL)
int execl(const char* path, const char* arg, ...); 
int execlp(const char* file, const char* arg, ...);
int execle(const char* path, const char* arg, ..., char* const envp[]);

// 传参数的指针数组，char *argv[] = {"ps","-l",NULL};
int execv(const char* path, const char* argv[]); 
int execvp(const char* file, const char* argv[]);
int execvpe(const char* path, const char* argv[], char* const envp[]);

// l(list) 参数地址列表，以空指针结尾
// v(vector) 存有各参数地址的指针数组的地址
// p(path) 按 PATH 环境变量指定的目录搜索可执行文件
// e(environment) 存有环境变量字符串地址的指针数组的地址
```

一般情况下，exec 函数是不返回的，除非出错。它出错时返回-1，并设置 errno。 如果没出错，则原程序中 exec 调用之后的代码都不会执行，因为此时原程序己经被 exec 的参数指定的程序完全替换（包括代码和数据）。**exec 函数不会关闭原程序打开的文件描述符**，除非该文件描述符被设置了类似 SOCK_CLOEXEC 的属性（见5.2节）。



#### 13.3 处理僵尸进程

每个进程结束之后都会释放自己地址空间中的用户区数据，内核区的 PCB 没办自己释放掉（pid ppid 等），需要父进程释放

- 在子进程结束运行之后，父进程尚未回收，子进程残留资源(PCB)存放于内核中，即父进程读取其退出状态之前，该子进程就是**僵尸进程**
- 父进程结束或者异常终止，而子进程继续运行（**孤儿进程**）。此时子进程的 PPID 将被操作系统设置为 1，即 **init 进程**。init 进程接管了该子进程，并等待它结束。

通过下面的系统调用，在父进程中调用等待子进程的结束，并获取子进程的返回信息，从而避免僵尸进程的产生，或者使子进程的僵尸状态立即结束

```cpp
#include <sys/types.h>
#include <sys/wait.h>

// 阻塞，pid_t 是结束子进程的进程 id，stat_loc 可以保留相关信息，通过相关的宏函数可以查看
pid_t wait(int* stat_loc);

// 只等待 pid 指定的子进程，为 -1 时与 wait 一样
// options 指定 WNOHANG 参数可以非阻塞：如果 pid 子进程还未结束或者意外终止直接返回 0
pid_t waitpid(pid_t pid, int* stat_loc, int options);
```

要在事件已经发生的情况下执行非阻塞调用才能提高程序的效率。对 waitpid 函数而言，我们最好在某个子进程退出之后再调用它。那么父进程从何得知某个子进程已经退出了呢？这正是 SIGCHLD 信号的用途。**当一个进程结束时，它将给其父进程发送一个SIGCHLD 信号**。因此，我们可以在父进程中捕获SIGCHLD 信号，并在信号处理函数中调用 waitpid 函数以“彻底结束”一个子进程



**进程退出**

```cpp
#include <stdlib.h>
void exit(int status); // 标准 C 库函数，会刷新缓冲区

#include <unistd.h>
void _exit(int status); // Linux 系统调用，不会刷新缓冲区
```



#### 13.4 管道

**匿名管道(pipe)** 能在父子进程间传递数据，一般来说是单向的，只能保证父子进程之间一个方向的数据，父子进程必须有一个关闭 fd[0]、另一个关闭 fd[1]，如果要实现父子进程之间的双向数据传输就必须使用两个管道。

> 书中描述了 squid 如何使用 socketpair 系统调用创建一个全双工管道的

匿名管道默认是阻塞的：如果管道中没有数据，read 阻塞，如果管道满了，write 阻塞



**有名管道(FIFO)** 不同于匿名管道之处在于它提供了一个路径名与之关联，以 FIFO 的文件形式存在于文件系统中，并且其打开方式与打开一个普通文件是一样的，这样 即使与 FIFO 的创建进程不存在亲缘关系的进程，只要可以访问该路径，就能够彼此 通过 FIFO 相互通信，因此，通过 FIFO 不相关的进程也能交换数据。



有名管道(FIFO) 和 匿名管道(pipe) 有一些特点是相同的，例如默认的阻塞行为：

```bash
# 默认 FIFO 和 PIPE 都是阻塞的，读写端关闭或者打开时对方的行为是类似的：
读管道：
    管道中有数据，read返回实际读到的字节数
    管道中无数据：
        写端被全部关闭，read返回0（相当于读到文件的末尾）
        写端没有完全关闭，read阻塞等待

写管道：
    管道读端全部被关闭，进程异常终止（进程收到SIGPIPE信号）
    管道读端没有全部关闭：
        管道已满，write阻塞
        管道没有满，write将数据写入，并返回实际写入的字节数
```

不一样的地方在于：

- FIFO 在文件系统中作为一个特殊文件存在，但 FIFO 中的内容却存放在内存中
- 当使用 FIFO 的进程退出后，FIFO 文件将继续保存在文件系统中以便以后使用
- FIFO 有名字，不相关的进程可以通过打开有名管道进行通信









#### 13.5 信号量

**信号量原语**

关键代码段/临界区代码会引发进程之间的竞态条件，进程同步需要确保任一时刻只有一个进程能进入关键代码段。Dekker 算法和 Peterson 算法通过忙等待解决同步问题，CPU 利用率低；Dijkstra 提出的信号量（Semaphore）通过 P、V 操作实现。

信号量取值可以是任何自然数，最常用的 0 1 是Mutex，Linux 相关的系统调用是 semget、semop 和 semctl，内核中与信号量相关联的数据结构是 semid_ds

**semget** 

semget 系统调用创建一个新的信号量集（会修改内核数据结构体 semid_ds），或者获取一个已经存在的信号量集

```cpp
#include <sys/sem.h>

// key 参数标识一个全局唯一的信号量集合
// num_sems 指定要创建/获取的信号量集中信号量的数目，0 标识获取已经存在的信号量
// sem_flags 参数指定一组标志
// [return] 成功返回一个正整数表示信号量集的标识符，失败 -1 并设置 errno
int semget(ket_t key, int num_sems, int sem_flags);
```

key 可以传递一个特殊的键值 IPC_PRIVATE（值为0），这样无论信号量是否已经存在，semget 都将创建一个新的信号量。所有其他进程都可以使用这个新创建的信号量

**semop** 

```cpp
#include <sys/sem.h>

// sem_id 是 semget 返回的信号量集标识符
// sem_ops 指向一个 sembuf 结构体类型的数组
// num_sem_ops 指定要执行的操作个数
int semop(int sem_id, struct sembuf* sem_ops, size_t num_sem_ops);
```

这个 sembuf 结构体中 sem_flg 和 sem_op 关系有点复杂，没看懂o(╥﹏╥)o

**semctl**

```cpp
#include <sys/sem.h>

// sem_id 是 semget 返回的信号量集标识符
// sem_num 指定被操作的信号量在信号量集中的编号
// command 指定要执行的命令
int semctl(int sem_id, int sem_num, int command, ...);
```

> 代码参考：[13-3sem.cpp](ch13/13-3sem.cpp)



#### 13.6 共享内存

共享内存是最高效的 IPC 机制，他不涉及进程之间的任何数据传输，内核中与共享内存相关联的数据结构是 shmid_ds

**shmget**

和 semget 一样，shmget 系统调用创建一段新的共享内存或者获取一段已经存在的共享内存

```cpp
#include <sys/shm.h>

// key 表示一段全局唯一的共享内存，非零值，一般是 16 进制
// size 指定共享内存的大小，单位 byte，为 0 表示获取已经存在的共享内存
// shmflg 和 sem_flags 类似
// [return] 成功返回一个正整数标识共享内存的标识符，失败 -1 并设置 errno
int shmget(key_t key, size_t size, int shmflg);
```

**shmat 和 shmdt**

共享内存创建之后需要先将其关联到进程的地址空间才能使用

```cpp
// shm_id 是由 shmget 返回的共享内存标识符
// shm_addr 指定共享内存关联到进程的那块地址空间，为 NULL 就由内核指定
// shmflg 是一些标志 SHM_RND|SHM_RDONLY...
// [return] 成功时返回共享内存被关联到的地址，失败返回 -1 并设置 errno
void* shmat(int shm_id, const void* shm_addr, int shmflg);
```

使用完共享内存之后还需要将它从进程地址空间分离

```cpp
// 将关联到的 shm_addr 处的共享内存从进程中分离，失败 -1 并设置 errno
int shmdt(const void* shm_addr);
```

**shmctl**

```cpp
#include <sys/shm.d>

// shm_id 是 shmget 返回的共享内存标识符
// command 指定要执行的命令
// [return] 成功返回值取决于 command，失败 -1 并设置 errno
int shmctl(int shm_id, int command, struct shmid_ds* buf);
```

代码参考：[write_shm.c](nowcoder/2.28-shm/write_shm.c) 与 [read_shm.c](nowcoder/2.28-shm/read_shm.c) 进程通信传数据



**共享内存的POSIX方法**

mmap 函数利用它的 MAP ANONYMOUS 标志我们可以实现父、子进程之间的匿名内存共享。通过打开同一个文件，mmap 也可以实现无关进程之间的内存共享。Linux 提供了另外一种利用mmap 在无关进程之间共享内存的方式。这种方式无需任何文件的支特，但它需要先使用如下函数来创建或打开一个 POSIX 共享内存对象：

```cpp
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

// 与 open 系统调用完全相同，shm_open 成功时返回一个文件描述符，失败 -1 并设置 errno
int shm_open(const char* name, int oflag, mode_t mode);

// shm_open 创建的共享内存对象使用完之后需要删除
int shm_unlink(const char* name);
```

这里实现多进程的聊天服务器：一个子进程处理一个客户连接，所有客户 socket 连接的读缓冲设置为一块共享内存，实现”共享读“，每个子进程在使用共享内存时无需加锁

> 代码参考：[13-4shm_talk_server.cpp](ch13/13-4shm_talk_server.cpp)



#### 13.7 消息队列

消息队列是在两个进程之间传递二进制块数据的一种简单有效的方式。每个数据块都有一个特定的类型，**接收方可以根据类型来有选择地接收数据**，而不一定像管道和命名管道那样必须以先进先出的方式接收数据。

同样的有 4 个系统调用：

```cpp
#include <sys/msg.h>

// 创建一个消息队列，或者获取一个已有的消息队列
int msgget(key_t key, int msgflg);

// 将一条消息 msg_ptr 添加到消息队列
int msgsnd(int msqid, const void* msg_ptr, size_t msg_sz, int msgflg);

// 从消息队列中获取消息 msg_ptr
int msgrcv(int msqid, void* msg_ptr, size_t msg_sz, long int msgtype, int msgflg);

// 控制消息队列的某些属性
int msgctl(int msqid, int command, struct msqid_ds* buf);
```



#### 13.8 IPC 命令

ipcs 命令可以显示 Linux 系统拥有的共享内存、信号量和消息队列资源使用情况



#### 13.9 在进程间传递文件描述符

由于 fork 调用之后，父进程中打开的文件描述符在子进程中仍然保持打开，所以文件描述符可以很方便地从父进程传递到子进程。需要注意的是，传递一个文件描述符并不是传递一个文件描述符的值，**而是要在接收进程中创建一个新的文件描述符，并且该文件描述符和发送进程中被传递的文件描述符指向内核中相同的文件表项。**

那么如何把子进程中打开的文件描述符传递给父进程呢？或者更通俗地说，如何在两个不相于的进程之间传递文件描述符呢？在 Linux 下，我们可以利用 UNIX 域 socket 在进程间传递特殊的辅助数据，以实现文件描述符的传递。代码清单 13-5 给出了一个实例，它在子进程中打开一个文件描述符，然后将它传递给父进程，父进程则通过读取该文件描述符来获得文件的内容。

> 代码参考：[13-5passfd.cpp](ch13/13-5passfd.cpp)



### Ch.14 多线程编程

#### 14.1 Linux  线程概述

**线程模型**

- 内核线程：运行在内核空间，内核来调度，数量 M
- 用户线程：运行在用户空间，线程库来调度，数量 N

内核线程相当于用户线程的容器，M <= N，根据 M:N 取值将线程实现方式分为三种模式：完全在用户空间实现、完全由内核调度和双层调度

**Linux 线程库**：LinuxThreads 和 NPTL 线程库



#### 14.2 创建线程和结束线程

```cpp
#include <pthread.h>

// pthread_t 是 unsigned long int 类型
// attr 设置新线程的属性
// start_routine 是函数指针，就是线程运行的函数，arg 是其参数
// [return] 成功返回 0，失败返回错误码
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void *), void* arg);

// 通过 retval 参数向线程的回收者传递其退出信息
void pthread_exit(void* retval);

// thread 是目标线程的标识符
// retval 是目标线程返回的退出信息
// [return] 成功返回 0，失败返回错误码
int pthread_join(pthread_t thread, void** retval); // 会一直阻塞，知道被回收的线程结束

// thread 是目标线程的标识符
int pthread_cancel(pthread_t thread);
```

代码参考：[3.2-pthread](nowcoder/3.2-pthread)



#### 14.3 线程属性

pthread_attr_t 结构体定义了一套完整的线程属性

```cpp
#include <bits/pthreadtypes.h>

typedef union {
    char __size[__SIZEOF_PTHREAD_ATTR_T];
    long int __align;
} pthread_attr_t;

// 初始化线程属性对象
int pthread_attr_init(pthread_attr_t* attr);

// 销毁线程属性对象，被销毁的线程属性对象只有再次初始化之后才能继续使用
int pthread_attr_destroy(pthread_attr_t* attr);

// 获取和设置线程属性对象的某个属性很熟有很多...
```



#### 14.4 POSIX 信号量 :three:

POSIX 信号量函数都以 sem_ 开头

```cpp
#include <semaphore.h>

// 无名信号量
int sem_init(sem_t* sem, int pshared, unsigned int value);
int sem_destroy(sem_t* sem);
int sem_wait(sem_t* sem); // 信号量减1
int sem_trywait(sem_t* sem); // wait 的非阻塞版本
int sem_post(sem_t* sem); // 信号量加1

// 有名信号量
sem_t *sem_open(const char *name, int oflag);  
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
```

在选择使用有名信号量还是无名信号量时，需要根据具体的应用需求和场景进行考虑。如果需要在不同进程之间进行信号量共享或需要持久性的信号量，可以选择有名信号量。如果只需要在同一进程内的线程之间进行信号量共享，可以选择无名信号量。具体区别如下： [参考](https://blog.csdn.net/u014426028/article/details/105628740)

1. 命名和标识符：
   - 有名信号量具有一个唯一的标识符（名称），可以通过该标识符在不同进程之间进行共享。在创建有名信号量时，需要提供一个唯一的名称。
   - 无名信号量没有名称，仅存在于创建它的进程中，无法在进程之间共享。
2. 共享：
   - 有名信号量可以在不同进程之间进行共享，允许多个进程访问同一个有名信号量，以实现进程间的同步和互斥。
   - 无名信号量只能在同一进程内的线程之间进行共享，不能跨进程使用。
3. 持久性：
   - 有名信号量在系统中是持久性的，即创建它的进程终止后仍然存在。其他进程可以通过标识符（名称）访问和使用该信号量。
   - 无名信号量是进程私有的，当创建它的进程终止时会自动被释放和销毁。
4. 访问控制：
   - 有名信号量可以通过权限控制机制（如文件系统权限）进行访问控制，可以限制哪些进程可以打开、读取和修改该信号量。
   - 无名信号量没有访问控制机制，仅能在创建它的进程内部使用。

代码参考：[nowcoder-semaphore.c](nowcoder/3.8-thread-sync/semaphore.c)



#### 14.5 互斥锁 :one:

**基础 API**

互斥锁（也称互斥量）可以用于保护关键代码段，以确保其独占式的访向，这有点像一个二进制信号量（见 13.5.1 小节）。当进入关键代码段时，我们需要获得互斥锁并将其加锁。这等价于二进制信号量的P操作：当离开关键代码段时，我们需要对互斥锁解锁，以唤醒其他等待该互斥锁的线程，这等价于二进制信号量的V操作。

```cpp
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexatrr_t* mutexattr);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_mutex_lock(pthread_mutex_t* mutex); // 互斥锁加锁
int pthread_mutex_trylock(pthread_mutex_t* mutex); // lock 的非阻塞版本
int pthread_mutex_unlock(pthread_mutex_t* mutex); // 互斥锁解锁
```

**互斥锁属性**

pthread_mutexattr_t 结构体定义了一套完整的互斥锁属性

```cpp
#include <pthread.h>
/*初始化互斥锁属性对象*/
int pthread_mutexattr_init(pthread_mutexattr_t* attr);

/*销毁互斥锁属性对象*/
int pthread_mutexattr_destroy(pthread_mutexattr_t* attr);

/*获取和设置互斥锁的 pshared 属性*/
int pthread_mutexattr_getpshared(const pthread_mutexattr_t* attr, int* pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t* attr, int pshared);

/*获取和设置互斥锁的 type 属性*/
int pthread_mutexattr_gettype(const pthread_mutexattr_t* attr, int* type);
int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type);
```

- pshared 参数指定是否允许跨进程共享互斥锁
- type 参数指定互斥锁类型：普通锁、检错锁、嵌套锁、默认锁

**死锁举例**

主线程获取 mutex_a 之后等待 mutex_b，子线程获取 mutex_b 之后等待 mutex_a

> 代码参考：[14-1mutual_lock.c](ch14/14-1mutual_lock.c)



#### 14.6 条件变量 :two:

条件变量用于在线程之间同步共享数据的值，需要配合互斥锁使用

```cpp
#include <pthread.h>

int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* cond_attr);
int pthread_cond_destroy(pthread_cond_t* cond);

int pthread_cond_broadcast(pthread_cond_t* cond); // 广播方式唤醒所有等待目标条件变量的线程
int pthread_cond_signal(pthread_cond_t* cond); // 唤醒一个等待目标条件变量的线程

// 当这个函数调用阻塞的时候，会对互斥锁进行解锁，当不阻塞的，继续向下执行，会重新加锁
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
```

代码参考：[nowcoder-cond.c](nowcoder/3.8-thread-sync/cond.c)



#### 14.7 线程同步机制包装类

将 sem、mutex 和 cond 封装成类

> 代码参考：[14-2locker.h](ch14/14-2locker.h)



#### 14.8 多线程环境

**可重入函数**

一个函数能被多个线程同时调用且不发生竞态条件，那就是线程安全的，该函数就是可重入函数，Linux 库函数中 inet_ntoa、getservbyname 等函数是不可重入的，主要是因为它们内部使用了静态变量。

不过 Linux 对很多不可重入的库函数提供了对应的可重入版本，原函数尾部加上 _r，多线程编程中一定要使用可重入版本



**线程和进程**

思考这样一个问题：如果一个多线程程序的某个线程调用了 fork 函数，那么新创建的子进程是否将自动创建和父进程相同数量的线程呢？答案是 “否”，

正如我们期望的那样。**子进程只拥有一个执行线程，它是调用 fork 的那个线程的完整复制。并且子进程将自动继承父进程中互斥锁（条件变量与之类似）的状态。**也就是说，父进程中已经被加锁的互斥锁在子进程中也是被锁住的。

这就引起了一个问题：子进程可能不清楚从父进程继承而来的互斥锁的具体状态（是加锁状态还是解锁状态）。这个互斥锁可能被加锁了，但并不是由调用 fork 函数的那个线程锁住的，而是由其他线程锁住的。如果是这种情况，则子进程若再次对该互斥锁执行加锁操作就会导致死锁，如代码清单 14-3 所示。

> 代码参考：[14-3thread_atfork.c](ch14/14-3thread_atfork.c)



**线程与信号**

多线程版本下的信号掩码设置函数为

```cpp
#include <pthread.h>
#include <signal.h>

int pthread_sigmask(int how, const sigset_t* newmask, sigset_t* oldmask);
```

由于进程中的所有线程共享该进程的信号，所以线程库将根据**线程掩码**决定把信号发送给哪个具体的线程。因此，如果我们在每个子线程中都单独设登信号掩码，就很容易导致逻辑错误。此外，**所有线程共享信号处理函数**。也就是说，当我们在一个线程中设置了某个信号的信号处理函数后，它将覆盖其他线程为同一个信号设置的信号处理函数。这两点都说明，我们应该定义一个专门的线程来处理所有的信号。这可以通过如下两个步骤来实现：

1. 在主线程创建出其他子线程之前就调用 pthread_sigmask 来设置好信号掩码，所有新创建的子线程都将自动继承这个信号掩码。这样做之后，实际上所有线程都不会响应被屏蔽的信号了

2. 在某个线程中调用如下函数来等待信号并处理之：

   ```cpp
   #include <signal.h>
   
   // set 参数指定需要等待的信号集合
   // sig 存储返回值
   int sigwait(const sigset_t* set, int* sig);
   
   // 明确讲一个信号 sig 发给 thread 线程，sig = 0 不发送信号
   // thread 参数指定目标线程，sig 指定待发送的信号
   // 可以用于检查指定线程是否存在，成功返回 0，失败返回错误码
   int pthread_kill(pthread_t thread, int sig);
   ```

> 代码参考：[14-5sigmask.c](ch14/14-5sigmask.c) 没看懂o(╥﹏╥)o





### Ch.15 进程池和线程池

#### 15.1 概述

进程池中的所有子进程都运行着相同的代码，并具有相同的属性，比如优先级、PGID等。因为进程池在服务器启动之初就创建好了，所以每个子进程都相对 “干净”，即它们没有打开不必要的文件描述符（从父进程继承而来），也不会错误地使用大块的堆内存（从父进程复制得到）。

**当有新的任务到来时，主进程将通过某种方式选择进程池中的某一个子进程来为之服务**。相比于动态创建子进程，选择一个已经存在的子进程的代价显然要小得多。至于主进程选择哪个子进程来为新任务服务，则有两种方式：

- 随机算法和 Round Robin 算法、以及更优秀的均匀分配算法
- 主进程和所有子进程通过一个共享的工作队列来同步，子进程都睡眠在该队列上，有新任务时将任务添加到工作队列，唤醒正在等待的子进程

选择好子进程之后，可以通过管道实现父子进程之间的数据传递



#### 15.2 处理多客户

- 半同步/半反应堆：主进程接受新的连接以得到连接 socket，然后它需要将该 socket 传递给子进程（对于线程池而言，父线程将socket 传递给子线程必须使用13.9节介绍的 socketpair 系统调用创建的双向管道实现）
- 半同步/半异步模式以及领导者/追随者模式，是由主进程管理所有监听 socket，而各个子进程分别管理属于自己的连接 socket 的，子进程自己调用 accept 来接受新的连接



#### 15.3 半同步/半异步进程池

为了避免在父子进程之间传递文件描述符，我们将接收新连接的操作放到子进程中

> 代码参考：[15-1processpool.h](ch15/15-1processpool.h)



#### 15.4 用进程池实现简单的 CGI 服务器

复用前面的进程池，构建一个并发的 CGI 服务器

> 代码参考：[15-2pool_cgi.cpp](ch15/15-2pool_cgi.cpp)



#### 15.5 半同步/半反应堆线程池

本节我们实现一个基于图 8-10 所示的半同步/半反应堆并发模式的线程池，如代码清单15-3 所示。相比代码清单 15-1 所示的进程池实现，该线程池的通用性要高得多，因为它使用一个工作队列完全解除了主线程和工作线程的耦合关系：**主线程往工作队列中插入任务，工作线程通过竞争来取得任务并执行它**。不过，如果要将该线程池应用到实际服务器程序中，那么我们必须保证所有客户请求都是无状态的，因为同一个连接上的不同请求可能会由不同的线程处理。

> 代码参考：[15-3threadpool.h](ch15/15-3threadpool.h)



#### 15.6 用线程池实现的简单 web 服务器

- [15-4http_conn.h](ch15/15-4http_conn.h)
- [15-5http_conn.cpp](ch15/15-5http_conn.cpp)
- [15-6main.cpp](ch15/15-6main.cpp)

类似于一个传输文本服务器，简单测试的客户端可以使用 telnet

```bash
# HTTP 请求格式
GET /test.txt HTTP/1.1
Connection: keep-alive
Content-Length: 8
Host: telnet

param1=1
```



### 参考

- 《Linux 高性能服务器编程》- 游双著
- nowcoder - Linux 多进程开发
- 源码：[注释版-wgfxcu/HPS](https://github.com/wgfxcu/HPS)  [无注释版本-raichen/LinuxServerCodes](https://github.com/raichen/LinuxServerCodes)
