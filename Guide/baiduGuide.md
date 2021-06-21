# 百度

## C++

### 1. 简述 C++ 中智能指针的特点，简述 new 与 malloc 的区别

- 特点
  1. 是一个类，存储指向动态分配对象的指针，负责自动释放动态分配的对象，防止堆内存泄漏。动态分配的资源，交给一个类对象去管理，当类对象声明周期结束时，自动调用析构函数释放资源。普通指针如果忘记释放之后容易造成内存泄漏
  2. shared_ptr：采用计数器方式；unique_ptr：不支持普通的拷贝和复制操作；weak_ptr：弱引用，打破环形引用；

- 区别参考：[new、delete以及malloc、free区别](https://github.com/EricPengShuai/Interview/blob/main/review/baidu.md)

  

### 2. 简述 C++ 的内联函数

内联函数是C++的增强特性之一，用来降低程序的运行时间。当内联函数收到编译器的指示时，即可发生内联：编译器将使用函数的定义体来**替代**函数调用语句，这种替代行为**发生在编译阶段而非程序运行阶段**。

值得注意的是，内联函数仅仅是对编译器的内联建议，编译器是否觉得采取你的建议取决于**函数是否符合内联的有利条件**。如何函数体非常大，那么编译器将忽略函数的内联声明，而将内联函数作为普通函数处理。

参考：https://www.cnblogs.com/qg-whz/p/4641479.html



### 3. C++ 中多态是怎么实现的

在基类的函数前加上**virtual**关键字，在派生类中重写该函数，运行时将会根据所指对象的实际类型来调用相应的函数，如果对象类型是派生类，就调用派生类的函数，如果对象类型是基类，就调用基类的函数。

虚表：虚函数表的缩写，类中含有virtual关键字修饰的方法时，编译器会自动生成虚表

虚表指针：在含有虚函数的类实例化对象时，对象地址的前四个字节存储的指向虚表的指针

会有两张虚表：

![子类](https://cdn.jsdelivr.net/gh/forthespada/mediaImage2@2.6/202104/C++-36-1.png)



![父类](https://cdn.jsdelivr.net/gh/forthespada/mediaImage2@2.6/202104/C++-36-2.png)



### 4. C++ 是如何进行内存管理的？

- 含freelist的分配器

  - 空闲空间管理方式
    - 隐式空闲链表：将空闲链表信息与内存块存储在一起。主要记录大小，已分配位等信息
    - 显示空闲链表：单独维护一块空间来记录所有空闲块信息
    - 分离适配：将不同大小的内存块放在一起容易造成外部碎片，可以设置多个freelist，并让每个freelist存储不同大小的内存块，申请内存时选择满足条件的最小内存块
    - 位图：除了freelist之外，还可以考虑用0，1表示对应内存区域是否已分配，称为位图
  - 怎样分配内存块
    - 首次适应
    - 最佳适应
    - 循环首次适应

- buddy分配器

  按照一分为二，二分为四的原则，直到分裂出一个满足大小的内存块；合并的时候看buddy是否空闲，如果是就合并。

  可以通过位运算直接算出buddy，buddy的buddy，速度较快。但内存碎片较多。

<img src="https://pic2.zhimg.com/80/v2-26bdd3d3bcb29400bf29ff98d322ec9d_1440w.jpg" style="zoom:80%;" />

操作系统将Physical Memory映射为连续的Virtual Memory**(通过TLB)**，并提供了一些与Virtual Memory相关的API(VirtualAlloc，VirtualFree...)对Virtual Memory进行管理，在Virtual Memory API之上又构建了**Heap Memory Memory API (HeapAlloc...)**，而C的内存管理机制(malloc，free)就构建在 Heap Memory Memory API之上。



参考1：https://zhuanlan.zhihu.com/p/51855842

参考2：https://zhuanlan.zhihu.com/p/344377490



### 5. C++ 中虚函数与纯虚函数的区别

参考：[GitHub](https://github.com/EricPengShuai/Interview/blob/main/Guide/ByteDanceGuide.md#6-c-%E4%B8%AD%E8%99%9A%E5%87%BD%E6%95%B0%E4%B8%8E%E7%BA%AF%E8%99%9A%E5%87%BD%E6%95%B0%E7%9A%84%E5%8C%BA%E5%88%AB)



### 6. lambda 函数的特点，和普通函数相比有什么优点？

- 利用lambda表达式可以**编写内嵌的匿名函数**，用以替换独立函数或者函数对象；
- 每当你定义一个lambda表达式后，编译器会自动生成一个**匿名类**（这个类当然重载了()运算符），我们称为闭包类型（closure type）。那么在运行时，这个lambda表达式就会返回一个匿名的闭包实例，其实一个右值。所以，我们上面的lambda表达式的结果就是一个个闭包。闭包的一个强大之处是其可以通过传值或者引用的方式捕捉其封装作用域内的变量，前面的方括号就是用来定义捕捉模式以及变量，我们又将其称为lambda捕捉块。



### 7. 父类和子类是不是在同一个虚函数表

不是



### 8. 简述 C++ 的内存分区

<img src="https://pic1.zhimg.com/80/v2-6b739f63e83c84d07db2b7badcbe472c_1440w.jpg" style="zoom:50%;" />

- **代码区（Code Segment）**：存放**可执行程序的机器码**
- **数据区（Data Segment）**：存放**已初始化的全局和静态变量， 常量数据**（如字符串常量）
- **全局区**：block started by symbol，存放**未初始化的全局和静态变量**
- **Heap**：从低地址向高地址增长。容量大于栈，程序中**动态分配的内存**在此区域
- **Stack**：从高地址向低地址增长。由编译器自动管理分配。程序中的**局部变量、函数参数值、返回变量**等存在此区域



> #### 堆和栈的不同？
>
> 1. 分配和管理方式不同
>
> 堆是**动态分配**的，其空间的分配和释放都由程序员控制。 
>
> 栈由**编译器自动管理**。栈有两种分配方式：静态分配和动态分配。静态分配由编译器完成，比如局部变量的分配。动态分配由_alloca()函数进行分配，但是栈的动态分配和堆是不同的，它的动态分配是由编译器进行释放，无须手工控制。
>
> 2. 产生碎片不同
>
> 对堆来说，频繁的new/delete或者malloc/free可能会造成内存空间的不连续，造成大量的碎片，使程序效率降低。
>
> 对栈而言，则不存在碎片问题，因为**栈是先进后出的"队列"**，永远不可能有一个内存块从栈中间弹出。
>
> 3. 增长方向不同
>
> **堆由低地址向高地址增长。**
>
> **栈由高地址向低地址增长。**



### 9. STL 中 vector 与 list 具体是怎么实现的？常见操作的时间复杂度是多少？

- **vector**：和数组类似，拥有一段连续的内存空间，并且起始地址不变。因此能高效的进行随机存取，时间复杂度为o(1);

  **连续存储结构**：vector是可以实现动态增长的对象数组，支持对数组高效率的访问和在数组尾端的删除和插入操作，在中间和头部删除和插入相对不易，需要挪动大量的数据。它与数组最大的区别就是vector不需程序员自己去考虑容量问题，库里面本身已经实现了容量的动态增长，而数组需要程序员手动写入扩容函数进形扩容。

-  **list**：由双向链表实现的，因此内存空间是不连续的。只能通过指针访问数据，所以list的随机存取非常没有效率，时间复杂度为o(n);但由于链表的特点，能高效地进行插入和删除。

  **非连续存储结构**：list是一个双链表结构，支持对链表的双向遍历。每个节点包括三个信息：元素本身，指向前一个元素的节点（prev）和指向下一个元素的节点（next）。因此list可以高效率的对数据元素任意位置进行访问和插入删除等操作。由于涉及对额外指针的维护，所以开销比较大。

- **区别**：
  - vector的随机访问效率高，但在插入和删除时（不包括尾部）需要挪动数据，不易操作。
  - list的访问要遍历整个链表，它的随机访问效率低。但对数据的插入和删除操作等都比较方便，改变指针的指向即可。
  - list是单向的，vector是双向的。vector中的迭代器在使用后就失效了，而list的迭代器在使用之后还可以继续使用。 

### 10. 指针和引用的区别是什么？

- 指针是一个变量，存储的是一个地址，引用跟原来的变量实质上是同一个东西，是原变量的别名
- 指针可以有多级，引用只有一级
- 指针可以为空，引用不能为NULL且在定义时必须初始化
- 指针在初始化后可以改变指向，而引用在初始化之后不可再改变
- ……

---



## 计算机网络

#### 1. 从输入 URL 到展现页面的全过程

1. 浏览器**查询 DNS**

   获取域名对应的IP地址: 具体过程包括浏览器搜索自身的DNS缓存、搜索操作系统的DNS缓存、读取本地的Host文件和向本地DNS服务器进行查询等。对于向本地DNS服务器进行查询，如果要查询的域名包含在本地配置区域资源中，则返回解析结果给客户机，完成域名解析(此解析具有权威性)；如果要查询的域名不由本地DNS服务器区域解析，但该服务器已缓存了此网址映射关系，则调用这个IP地址映射，完成域名解析（此解析不具有权威性）。如果本地域名服务器并未缓存该网址映射关系，那么将根据其设置发起递归查询或者迭代查询；

   另可参考：[域名解析过程](https://blog.csdn.net/yipiankongbai/article/details/25031461)

2. 浏览器获得域名对应的IP地址以后，浏览器向服务器请求建立链接，发起**三次握手**；

3. TCP/IP链接建立起来后，浏览器向服务器**发送HTTP请求**；

4. 服务器接收到这个请求，并根据路径参数映射到特定的请求处理器进行处理，并**将处理结果及相应的视图返回给浏览器**；

5. 浏览器解析并**渲染视图**，若遇到对js文件、css文件及图片等静态资源的引用，则重复上述步骤并向服务器请求这些资源；

6. 浏览器根据其请求到的资源、数据渲染页面，最终向用户呈现一个完整的页面。

   

#### 2. 简述 TCP 三次握手以及四次挥手的流程。为什么需要三次握手以及四次挥手？

[参考](https://github.com/wolverinn/Waking-Up/blob/master/Computer%20Network.md)



#### 3. TCP 与 UDP 在网络协议中的哪一层，他们之间有什么区别？ 

传输层

- TCP是面向连接的，UDP是无连接的；

  > 什么叫无连接？
  >
  > UDP发送数据之前不需要建立连接

- TCP是可靠的，UDP不可靠；

  > 什么叫不可靠？
  >
  > UDP接收方收到报文后，不需要给出任何确认

- TCP只支持点对点通信，UDP支持一对一、一对多、多对一、多对多；

- TCP是面向字节流的，UDP是面向报文的；

  > 什么意思？
  >
  > 面向字节流是指发送数据时以字节为单位，一个数据包可以拆分成若干组进行发送，而UDP一个报文只能一次发完。

- TCP有拥塞控制机制。UDP没有，网络出现的拥塞不会使源主机的发送速率降低，这对某些实时应用是很重要的，比如媒体通信，游戏；

- TCP首部开销（20字节）比UDP首部开销（8字节）要大UDP 的主机不需要维持复杂的连接状态表

  

#### 4. HTTP 与 HTTPS 有哪些区别？[参考2](https://www.runoob.com/w3cnote/http-vs-https.html)

- HTTPS协议需要到**CA申请证书**，一般免费证书很少，需要交费。

- HTTP协议运行在**TCP之上**，所有传输的内容都是明文，HTTPS运行在**SSL/TLS之上**，SSL/TLS运行在TCP之上，所有传输的内容都经过加密的。
- HTTP和HTTPS使用的是**完全不同的连接方式**，用的端口也不一样，前者是80，后者是443。
- HTTPS可以有效的防止运营商劫持，解决了防劫持的一个大问题。



#### 5. 简述 DDOS 攻击原理，如何防范它？

Distribute Denial of Service：分布式拒绝服务

防范操作：

1. 高防服务器
2. 添加黑名单
3. DDoS清洗
4. CDN加速：CDN 服务将网站访问流量**分配到了各个节点**中，这样一方面隐藏网站的真实 IP，另一方面即使遭遇 DDoS 攻击，也可以将流量分散到各个节点中，防止源站崩溃。



#### 6. 简述 HTTP 1.0，1.1，2.0 的主要区别 [参考2](https://juejin.cn/post/6844903489596833800)

HTTP1.0最早在网页中使用是在1996年，那个时候只是使用一些较为简单的网页上和网络请求上，而HTTP1.1则在1999年才开始广泛应用于现在的各大浏览器网络请求中，同时HTTP1.1也是当前使用最为广泛的HTTP协议。 主要区别主要体现在：

1. **缓存处理**，在HTTP1.0中主要使用header里的If-Modified-Since,Expires来做为缓存判断的标准，HTTP1.1则引入了更多的缓存控制策略例如Entity tag，If-Unmodified-Since, If-Match, If-None-Match等更多可供选择的缓存头来控制缓存策略。
2. **带宽优化及网络连接的使用**，HTTP1.0中，存在一些浪费带宽的现象，例如客户端只是需要某个对象的一部分，而服务器却将整个对象送过来了，并且不支持断点续传功能，HTTP1.1则在请求头引入了range头域，它允许只请求资源的某个部分，即返回码是206（Partial Content），这样就方便了开发者自由的选择以便于充分利用带宽和连接。
3. **错误通知的管理**，在HTTP1.1中新增了24个错误状态响应码，如409（Conflict）表示请求的资源与资源的当前状态发生冲突；410（Gone）表示服务器上的某个资源被永久性的删除。
4. **Host头处理**，在HTTP1.0中认为每台服务器都绑定一个唯一的IP地址，因此，请求消息中的URL并没有传递主机名（hostname）。但随着虚拟主机技术的发展，在一台物理服务器上可以存在多个虚拟主机（Multi-homed Web Servers），并且它们共享一个IP地址。HTTP1.1的请求消息和响应消息都应支持Host头域，且请求消息中如果没有Host头域会报告一个错误（400 Bad Request）。
5. **长连接**，HTTP 1.1支持长连接（PersistentConnection）和请求的流水线（Pipelining）处理，在一个TCP连接上可以传送多个HTTP请求和响应，减少了建立和关闭连接的消耗和延迟，在HTTP1.1中默认开启Connection： keep-alive，一定程度上弥补了HTTP1.0每次请求都要创建连接的缺点。



#### 7. 简述 TCP 的 TIME_WAIT 和 CLOSE_WAIT  [参考2](https://zhuanlan.zhihu.com/p/60382685)

<img src="https://img-blog.csdn.net/20180410140623642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZWVudGVhbGM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70" alt="img" style="zoom:70%;" />

#### 8. TCP 中常见的拥塞控制算法有哪些？ 中等 [参考1](https://zh.wikipedia.org/wiki/TCP拥塞控制) [参考2](https://snailclimb.gitee.io/javaguide/#/docs/network/计算机网络?id=四-tcp-协议如何保证可靠传输)

- 慢启动和拥塞避免

- 快重传和快恢复

  当发送方连续收到三个重复确认时，就把慢开始门限减半，然后执行拥塞避免算法。不执行慢开始算法的原因：因为如果网络出现拥塞的话就不会收到好几个重复的确认，所以发送方认为现在网络可能没有出现拥塞。
  也有的快重传是把开始时的拥塞窗口cwnd值再增大一点，即等于 ssthresh + 3*MSS 。这样做的理由是：既然发送方收到三个重复的确认，就表明有三个分组已经离开了网络。这三个分组不再消耗网络的资源而是停留在接收方的缓存中。可见现在网络中减少了三个分组。因此可以适当把拥塞窗口扩大些。



#### 9. 简述 BGP 协议和 OSPF 协议的区别

- OSPF属于IGP（内部网关协议），主要作用是在网络内部发现、计算路由

- BGP属于EGP（外部网关协议），主要作用是在不同网络之间传递、控制路由（路由来源于IGP）

一个孤立的网络只需要IGP协议就可以实现内部互通，而需要和外部互通的时候就需要EGP协议（IGP本身也需要存在），不存在只运行BGP没有IGP的这种说法。

> OSPF(Open Shortest Path First开放式最短路径优先）是一个内部网关协议(Interior Gateway Protocol，简称IGP），用于在单一自治系统（autonomous system,AS）内决策路由。是对链路状态路由协议的一种实现，隶属内部网关协议（IGP），故运作于自治系统内部。著名的迪克斯加算法(Dijkstra)被用来计算最短路径树。
>
> OSPF分为OSPFv2和OSPFv3两个版本,其中OSPFv2用在IPv4网络，OSPFv3用在IPv6网络。OSPFv2是由RFC 2328定义的，OSPFv3是由RFC 5340定义的。与RIP相比，OSPF是链路状态协议，而RIP是距离矢量协议。



#### 10. TCP 的 keepalive 了解吗？说一说它和 HTTP 的 keepalive 的区别？ 简单 [参考1](https://stackoverflow.com/questions/9334401/http-keep-alive-and-tcp-keep-alive#:~:text=HTTP Keep-Alive is a feature that allows HTTP client,over the same TCP connection.&text=HTTP request%2C decreases network traffic,opened by sending small packets.)

- TCP

  在双方长时间未通讯时，如何得知对方还活着？如何得知这个TCP连接是健康且具有通讯能力的？

  **TCP的保活机制**就是用来解决此类问题，这个机制我们也可以称作：keepalive。保活机制默认是关闭的，TCP连接的任何一方都可打开此功能。如果在一段时间（保活时间：tcp_keepalive_time）内此连接都不活跃，开启保活功能的一端会向对端发送一个保活探测报文。

  > - 若对端正常存活，且连接有效，对端必然能收到探测报文并进行响应。此时，发送端收到响应报文则证明TCP连接正常，重置保活时间计数器即可。
  > - 若由于网络原因或其他原因导致，发送端无法正常收到保活探测报文的响应。那么在一定**探测时间间隔（tcp_keepalive_intvl）后，将继续发送保活探测报文。直到收到对端的响应，或者达到配置的探测循环次数上限（tcp_keepalive_probes）**都没有收到对端响应，这时对端会被认为不可达，TCP连接随存在但已失效，需要将连接做中断处理。

- HTTP

  HTTP协议简介中提到http协议是一个运行在TCP协议之上的无状态的应用层协议。它的特点是：**客户端的每一次请求都要和服务端创建TCP连接，服务器响应后，断开TCP连接。下次客户端再有请求，则重新建立连接。**

  在早期的http1.0中，默认就是上述介绍的这种“请求-应答”模式。这种方式频繁的创建连接和销毁连接无疑是有一定性能损耗的。所以引入了keep-alive机制。http1.0默认是关闭的，通过http请求头设置“connection: keep-alive”进行开启；**http1.1中默认开启**，通过http请求头设置“connection: close”关闭。

  keep-alive机制：若开启后，在一次http请求中，**服务器进行响应后，不再直接断开TCP连接，而是将TCP连接维持一段时间**。在这段时间内，如果同一客户端再次向服务端发起http请求，便可以复用此TCP连接，向服务端发起请求，并重置timeout时间计数器，在接下来一段时间内还可以继续复用。这样无疑省略了反复创建和销毁TCP连接的损耗。

- 参考：https://blog.csdn.net/lpf463061655/article/details/108460311



#### 11. 简述 OSI 七层模型，TCP，IP 属于哪一层？

<img src="https://github.com/wolverinn/Waking-Up/raw/master/_v_images/20191129195451183_16713.png" style="zoom:60%;" />

TCP/UDP在传输层，IP在网络层



#### 12. traceroute 有什么作用？

**寻路**

1. 构造一个UDP报文，TTL分别为1,当这个报文到达第一个路由器后，TTL减去1后为零，报文被丢弃，然后路由器发送ICMP报文（时间超过）给源主机。

2. 构造UDP报文，TTL为2，同样的，第二个路由器会返回ICMP报文（时间超过）给源主机

   > 以此类推，注意发送过程中的UDP报文使用的端口都是非法端口，以使最后达到的时候无法交付，不然你就不知道什么时候应该停

3. 直到TTL 到达某个值，而这个值可以使这个UDP报文刚好到达目的主机，由于报文的端口号不合法，目的主机返回ICMP（端口不可达）报文给源主机。

   > 上述过程中，路由器和目的主机返回的ICMP报文中均含有它们的地址，所以得到了路径。
   >
   > 实际实现中，源主机每次发送三个相同TTL的UDP报文，因为现实中网络环境比较复杂，可能会有丢包情况发生。



#### 13. DNS 查询服务器的基本流程是什么？DNS 劫持是什么？ 

[查询过程](https://blog.csdn.net/yipiankongbai/article/details/25031461)

[DNS劫持](https://github.com/EricPengShuai/Interview/blob/main/Guide/ByteDanceGuide.md#4-dns相关)

---



## 操作系统

### 1. 进程和线程之间有什么区别？

参考：[MyGitHub](https://github.com/EricPengShuai/Interview/blob/main/Guide/ByteDanceGuide.md#1-进程和线程区别)



### 2. 简述 select, poll, epoll 的使用场景以及区别，epoll 中水平触发以及边缘触发有什么不同？ 

这三个玩意是为了实现IO多路复用的，**IO多路复用（IO Multiplexing）**是指单个进程/线程就可以同时处理多个IO请求。



### 3. 进程间有哪些通信方式？ 

- 管道(Pipe)

  > 管道是半双工的，数据只能向一个方向流动；
  >
  > 需要双方通信时，需要建立起两个管道；一个进程向管道中写的内容被管道另一端的进程读出。写入的内容每次都添加在管道缓冲区的末尾，并且每次都是从缓冲区的头部读出数据；
  >
  > 只能用于父子进程或者兄弟进程之间(具有亲缘关系的进程)

- 命名管道

- 消息队列

- 信号(Signal)

- 共享内存

- 信号量(Semaphore)：初始化操作、P操作、V操作；

  > P操作：信号量-1，检测是否小于0，小于则进程进入阻塞状态；V操作：信号量+1，若小于等于0，则从队列中唤醒一个等待的进程进入就绪态

- 套接字(Socket)



### 4. 简述 Linux 进程调度的算法 

- 先来先服务
- 最短作业优先
- 时间片轮转
- 多级反馈队列

> 参考：https://blog.csdn.net/qq_41943585/article/details/91357110



### 5. 简述几个常用的 Linux 命令以及他们的功能

参考：https://www.cnblogs.com/xuxinstyle/p/9609551.html

---



## 数据库

### 1. MySQL 为什么使用 B+ 树来作索引，对比 B 树它的优点和缺点是什么？ 

参考：[MyGitHub](https://github.com/EricPengShuai/Interview/blob/main/Guide/ByteDanceGuide.md#2-mysql-为什么使用-b-树来作索引对比-b-树它的优点和缺点是什么)



### 2. 联合索引的存储结构是什么？

参考：https://blog.csdn.net/ibigboy/article/details/104571930

<img src="https://imgconvert.csdnimg.cn/aHR0cHM6Ly91c2VyLWdvbGQtY2RuLnhpdHUuaW8vMjAyMC8yLzI3LzE3MDg2N2NiNmFmMGE3MmQ?x-oss-process=image/format,png" style="zoom:60%;" />



### 3. 聚簇索引和非聚簇索引有什么区别？ 

- 聚集索引：数据行的物理顺序与列值（一般是主键的那一列）的逻辑顺序相同，一个表中只能拥有一个聚集索引。**索引的叶子节点就是对应的数据节点**

- 非聚集索引：该索引中索引的逻辑顺序与磁盘上行的物理存储顺序不同，一个表中可以拥有多个非聚集索引。[涉及到二次查询](https://www.cnblogs.com/s-b-b/p/8334593.html)：**非聚集索引叶节点仍然是索引节点，只是有一个指针指向对应的数据块，此如果使用非聚集索引查询，而查询列中包含了其他该索引没有覆盖的列，那么他还要进行第二次的查询，查询节点上对应的数据行的数据**

  

### 4. 什么是数据库事务，MySQL 为什么会使用 InnoDB 作为默认选项

事务指的是满足 ACID 特性的一组操作，可以通过 Commit 提交一个事务，也可以使用 Rollback 进行回滚

- InnoDB**支持事务**，可以进行Commit和Rollback；
- MyISAM 只支持表级锁，而 InnoDB 还**支持行级锁**，提高了并发操作的性能；InnoDB **支持外键**；
- MyISAM **崩溃**后发生损坏的概率比 InnoDB 高很多，而且**恢复的速度**也更慢；
- MyISAM 支持**压缩**表和空间数据索引，InnoDB需要更多的内存和存储；
- InnoDB 支持在线**热备份**

另外参考：https://www.jianshu.com/p/6b45b150bfbf



### 5. 数据库的事务隔离级别有哪些？各有哪些优缺点？ 

- **未提交读**：事务中的修改，即使没有提交，对其它事务也是可见的。**可以保证读取没有损坏的数据**
- **提交读**：一个事务只能读取已经提交的事务所做的修改。换句话说，一个事务所做的修改在提交之前对其它事务是不可见的。**可以避免脏读**
- **可重复读**：保证在同一个事务中多次读取同一数据的结果是一样的。**可以避免不可重复读的发生**
- **可串行化**：强制事务串行执行，这样多个事务互不干扰，**不会出现并发一致性问题**。该隔离级别需要加锁实现，因为要使用加锁机制保证同一时间只有一个事务执行，也就是保证事务串行执行。



### 6. redis 如何实现高可用？



### 7. 简述乐观锁以及悲观锁的区别以及使用场景 

- 悲观锁：认为数据随时会被修改，因此每次读取数据之前都会上锁，防止其它事务读取或修改数据；应用于**数据更新比较频繁**的场景；
- 乐观锁：操作数据时不会上锁，但是更新时会判断在此期间有没有别的事务更新这个数据，若被更新过，则失败重试；适用于读多写少的场景。乐观锁的实现方式有：
  - 加一个版本号或者时间戳字段，每次数据更新时同时更新这个字段；
  - 先读取想要更新的字段或者所有字段，更新的时候比较一下，只有字段没有变化才进行更新



### 8. 简述 MySQL 三种日志的使用场景

- **binlog**：`binlog`用于记录数据库执行的写入性操作(不包括查询)信息，以二进制的形式保存在磁盘中。`binlog`是`mysql`的逻辑日志，并且由`Server`层进行记录，使用任何存储引擎的`mysql`数据库都会记录`binlog`日志

  >在实际应用中，`binlog`的主要使用场景有两个，分别是**主从复制**和**数据恢复**。

- **Redo log**：具体来说就是只记录事务对数据页做了哪些修改

  > 用于保持一致性

- **Undo log**：**原子性** 底层就是通过 `undo log` 实现的。`undo log`主要记录了数据的逻辑变化，比如一条 `INSERT` 语句，对应一条`DELETE` 的 `undo log` ，对于每个 `UPDATE` 语句，对应一条相反的 `UPDATE` 的 `undo log` ，这样在发生错误时，就能回滚到事务之前的数据状态。



### 9. Redis 如何实现分布式锁？ 困难 [参考1](https://redis.io/topics/distlock) [参考2](https://www.redis.com.cn/topics/distlock.html)



### 10. 简述 Redis 中如何防止缓存雪崩和缓存击穿 中等 [参考1](https://www.pixelstech.net/article/1586522853-What-is-cache-penetration-cache-breakdown-and-cache-avalanche) [参考2](https://zhuanlan.zhihu.com/p/346651831)



### 11. 什么情况下会发生死锁，如何解决死锁？ 中等 [参考1](https://dev.mysql.com/doc/refman/8.0/en/innodb-deadlocks.html) [参考2](https://github.com/SmartKeyerror/Psyduck#bento-database)

如果线程A锁住了记录1并等待记录2，而线程B锁住了记录2并等待记录1，这样两个线程就发生了死锁现象

- **主要原因**：系统资源不足； 进程运行推进的顺序不合适；资源分配不当等。
- **避免死锁**：破坏出现死锁的4个必要条件中的某一个：不让线程循环等待



### 12. 什么是 SQL 注入攻击？如何防止这类攻击？

https://blog.csdn.net/qq_43437465/article/details/96771962

另参考：[CSDN](https://blog.csdn.net/weixin_46856842/article/details/106203132?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-0&spm=1001.2101.3001.4242)



### 13. 数据库如何设计索引，如何优化查询？



### 14. 简述 Redis 的通信模型



### 15. 简述数据库中的 ACID 分别是什么？ 简单



### 16. 简述 SQL 中左连接和右连接的区别



### 17. MySQL中 InnoDB 和 MylSAM 的区别是什么？ 简单



### 18. 数据库有哪些常见索引？数据库设计的范式是什么？ 中等 [参考1](https://smartkeyerror.oss-cn-shenzhen.aliyuncs.com/Phyduck/database/深入浅出 InnoDB Index.pdf)



### 19. MySQL 中 join 与 left join 的区别是什么？ 简单



#### 20. 简述什么是最左匹配原则 简单



#### 21. Redis 有几种数据结构？Zset 是如何实现的？ 中等 [参考1](https://redis.io/topics/data-types) [参考2](https://smartkeyerror.oss-cn-shenzhen.aliyuncs.com/Phyduck/database/Redis数据结构.pdf)



#### 22. MySQL 有哪些常见的存储引擎？它们的区别是什么？ 中等



#### 23. 什么是公平锁？什么是非公平锁？ 简单




