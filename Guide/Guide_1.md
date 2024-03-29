## _ByteDance_


### 🚀操作系统🚀

#### 1. 进程和线程区别

- 进程是程序的一次执行过程，是一个动态概念，**是程序在执行过程中分配资源的基本单位**，每一个进程都有一个自己的地址空间，至少有 5 种基本状态，它们是：初始态，执行态，等待状态，就绪状态，终止状态。
- **线程是CPU调度的基本单位**，它可与同属一个进程的其他的线程共享进程所拥有的全部资源。

> 联系： 线程是进程的一部分，一个线程只能属于一个进程，而一个进程可以有多个线程，但至少有一个线程。线程共享进程的资源：**进程代码段、进程的公有数据(利用这些共享的数据，线程很容易实现相互之间的通讯)、进程打开的文件描述符、信号的处理器、进程的当前目录和进程用户ID与进程组ID**
>
> 所处环境：在操作系统中能同时运行多个进程（程序）；而在同一个进程（程序）中有多个线程同时执行（通过CPU调度，在每个时间片中只有一个线程执行）
>
> 包含关系：没有线程的进程可以看做是单线程的，如果一个进程内有多个线程，则执行过程不是一条线的，而是多条线（线程）共同完成的；线程是进程的一部分，所以线程也被称为轻权进程或者轻量级进程。
>
> > **相关概念**
> >
> > **并发**：是指同一个时间段内多个任务同时都在执行，并且都没有执行结束。并发任务强调在一个时间段内同时执行，而一个时间段由多个单位时间累积而成，所以说并发的多个任务在单位时间内不一定同时在执行
> >
> > **并行**：是说在单位时间内多个任务同时在执行 
> >
> > 在多线程编程实践中，线程的个数往往多于CPU的个数，所以一般都称**多线程并发编程而不是多线程并行编程**。



#### 2. 进程间的通信方式

- 管道：匿名管道`|`和命名管道`mkfifo`，前者只能用于父子或者兄弟进程，后者可以用于任意进程
- 共享内存：最快的IPC方式，往往和信号量结合起来使用以达到进程的同步与互斥
- 信号量 
- 消息队列
- socket：往往是不同主机（网络）进程之间的通信

**信号量与互斥锁的不同**

- 信号量（semaphore[ˈseməfɔ:(r)]）用在多线程多任务同步的，一个线程完成了某一个动作就通过信号量告诉别的线程，别的线程再进行某些动作。
- 互斥锁（Mutual exclusion，缩写 Mutex）是用在多线程多任务互斥的，一个线程占用了某一个资源，那么别的线程就无法访问，直到这个线程unlock，其他的线程才开始可以利用这个资源。

> mutex是semaphore的一种特殊情况（n=1时）



#### 3. 申请和管理内存

- 申请：申请虚拟地址

  - brk是将数据段的最高地址指针_edata往高地址推；
  - mmap是在进程的虚拟地址空间中（堆和栈中间，文件映射区）找一块空闲的虚拟内存。

  > **这两种方式分配的都是虚拟内存，没有分配物理内存。在第一次访问已分配的虚拟地址空间的时候，发生缺页中断，操作系统负责分配物理内存，然后建立虚拟内存和物理内存之间的映射关系**

- 管理：通过首次适应、最佳适应、最坏适应分配内存块

  - 页式管理：内部碎片
  - 段式管理：外部碎片
  - 段页式管理：内部碎片



#### 4. 虚拟地址与物理地址相关

1. 如何映射？

   - 虚拟内存地址由页号（与页表中的页号关联）和偏移量组成

   - 虚拟内存地址的大小是与地址总线位数相关，物理内存地址的大小跟物理内存条的容量相关。

   - 页号+页内偏移


2. Linux 中虚拟内存和物理内存有什么区别？有什么优点？

   - 一个新进程建立的时候，将会建立起自己的内存空间，各个进程的内存空间具有类似的结构，这就是虚拟内存。当进程访问某个虚拟地址，去看页表，都需要把地址翻译为实际物理内存地址，如果发现对应的数据不在物理内存中，则缺页异常。

   - 优点
     - 既然每个进程的内存空间都是一致而且固定的，所以链接器在链接可执行文件时，可以设定内存地址，而不用去管这些数据最终实际的内存地址，这是有独立内存空间的好处
     - 当不同的进程使用同样的代码时，比如库文件中的代码，物理内存中可以只存储一份这样的代码，不同的进程只需要把自己的虚拟内存映射过去就可以了，节省内存



#### 5.  简述 socket 中 select 与 epoll 的使用场景以及区别，epoll 中水平触发以及边缘触发有什么不同？

> I/O多路复用中 select, poll, epoll之间有什么区别，各自支持的最大描述符上限以及原因是什么？

参考：[Cyc2018-IO复用](http://www.cyc2018.xyz/计算机基础/Socket/Socket.html#二、i-o-复用)

- [网络 IO 演变过程](https://zhuanlan.zhihu.com/p/353692786)
- [深入浅出理解select、poll、epoll的实现](https://zhuanlan.zhihu.com/p/367591714)
- [答应我，这次搞懂 I/O 多路复用！](https://blog.csdn.net/qq_34827674/article/details/115619261)



#### 6. 简述操作系统中的缺页中断？

缺页中断：在请求分页系统中，可以通过查询页表中的状态位来确定所要访问的页面是否存在于内存中。每当所要访问的页面不在内存中时，会产生一次缺页中断，此时操作系统会根据页表中的外存地址在外存中找到所缺的一页，将其调入内存。

缺页中断是由于所要访问的页面不存在于内存时，由**硬件所产生的一种特殊的中断**，因此，与一般的中断存在区别：

1. 在指令执行期间产生和处理缺页中断信号
2. 一条指令在执行期间，可能产生多次缺页中断
3. 缺页中断返回是执行产生中断的一条指令，而一般的中断返回是执行下一条指令

> 页表在内存中，为了加快对页表的访问，加入了 TLB（Transfer Look-aside Table）快表/高速地址缓存表，TLB 表在缓存中，如果命中直接从内存访问数据，否则就去内存找页表



#### ==7.  两个线程交替打印一个共享变量==



#### 8. 简述 mmap 的使用场景以及原理

Linux通过将一个虚拟内存区域与一个磁盘上的对象(object)关联起来，以初始化这个虚拟内存区域的内容，这个过程称为**内存映射**(memory mapping)。

**mmap是用来建立从虚拟空间到磁盘空间的映射的**，可以将一个虚拟空间地址映射到一个磁盘文件上，当不设置这个地址时，则由系统自动设置，函数返回对应的内存地址（虚拟地址），当访问这个地址的时候，就需要把磁盘上的内容拷贝到内存了，然后就可以读或者写，最后通过munmap可以将内存上的数据换回到磁盘，**也就是解除虚拟空间和内存空间的映射，这也是一种读写磁盘文件的方法，也是一种进程共享数据的方法 共享内存**



#### 9. 什么时候会由用户态陷入内核态？

a.  **系统调用**：用户进程通过系统调用申请操作系统提供的服务程序完成工作，调用int `$0x80`的汇编指令（软中断）

b.  **异常**：当CPU在执行运行在用户态的程序时，发现了某些事件不可知的异常，这是会触发由当前运行进程切换到处理此异常的内核相关程序中，也就到了内核态，比如缺页异常（硬中断）

c.  **外围设备的中断**：硬盘读写操作完成，系统会切换到硬盘读写的中断处理程序中执行后续操作等



#### 10. 什么情况下，进程会进行切换？

<img src="https://i.loli.net/2021/03/27/Ow78cQbXI9L1vkV.png" alt="2.1.2 进程状态切换" style="zoom:50%;" />

进程切换和线程切换的区别？

> 最主要的一个区别在于**进程切换涉及虚拟地址空间的切换而线程不会**。因为每个进程都有自己的虚拟地址空间，而线程是共享所在进程的虚拟地址空间的，因此同一个进程中的线程进行线程切换时不涉及虚拟地址空间的转换。

有的同学可能还是不太明白，为什么虚拟地址空间切换会比较耗时呢？

> 现在我们已经知道了进程都有自己的虚拟地址空间，把虚拟地址转换为物理地址需要查找页表，页表查找是一个很慢的过程，因此通常使用 cache 来缓存常用的地址映射，这样可以加速页表查找，这个 cache 就是 TLB（Translation Lookaside Buffer，我们不需要关心这个名字只需要知道 TLB 本质上就是一个 cache，是用来加速页表查找的）。由于每个进程都有自己的虚拟地址空间，那么显然每个进程都有自己的页表，那么**当进程切换后页表也要进行切换，页表切换后 TLB 就失效了**，cache 失效导致命中率降低，那么虚拟地址转换为物理地址就会变慢，表现出来的就是程序运行会变慢，而线程切换则不会导致 TLB 失效，因为线程无需切换地址空间，因此我们通常说线程切换要比较进程切换块，原因就在这里。



#### 11. 简述 traceroute 命令的原理

1. 构造一个 UDP 报文，TTL 分别为1，当这个报文到达第一个路由器后，TTL 减去1后为零，报文被丢弃，然后路由器发送 ICMP 报文（时间超过）给源主机。

2. 构造 UDP 报文，TTL 为2，同样的，第二个路由器会返回 ICMP 报文（时间超过）给源主机

   > 以此类推，注意发送过程中的 UDP 报文使用的端口都是**非法端口**，以使最后达到的时候无法交付，不然你就不知道什么时候应该停

3. 直到 TTL 到达某个值，而这个值可以使这个UDP报文刚好到达目的主机，由于报文的端口号不合法，目的主机返回 ICMP（端口不可达）报文给源主机。

   > 上述过程中，路由器和目的主机返回的ICMP报文中均含有它们的地址，所以得到了路径。
   >
   > 实际实现中，源主机每次发送三个相同TTL的UDP报文，因为现实中网络环境比较复杂，可能会有丢包情况发生。



#### 12.简述 Linux 零拷贝的原理

**零拷贝(Zero-Copy)** 是指将数据直接从磁盘文件复制到网卡设备中，而不需要经由应用程序之手 。零拷贝大大提高了应用程序的性能，**减少了内核和用户模式之间的上下文切换** 。 对 Linux操作系统而言，零拷贝技术依赖于底层的 `sendfile()` 方法实现 。

> 考虑这样一种常用的情形 ： 你需要将静态内容(类似图片、文件)展示给用户 。 这个情形就意味着需要先将静态内容从磁盘中复制出来放到一个内存 buf 中，然后将这个 buf 通过套接字（Socket）传输给用户，进而用户获得静态内容 
>
> ```java
> read(file, tmp_buf, len);
> write(socket, tmp_buf, len);
> ```
>
> 如果采用了零拷贝技术，那么应用程序可以直接请求内核把磁盘中的数据传输给 Socket
>
> https://zhuanlan.zhihu.com/p/85571977



#### 13. 进程调度算法

**批处理系统**：保证吞吐量和周转时间

- 先来先服务(FCFS)/先进先出(FIFO)调度算法
- 短作业优先调度算法（SJF）
- 最短剩余时间优先（SRTN）：SJF的抢占式版本

**交互式系统**：保证响应时间

- 简单的时间片轮转法(RR—Round Robin)
- 高优先权调度算法
- 高响应比优先调度算法
- 多级反馈队列



#### 14. 简述自旋锁与互斥锁的使用场景

加锁的目的就是保证共享资源在任意时间里，只有一个线程访问，这样就可以避免多线程导致共享数据错乱的问题。

当已经有一个线程加锁后，其他线程加锁则就会失败，互斥锁和自旋锁对于加锁失败后的处理方式是不一样的：

- **互斥锁**加锁失败后，线程会**释放 CPU** ，给其他线程；
- **自旋锁**加锁失败后，线程会**忙等待**，直到它拿到锁；

所以，**如果你能确定被锁住的代码执行时间很短，就不应该用互斥锁，而应该选用自旋锁，否则使用互斥锁。**

**当加锁失败时，互斥锁用「线程切换」来应对，自旋锁则用「忙等待」来应对**。

参考：[互斥锁、自旋锁、读写锁、悲观锁、乐观锁的应用场景—CSDN](https://blog.csdn.net/qq_34827674/article/details/108608566)

---



### 💻︎计算机网络💻︎

#### 1. HTTP vs HTTPS

1. **HTTP**：HTTP 协议是以明文方式发送信息的，如果黑客截取了 Web 浏览器和服务器之间的传输报文，就可以直接获得其中的信息。

   ① 客户端的浏览器首先要通过网络与服务器建立连接，该连接是通过 TCP 来完成的，一般 **TCP 连接的端口号是80**。 建立连接后，客户机发送一个请求给服务器，请求方式的格式为：**统一资源标识符（URI）、协议版本号，后边是 MIME 信息**包括请求修饰符、客户机信息和许可内容。
   
   <table> 	
   <tr> 
   <td>统一资源标识符（URI）</td> 
   <td>协议版本号</td> 
   <td>MIME信息</td>
   </tr>
   </table>
   
   ② 服务器接到请求后，给予相应的**响应**信息，其格式为**一个状态行，包括信息的协议版本号、一个成功或错误的代码，后边是 MIME 信息**包括服务器信息、实体信息和可能的内容。
   
   <table> 	
   <tr> 
   <td>状态行（包括协议版本号、code）</td>  
   <td>MIME信息</td>
   </tr>
   </table>
   
2. **HTTPS**：是以安全为目标的 HTTP 通道，是 HTTP 的安全版。HTTPS 的安全基础是 SSL。SSL 协议位于TCP/IP 协议与各种应用层协议之间，为数据通讯提供安全支持。**端口号是443**

   SSL 协议可分为两层：**SSL 记录协议（SSL Record Protocol）** 建立在可靠的传输协议（如TCP）之上，为高层协议提供数据封装、压缩、加密等基本功能的支持。**SSL 握手协议（SSL Handshake Protocol）** 建立在 SSL 记录协议之上，用于在实际的数据传输开始前，通讯双方进行身份认证、协商加密算法、交换加密密钥等。
   
   <img src="https://img-blog.csdnimg.cn/20200707084720409.png" alt="img" style="zoom:75%;" />
   
   **双向身份认证**
   
   客户端和服务器传输数据之前，会有一个基于X.509证书的双向身份认证的过程：
   
   ① 客户端发起SSL handshake protocol消息给服务器
   
   ② 服务端将「证书=签名+原信息」发给给客户端验证
   
   ③ 客户端检查服务端证书，合规就认证了服务端
   
   ④ 服务端要求客户端发送证书，并检查是否通过验证，成功之后从证书中拿到公钥
   
   

3. **Cookie和Session的关系和区别是什么**

   - cookie 和 session 都是用来跟踪浏览器用户身份的会话方式。[了解一下两者的过程](https://blog.csdn.net/chen13333336677/article/details/100939030)
   - cookie 数据保存在客户端，session 数据保存在服务端
     - 浏览器第一次访问服务器时，服务器端创建 Cookie，该 Cookie 中包含用户的信息，然后将该 Cookie 发送到浏览器端，服务器端通过 Cookie 中携带的数据区分不同的用户
     - session 基于 cookies 实现，浏览器第一次访问服务器时候创建一个 session，同时会创建一个特殊的 Cookie（`name`为`JSESSIONID`的固定值，`value`为`session对象的ID`），然后将该 Cookie 发送至浏览器端，服务器端根据 name 为 JSESSIONID 的 Cookie 的 value(sessionId)，去查询Session对象，从而区分不同用户。
   - cookies 不是很安全，存储数据大小限制 3K，有 cookies 欺骗，session 一般存放登录等重要信息
   - session 会在一定时间内保存在服务器上。当访问增多，会比较占用你服务器的性能，如果主要考虑到减轻服务器性能方面，应当使用 Cookies



#### 2. TCP && UDP 相关

##### 2.1 TCP vs UDP

1. 前者有连接，建立连接三次握手；后者无连接
2. TCP要求系统资源较多，UDP较少、UDP程序结构较简单 
3. TCP通过**拥塞控制、滑动窗口**机制实现**可靠传输**，保证数据正确性、数据顺序（也就是流模式），UDP可能丢包，不保证数据顺序（也就是数据报模式）

> TCP应用场景：面向字节流，适用于对数据传输可靠性要求比较高的场景，例如**文本传输**之类的。具体点比如HTTP、HTTPS、FTP等传输文件的协议，POP、SMTP等邮件传输的协议。
>
> UDP应用场景：面向数据报方式，网络数据大多为**短消息**，拥有大量Client等广播、多播通信，对数据安全性无特殊要求，视频音频等对**实时性要求很高的多媒体通信**，具体协议比如：DNS，NFS

- TCP格式

<img src="https://img-blog.csdn.net/20180410135625408" alt="img" style="zoom:80%;" />

- UDP格式

<img src="https://img-blog.csdn.net/20180410135845213" alt="img" style="zoom:80%;" />



##### 2.2 TCP 连接和释放

（1）三次握手

<img src="https://img-blog.csdn.net/20180410140138631" alt="img" style="zoom:70%;" />

- 首先是服务器监听状态
-  **[→]** SYN=1，同时选择一个初始序列号 seq=x（不携带数据，但是消耗一个序号）
- **[←]** SYN=1，确认号是ack=x+1，同时也要为自己初始化一个序列号 seq=y （不携带数据，但是消耗一个序号）
- **[→]** 客户端确认报文，ACK=1，确认号ack=y+1，自己的序列号seq=x+1（可以携带数据，但是如果不携带数据就不消耗一个序号）

> 问题：为什么需要三次握手？
>
> 答案：如果只有两次握手，假设前两次就可以建立连接，假设一个场景：客户端发送一个请求建立连接，但是由于网络延迟很久之后才到服务器，服务器这边也就不会立马ack，此时客户端以为丢失所以就是重传，这时很快就完成了前两次的握手过程，然后传输数据释放连接。**但是之后服务器又收到之前那个请求，又给客户端这边ack，又会建立连接（本来不需要了），所以就造成了不必要的[资源浪费](https://blog.csdn.net/wwl012345/article/details/90261423)。但是如果是三次握手，客户端这边收到ack之后才会给服务器那边ACK**
>
> 另外甚至可能[发生死锁](https://blog.csdn.net/qq_38950316/article/details/81087809)，客户端这边一直等待服务器发送ack报文

（2）四次挥手

<img src="https://img-blog.csdn.net/20180410140623642" alt="img" style="zoom:70%;" />

- **[→]** 客户端进程发出连接释放报文，并且停止发送数据。释放数据报文首部，FIN=1，其序列号为seq=u（等于前面已经传送过来的数据的最后一个字节的序号加1），**客户端进入FIN-WAIT-1（终止等待1）状态**。 TCP规定，FIN报文段即使不携带数据，也要消耗一个序号。
- **[←]** 服务器收到连接释放报文，发出确认报文，ACK=1，ack=u+1，并且带上自己的序列号seq=v，**服务端就进入了CLOSE-WAIT（关闭等待）状态。**
- 客户端收到服务器的确认请求后，此时，**客户端就进入FIN-WAIT-2（终止等待2）状态**，等待服务器发送连接释放报文（在这之前还需要接受服务器发送的最后的数据）。
- **[←]** 服务器将最后的数据发送完毕后，就向客户端发送连接释放报文，FIN=1，ack=u+1，由于在半关闭状态，服务器很可能又发送了一些数据，假定此时的序列号为seq=w，**服务器就进入了LAST-ACK（最后确认）状态，等待客户端的确认。**
- **[→]** 客户端收到服务器的连接释放报文后，必须发出确认，ACK=1，ack=w+1，而自己的序列号是seq=u+1，**客户端就进入了TIME-WAIT（时间等待）状态**。注意此时TCP连接还没有释放，必须经过2MSL（最长报文段寿命）的时间后，当客户端撤销相应的传输控制块（TCB）后，才进入CLOSED状态。
- **服务器只要收到了客户端发出的确认，立即进入CLOSED状态**。同样，撤销TCB后，就结束了这次的TCP连接。可以看到，服务器结束TCP连接的时间要比客户端早一些。

> 【问题1】为什么连接的时候是三次握手，关闭的时候却是四次握手？
>
> 答：**因为当Server端收到Client端的SYN连接请求报文后，可以直接发送SYN+ACK报文**。但是关闭连接时，当Server端收到FIN报文时，很可能并不会立即关闭SOCKET，所以只能先回复一个ACK报文，告诉Client端，"你发的FIN报文我收到了"。**只有等到我Server端所有的报文都发送完了，我才能发送FIN报文，因此不能一起发送**。故需要四步握手。
>
> 【问题2】为什么TIME_WAIT状态需要经过2MSL(最大报文段生存时间)才能返回到CLOSE状态？
>
> 答：**有可以最后一个ACK丢失。所以TIME_WAIT状态就是用来重发可能丢失的ACK报文**。Server如果没有收到ACK，将不断重复发送FIN片段。所以Client不能立即关闭，它必须确认Server接收到了该ACK。Client会设置一个计时器，等待2MSL的时间。如果在该时间内再次收到FIN，那么Client会重发ACK并再次等待2MSL。所谓的2MSL是两倍的MSL(Maximum Segment Lifetime)。MSL指一个片段在网络中最大的存活时间，2MSL就是一个发送和一个回复所需的最大时间。如果直到2MSL，Client都没有再次收到FIN，那么Client推断ACK已经被成功接收，则结束TCP连接
>
> 【问题3】如果已经建立了连接，但是客户端突然出现故障了怎么办？
>
> 答：TCP还设有一个保活计时器，显然，客户端如果出现故障，服务器不能一直等下去，白白浪费资源。服务器每收到一次客户端的请求后都会重新复位这个计时器，时间通常是设置为2小时，若两小时还没有收到客户端的任何数据，服务器就会发送一个探测报文段，以后每隔75秒钟发送一次。若一连发送10个探测报文仍然没反应，服务器就认为客户端出了故障，接着就关闭连接。


##### 2.3 数据包封装和解封过程

<img src="https://img-blog.csdnimg.cn/20191013113757339.png" alt="封装和解封" style="zoom:70%;" />



##### 2.4 TCP 滑动窗口

- Stop-Wait：停等协议，swnd = rwnd = 1
- Go-Back-N：GBN 协议，swnd = n，rwnd = 1，发送方可以连续发送多个数据包，但只有在接收方确认接收到前一个数据包之后，才会继续发送下一个数据包。如果接收方在某个数据包中出现错误，它会发送一个 NACK（Negative Acknowledgement）信号给发送方，要求发送方重新发送从出错数据包开始之后的所有数据
- Selective-Repeat：SR 协议，swnd = rwnd = n，发送方可以连续发送多个数据包，但接收方只需要确认已经接收到的数据包，没有出错的数据包可以直接被缓存起来，而无需重新传输。当出现丢包时，接收方会发送一个 SACK（Selective Acknowledgement）信号给发送方，告诉它需要重新发送哪些数据包。

> 参考：https://blog.csdn.net/weixin_46935110/article/details/126686631



##### 2.5 TCP 拥塞控制算法

- 慢启动：假定 swnd = cwnd，发送方每接收到一个 ACK，cwnd 就会增加1，这个阶段 cwnd 呈指数式增长
- 拥塞避免：当 cwnd 增长到慢启动门限 slow start threshold 时，使用拥塞避免算法，cwnd 每次加1
- 拥塞发生：如果出现（超时）重传，就说明发生了拥塞，ssthread = cwnd/2，swnd = cwnd = 1
- 快重传和快恢复：如果连续收到 3个重复的 ACK，可以使用快速重传机制，ssthread = cwnd/2，cwnd = ssthread + 3，**直接进入到拥塞避免阶段**

> 参考：https://xiaolincoding.com/network/3_tcp/tcp_feature.html



##### 2.6 流量控制 vs 拥塞控制

- **流量控制是点到点/端到端的控制**，例如A通过网络给B发数据，A发送的太快导致B没法接收(B缓冲窗口过小或者处理过慢)，这时候的控制就是流量控制，原理是通过滑动窗口的大小改变来实现。 
- **拥塞控制全局性的过程**，是A与B之间的网络发生堵塞导致传输过慢或者丢包，来不及传输。防止过多的数据注入到网络中，这样可以使网络中的路由器或链路不至于过载。涉及到所有的主机、路由器，以及与降低网络性能有关的所有因素。



##### 2.7 TCP 如何保证可靠性

- 应用数据被分割成 TCP 认为最适合发送的数据块。
- 校验和：首部和数据的校验和，TCP会丢弃校验和有差错的字段。
- 流量控制：TCP的接收端只允许发送端发送接收端缓冲区能接纳的数据。当接收方来不及处理发送方的数据，能提示发送方降低发送的速率，防止包丢失。
- 拥塞控制
- 停止等待协议：每发完一个分组就- 停止发送，等待对方确认。在收到确认后再发下一个分组。
- 有RTO重传计时器

> 参考：https://cloud.tencent.com/developer/article/1591989



##### 2.8 TCP四个计时器

- 重传计时器：重传丢失的报文段
- 坚持计时器：应付接收端零窗口问题，当发送端TCP收到接收端发来的零窗口通知时，就会启动坚持计时器。当计时器的期限到达时，发送端就会主动发送一个特殊的报文段告诉对方确认已经丢失，必须重新发送
- 保活计时器：解决客户端出现问题的情况，每当服务器端收到客户端的数据时，都将保活计时器重新设置（通常设置为2小时）。过了2小时后，服务器端如果没有收到客户端的数据，会发送探测报文段给客户端，并且每隔75秒发送一个，当连续发送10次以后，仍没有收到对端的来信，则服务器端认为客户端出现故障，并会终止连接
- 时间等待计时器：用于解决服务端发送FIN报文段之后，客户端这边发送ACK丢失的情况

> 参考：[TCP中的四个计时器](https://blog.csdn.net/qq_33951180/article/details/60468267)



#### 3. HTTP相关
##### 3.1 一次完整的HTTP请求过程
![HTTP请求过程](https://img-blog.csdn.net/20180928090731646)

> [域名解析过程-DNS](https://xiaolincoding.com/network/1_base/what_happen_url.html#%E7%9C%9F%E5%AE%9E%E5%9C%B0%E5%9D%80%E6%9F%A5%E8%AF%A2-dns) 
>
> 本地域名服务器请求根域名服务器，然后根域名服务器递归请求顶级域名服务器，顶级域名服务器递归请求权限域名服务器，查到之后原路返回



##### 3.2 常见状态码

- **301-Moved Permanently**：跳转，代表永久性重定向
- **304-Not Modified**：客户端发送附带条件的请求时，服务器端允许请求访问资源，但因发生请求未满足条件的情况后，直接返回了304
- **401-Unauthorized**：发送的请求需要有通过HTTP认证的认证信息
- **403-Forbidden**：对请求资源的访问被服务器拒绝了

> 参考：https://www.runoob.com/http/http-status-codes.html



##### 3.3 简述 HTTPS 的加密与认证过程

首先认证服务器 $\longrightarrow$ 协商会话密钥 $\longrightarrow$ 加密传输

加密有对称密钥和非对称密钥（公钥和私钥）甚至还有混合密钥，认证过程有CA，包含数字签名

> 参考1：[一文看懂https如何保证数据传输的安全性的 - 帅地 - 博客园 (cnblogs.com)](https://www.cnblogs.com/kubidemanong/p/9390021.html)
>
> 参考2：[一个故事看懂HTTPS - 轩辕之风 - 博客园 (cnblogs.com)](https://www.cnblogs.com/xuanyuan/p/15122294.html)



##### 3.4 什么是跨域，什么情况下会发生跨域请求？

- 浏览器从一个域名的网页去请求另一个域名的资源时，域名、端口、协议任一不同，都是跨域
- **在前后端分离的模式下，前后端的域名是不一致的**，此时就会发生跨域访问问题。在请求的过程中我们要想回传数据一般都是 post/get 请求，所以跨域问题出现
- **跨域问题来源于 JavaScript 的同源策略，即只有 协议+主机名+端口号(如存在)相同，则允许相互访问**。也就是说 JavaScript 只能访问和操作自己域下的资源，不能访问和操作其他域下的资源。跨域问题是针对 JS 和 ajax 的，html 本身没有跨域问题，比如 a 标签、script 标签、甚至 form 标签（可以直接跨域发送数据并接收数据）等



##### 3.5 GET 和 POST 的区别

- **GET**：GET请求是可以缓存的，我们可以从浏览器历史记录中查找到GET请求，还可以把它收藏到书签中；且GET请求有长度限制，仅用于请求数据（不修改）。

  > 因GET请求的不安全性，在处理敏感数据时，绝不可以使用GET请求。

- **POST**：用于将数据发送到服务器以创建或更新资源，它要求服务器确认请求中包含的内容作为由URI区分的Web资源的另一个下属。

  > POST请求永远不会被缓存，且对数据长度没有限制；我们无法从浏览器历史记录中查找到POST请求。

> 两者区别参考：https://www.cnblogs.com/logsharing/p/8448446.html
>
> 1. 最直观的区别就是：GET将参数包含在URL中，POST 通过request body传递参数
> 2. GET请求在URL中传递的参数有长度限制，而且参数直接暴露在URL中有危险，POST没有，
> 3. 最大的区别就是：GET产生一个TCP数据包，POST产生两个TCP数据包。具体来说即是对于GET方式的请求，**浏览器会把http header和data一并发送出去，服务器响应200（返回数据）**；而对于POST，**浏览器先发送header，服务器响应100 continue，浏览器再发送data，服务器响应200 ok（返回数据）**。



#### 4. DNS相关

**DNS劫持**

> 那DNS服务器会告诉你A网站的IP是A，B网站的IP是B，那假如你输入的A网站域名，但是他给你B的IP，你是不是就访问到别的网站去了？
>
> 既然DNS服务器可以这么玩，我是不是可以将用户引导到我的网站上？比如你要看个电影，然后你输入了爱奇艺的官网，然后我作为DNS服务器，我把优酷的IP返回给你，最后就是你虽然输入的爱奇艺官网，但是却得到了优酷的IP并访问了优酷的IP，进入了优酷的网站上。
>
> 当然这里我只是举个例子，我举这个例子就是想告诉大家，DNS服务器想给你返回什么IP就给你返回什么，所以他可以在幕后操作一些东西。最简单的就是广告。比如A网站没有广告，你直接访问就是没有的，但是你的DNS服务器把A网站下载下来了，给这个网站加了个广告，然后重新上传到了一个IP上，并把这个IP告诉了你，那就是你虽然输入的A网站的域名，但是你访问的是一个包含了广告的复制版A网站，虽然两者功能一样，但是却完全不是一个服务器上的。
>
> 这些就被我们称为DNS劫持，DNS劫持对于网络访问的影响和体验是非常严重的，除了DNS服务器，你的路由器同样可以这么干，所以在买路由器的时候也有人会关注这个路由器是否会进行DNS劫持。

- 多个client请求问题需要进行ID转换
- 三种类型：本地服务器解析、屏蔽、中继解析
- 通过QR字段来避免多线程复杂编程：QR为0表示请求，QR为1表示响应（而且一定是中继模块）
- 收获：
  - 讨论出**signed char [-128, 128)**  vs **unsigned char [0, 256)**；
  - 解决多个用户同时请求服务器的情况：ID转换，默认时阻塞态



#### 5. OSI和TCP/IP模型

![img](https://img-blog.csdn.net/20130909165027546)



#### 6. 简述对称与非对称加密的概念

对称加密使用对称密钥（共享密钥），非对称加密使用（公钥和私钥）

---



### 📄数据库📄

#### 1. 基本概念

##### 1.1. 事务定义

事务指的是满足 ACID 特性的一组操作，可以通过 Commit 提交一个事务，也可以使用 Rollback 进行回滚

##### 1.2. <span id="2">并发一致性</span>

- 丢失修改就是写覆盖

  > 一级封锁协议，写的时候加锁，T结束才释放

- 读脏数据就是读取本来要回滚的数据

  > 二级封锁协议，读的时候加S锁，读完就释放

- 不可重复读就是由于第二次读取别的事务写之后的数据，而导致两次读取到不同数据

  > 三级封锁协议，读取数据加S锁，直到事务结束才释放

- 幻影读本质就是不可重复读

##### 1.3. 封锁粒度

MySQL 中提供了两种封锁粒度：行级锁以及表级锁。

应该尽量**只锁定需要修改的那部分数据**，而不是所有的资源。锁定的数据量越少，**发生锁争用的可能就越小，系统的并发程度就越高**。

但是加锁需要消耗资源，锁的各种操作（包括获取锁、释放锁、以及检查锁状态）都会增加系统开销。因此**封锁粒度越小，系统开销就越大**。

> 行锁的粒度小，加锁慢（锁住某一行），发生锁冲突的概率小；表锁的粒度大，加锁快（直接锁住整个表），发生锁冲突的概率大，行锁也是 MyISAM 和 InnoDB 的区别之一，InnoDB 支持行锁并且支持事务。

在选择封锁粒度时，需要**在锁开销和并发程度之间做一个权衡**。

> **可串行化调度**是指，通过并发控制，使得并发执行的事务结果与某个串行执行的事务结果相同。串行执行的事务互不干扰，不会出现并发一致性问题。
>
> 加锁和解锁分为两个阶段进行  ==>  冲突可串行化
>
> MySQL 的 InnoDB 存储引擎采用两段锁协议
>
> [意向锁的理解](https://www.zhihu.com/question/51513268)

##### 1.4. <span id='1'>隔离级别</span>

- **未提交读（READ UNCOMMITTED）**：事务中的修改，即使没有提交，对其它事务也是可见的。**可以保证读取没有损坏的数据**

- **提交读（READ COMMITTED）**：一个事务只能读取已经提交的事务所做的修改。换句话说，一个事务所做的修改在提交之前对其它事务是不可见的。**可以避免脏读**

- **可重复读（REPEATABLE READ）**：保证在同一个事务中多次读取同一数据的结果是一样的。**可以避免不可重复读**

  > REPEATABLE READ 是 MySQL 中默认的隔离级别，也可以解决幻影读

- **可串行化（SERIALIZABLE）**：强制事务串行执行，这样多个事务互不干扰，**不会出现并发一致性问题**。该隔离级别需要加锁实现，因为要使用加锁机制保证同一时间只有一个事务执行，也就是保证事务串行执行。

> 这四大等级从上到下，隔离的效果是逐渐增强，但是性能却是越来越差。
>
> 参考：https://www.nowcoder.com/discuss/465462788371087360

##### 1.5. 多版本并发控制

Multi-Version Concurrency Control（MVCC）是 `MySQL` 的 `InnoDB `存储引擎实现隔离级别的一种具体方式，**用于实现提交读和可重复读这两种隔离级别。而未提交读隔离级别总是读取最新的数据行，要求很低，无需使用 MVCC**。可串行化隔离级别需要对所有读取的行都加锁，单纯使用 MVCC 无法实现。

> ##### 版本号
>
> - 系统版本号 SYS_ID：是一个递增的数字，每开始一个新的事务，系统版本号就会自动递增。
> - 事务版本号 TRX_ID ：事务开始时的系统版本号。
>
> Undo日志
>
> ReadView…

##### 1.6. Next-Key Locks

Next-Key Locks 是 MySQL 的 InnoDB 存储引擎的一种锁实现。

MVCC 不能解决幻影读问题，Next-Key Locks 就是为了解决这个问题而存在的。在可重复读（REPEATABLE READ）隔离级别下，使用 MVCC + Next-Key Locks 可以解决幻读问题。

参考：https://juejin.cn/post/7018137095315128328

##### 1.7. 关系型数据库设计理论 && ER图

- 函数依赖：A --> B，A 决定 B，B 依赖于 A
- 异常：冗余、修改异常、删除异常、插入异常
- 范式：第一范式（属性不可分）、第二范式（每个非主属性完全函数依赖于键码）、第三范式（非主属性不传递函数依赖于键码）

参考：https://www.cnblogs.com/caiyishuai/p/10975736.html

##### 1.8. MySQL

- 查询性能优化：使用`explain`分析
  - 优化数据访问：减少请求的数据量、减少服务端扫描行数
  - 重构查询方式：切分大查询，分解大连接查询
  
- 存储引擎
  - InnoDB：事务型，支持外键和在线热备份
  - MyISAM：崩溃损坏频率高，恢复速度慢
  
  > [MyISAM与InnoDB 的区别（9个不同点）](https://blog.csdn.net/qq_35642036/article/details/82820178)
  
- 切分
  - 水平切分：将同一个表中的记录拆分到多个结构相同的表中，缓存单个数据库的压力
  - 垂直区分：将一张表按列切分成多个表，通常是按照列的关系密集程度进行切分，也可以利用垂直切分将经常被使用的列和不经常被使用的列切分到不同的表中
  
- 主从复制

  - **binlog线程**：将主服务器数据更改写入二进制日志`Binary log`
  - **I/O线程**：从主服务器上读取二进制日志，写入从服务器的中继日志
  - **SQL 线程** ：读取中继日志，解析出主服务器已经执行的数据更改并在从服务器中重放

- 读写分离

  - 主服务器处理写操作以及实时性比较高的读操作，而从服务器处理读操作

  - 提高性能原因如下：
    - 主从服务器负责各自的读和写，极大程度缓解了锁的争用
    - 从服务器可以使用 MyISAM，提升查询性能以及节约系统开销
    - 增加冗余，提高可用性
    

##### 1.9. 主键和外键

- 主键：能确定一条记录的唯一标识，比如，一条记录包括身份正号，姓名，年龄。身份证号是唯一能确定你这个人的，其他都可能有重复，所以，身份证号是主键
- 外键：用于与另一张表的关联。是能确定另一张表记录的字段，用于保持数据的一致性。比如，A表中的一个字段，是B表的主键，那他就可以是A表的外键

> 参考：https://blog.csdn.net/fengzongfu/article/details/78820485



#### 2. MySQL 为什么使用 B+ 树来作索引，对比 B 树它的优点和缺点是什么？

AVL树适合用于插入删除次数比较少，但查找多的情况。所以对于搜索、插入、删除操作多的情况下，我们就用红黑树。

稳定、更快（树的高度小）

- **B+树的磁盘读写代价更低**：B树的每个节点都存储了key和data，而B+树的data存储在叶子节点上。B+树非叶子节点仅存储key不存储data，这样一个节点就可以存储更多的key。可以使得B+树相对B树来说更矮（IO次数就是树的高度），所以与磁盘交换的IO操作次数更少。
- **B+树的查询效率更加稳定**：由于非终结点并不是最终指向文件内容的结点，而只是叶子结点中关键字的索引。所以任何关键字的查找必须走一条从根结点到叶子结点的路。所有关键字查询的路径长度相同，导致每一个数据的查询效率相当。

> 参考：https://blog.csdn.net/qq_37102984/article/details/119646611



####  3. 数据库的事务隔离级别有哪些？各有哪些优缺点

[隔离级别](#1)



#### 4. 关于Redis

==ToDo==



#### 5. 简述乐观锁以及悲观锁的区别以及使用场景

- 乐观锁总是认为不会产生并发问题，每次去取数据的时候总认为不会有其他线程对数据进行修改，因此不会上锁，但是在更新时会判断其他线程在这之前有没有对数据进行修改，一般会使用版本号机制或CAS操作实现
  - 工作方式：全程没有加锁（**无锁编程**），先修改完共享资源，再验证这段时间内有没有发生冲突，如果没有其他线程在修改资源，那么操作完成，如果发现有其他线程已经修改过这个资源，就放弃本次操作

  - 应用场景：比较适合**读取操作比较频繁**的场景，如果出现大量的写入操作，数据发生冲突的可能性就会增大，为了保证数据的一致性，应用层需要不断的重新获取数据，这样会增加大量的查询操作，降低了系统的吞吐量。

- 悲观锁总是假设最坏的情况，每次取数据时都认为其他线程会修改，所以都会加锁（读锁、写锁、行锁等），当其他线程想要访问数据时，都需要阻塞挂起
  - 工作方式：可以依靠数据库实现，如行锁、读锁和写锁等，都是在操作之前加锁
  - 应用场景：比较适合**写入操作比较频繁**的场景，如果出现大量的读取操作，每次读取的时候都会进行加锁，这样会增加大量的锁的开销，降低了系统的吞吐量。




#### 6. 数据库有哪些常见索引？数据库设计的范式是什么？

- 密集索引和稀疏索引：MyISAM全是稀疏索引，InnoDB有且只有一个密集索引

- 聚集索引和非聚集索引

  - 聚集索引：数据行的物理顺序与列值（一般是主键的那一列）的逻辑顺序相同，一个表中只能拥有一个聚集索引。**索引的叶子节点就是对应的数据节点**

  - 非聚集索引：该索引中索引的逻辑顺序与磁盘上行的物理存储顺序不同，一个表中可以拥有多个非聚集索引。[涉及到二次查询](https://www.cnblogs.com/s-b-b/p/8334593.html)：**非聚集索引叶节点仍然是索引节点，只是有一个指针指向对应的数据块，此如果使用非聚集索引查询，而查询列中包含了其他该索引没有覆盖的列，那么他还要进行第二次的查询，查询节点上对应的数据行的数据**



#### 7. 什么情况下会发生死锁，如何解决死锁

如果线程A锁住了记录1并等待记录2，而线程B锁住了记录2并等待记录1，这样两个线程就发生了死锁现象

- **主要原因**：系统资源不足； 进程运行推进的顺序不合适；资源分配不当等。
- **避免死锁**：破坏出现死锁的4个必要条件中的某一个：不让线程循环等待



#### 8. 并发事务会引发哪些问题？如何解决

- [并发一致性](#2)问题：会产生脏读（读到还没有提交的数据）、不可重复读（两次读取数据不一致）、幻影读、丢失修改
- 解决：[隔离级别](#1)



#### 9. MySQL 中 join 与 left join 的区别是什么

https://zhuanlan.zhihu.com/p/45338392



#### 10. MySQL三大日志以及使用场景

- **binlog**：`binlog`用于记录数据库执行的写入性操作(不包括查询)信息，以二进制的形式保存在磁盘中。`binlog`是`mysql`的逻辑日志，并且由`Server`层进行记录，使用任何存储引擎的`mysql`数据库都会记录`binlog`日志

  >在实际应用中，`binlog`的主要使用场景有两个，分别是**主从复制**和**数据恢复**。

- **Redo log**：具体来说就是只记录事务对数据页做了哪些修改，可以实现「持久性」

- **Undo log**：「原子性」底层就是通过 `undo log` 实现的。`undo log`主要记录了数据的逻辑变化，比如一条 `INSERT` 语句，对应一条`DELETE` 的 `undo log` ，对于每个 `UPDATE` 语句，对应一条相反的 `UPDATE` 的 `undo log` ，这样在发生错误时，就能回滚到事务之前的数据状态。



#### 11. 主从复制和读写分离

主从复制涉及的三个线程：binlog 线程、I/O 线程 和 SQL 线程

- binlog 线程：负责将主服务器上的数据更改写入二进制日志（Binary log）中
- I/O 线程：负责从主服务器上读取二进制日志，并写入从服务器的中继日志（Relay log）
- SQL 线程：负责读取中继日志，解析出主服务器已经执行的数据更改并在服务器中重放（Replay）

读写分离：主服务器处理写操作以及实时性要求比较高的读操作，而从服务器处理读操作，读写分离能提高性能的原因在于：

- 主从服务器负责负责各自的读和写，极大程度缓解了锁的争用
- 从服务器可以使用 MyISAM 引擎，提升查询性能以及节约系统开销
- 增加冗余，提高可用性


***



### 🌈C++🌈

#### 1. 简述 C++ 右值引用与转移语义

右值引用 (Rvalue Referene) 是 C++11 新标准中引入的新特性 , 它实现了**转移语义** (Move Sementics) 和**精确传递** (Perfect Forwarding)。它的主要目的有两个方面：

- **消除两个对象交互时不必要的对象拷贝，节省运算存储资源，提高效率**
- **能够更简洁明确地定义泛型函数**

简单来说，就是右值引用是用来支持转移语义的，转移语义可以将资源（堆、系统对象等）从一个对象转移到另一个对象，**这样能够减少不必要的临时对象的创建、拷贝以及销毁，能够大幅度提高 C++ 应用程序的性能**

在现有的 C++ 机制中，我们可以定义拷贝构造函数和赋值函数。要实现转移语义，需要定义**转移构造函数**，还可以定义**转移赋值操作符**。对于右值的拷贝和赋值会调用转移构造函数和转移赋值操作符。**如果转移构造函数和转移拷贝操作符没有定义，那么就遵循现有的机制，拷贝构造函数和赋值操作符会被调用。**

> 标准库函数**move**可以将一个左值引用**强制转换**成右值引用来使用，相当于 static_cast
>
> 参考：
>
> - [[C++]右值引用和转移语义_stary_yan的博客-CSDN博客](https://blog.csdn.net/stary_yan/article/details/51284929)
> - [C++ 移动构造函数详解](https://blog.csdn.net/weixin_44788542/article/details/126284429)



#### 2. const、static 关键字有什么区别

**首先明白内存的五个区**

- **栈(stack)**：由编译器自动分配释放，存放函数的参数值，局部变量的值（除static），其操作方式类似于数据结构中的栈。
- **堆(heap)**：一般由程序员分配释放，若程序员不释放，程序结束时可能由OS回收。注意它与数据结构中的堆(优先队列)是两回事，**分配方式倒是类似于链表**。
- **全局区(静态区)**：全局变量和静态变量被分配到同一块内存中，在以前的C语言中，全局变量和静态变量又分为初始化的和未初始化的，在C++里面没有这个区分了，它们共同占用同一块内存区，在该区定义的变量若没有初始化，则会被自动初始化，例如int型变量自动初始为0，**程序结束后由系统释放**。
- **文字常量区**：常量字符串就是放在这里的，如char str[]=”hello”，程序结束后由系统释放，区别const修饰的变量。
- **程序代码区**：存放函数体的二进制代码。

1. static 声明的变量内存只分配一次，static 变量存放在静态区，程序结束后才会释放；该变量（static）只被分配一次，因此它的值在下一次调用时依然可以维持上次结果的值。
   - static数据成员必须进行初始化，默认初始化为0
   - 模块内声明的static函数，可以被该模块内的函数进行调用，模块外的函数不可调用它
2. const 修饰的变量可以防止被改变，初始化后，就没有机会再去改变它。这样可以阻止一个变量被改变。
   - const 定义的常量在超出其作用域之后其空间会被释放，而static定义的静态常量在函数执行后不会释放其存储空间，程序结束之后才会释放。
   - const 变量存储位置以及如何改变：const变量的内存位于**栈区或者静态存储区**，不在符号表(常量表)中，[参考](https://blog.csdn.net/qq_43152052/article/details/99306967)
   - clang 环境下测试好像无法修改栈区的 const 变量，但是可以修改栈区的 const volatile 变量，编译运行都不会报错，[volatile](https://github.com/EricPengShuai/Interview/blob/main/C_C%2B%2B.md#39volatile-mutabe-explicit-%E5%85%B3%E9%94%AE%E5%AD%97%E7%9A%84%E7%94%A8%E6%B3%95)
   - clang 环境下测试好像无法修改静态区的 const 变量以及 const volatile 变量，编译不会报错，但是运行会报 bus error 错误



#### 3. C++的多态是如何实现的？

1. 函数重载：形参个数或者类型不同
2. 运算符重载：比如可以实现字符串相加
3. 虚函数：使用关键字`virtual`声明，在基类中申明一些能够在派生类中重新定义的函数



#### 4. C++ 中解释类模板和模板类的区别

1. **一个类模板(类生成类)允许用户为类定义个一种模式**，使得类中的某些数据成员、默认成员函数的参数，某些成员函数的返回值，能够取任意类型(包括系统预定义的和用户自定义的)。
   - 只能在头文件中定义
   - 外部定义函数需要加上模板声明
2. **模板类是类模板实例化后的一个产物**，类模板比作是一个作饺子的模子，而模板类就是用这个模子做出来的饺子，至于饺子什么馅儿的就需要你自己去实例化自己的内容

> 类模板：https://blog.csdn.net/weixin_30907935/article/details/95182154
>
> 类模板代码例子：https://blog.csdn.net/small_prince_/article/details/80533105



#### 5. C++ 中哪些函数不能被声明为虚函数？

- 普通函数（非类成员函数）
- 构造函数：**虚函数指针指向虚函数地址，vptr指针在构造函数时候被初始化，而调用虚函数的前提是有vptr指针**  [参考](https://blog.csdn.net/gochenguowei/article/details/79682681)
- 内联函数
- static成员函数不能定义为虚函数：**静态成员函数没有this指针，没办法调用vptr指针**
- 友元函数

>  **虚函数的调用关系：this --> vptr --> vtable -->virtual function**



#### 6. C++ 中虚函数与纯虚函数的区别

- 纯虚函数只有定义，没有实现；而虚函数既有定义，也有实现的代码

  ```cpp
  // 存虚函数一般没有代码实现部分
  virtual void print() = 0;
  
  // 虚函数必须要有代码实现部分
  virtual void print() { 
  	printf("This is virtual function\n");
  }
  ```
  
- 包含纯虚函数的类不能定义其对象，而包含虚函数的则可以



#### 7. 简述 C++ 中内存对齐的使用场景

- 结构(struct) 或 联合(union) 的数据成员，第一个数据成员放在offset为0的地方，以后每个数据成员存储的起始位置要从该成员大小或者成员的子成员大小（只要该成员有子成员，比如说是数组，结构体等）的整数倍开始

- 结构体作为成员: 如果一个结构里有某些结构体成员,则结构体成员要从其内部**最宽基本类型成员**的整数倍地址开始存储

  > struct a里存有struct b, b里有 char, int, double 等元素,那b应该从8的整数倍开始存储

**内存对齐原因**：

1. 平台原因(移植原因)：不是所有的硬件平台都能访问任意地址上的任意数据的；某些硬件平台只能在某些地址处取某些特定类型的数据，否则抛出硬件异常。
1. 性能原因：经过内存对齐后，CPU的内存访问速度大大提升



#### 8. 构造函数为什么不能被声明为虚函数？

1. **构造一个对象的时候，必须知道对象的实际类型**，而虚函数行为是在运行期间确定实际类型的。而在构造一个对象时，由于对象还未构造成功。编译器无法知道对象的实际类型，是该类本身，还是该类的一个派生类，或是更深层次的派生类。无法确定。

2. 虚函数的执行依赖于虚函数表。而虚函数表在构造函数中进行初始化工作，即初始化`vptr`，让他指向正确的虚函数表。而在构造对象期间，虚函数表还没有被初始化，将无法进行。

> 虚函数对应一个vtable，可是这个vtable其实是存储在对象的内存空间的。问题出来了，如果构造函数是虚的，就需要通过 vtable来调用，**可是对象还没有实例化，也就是内存空间还没有，怎么找vtable呢？** 所以构造函数不能是虚函数。



#### 9.  什么是内存泄漏，怎么确定内存泄漏？

- **内存泄漏**：内存泄漏是指由于疏忽或错误造成了程序未能释放掉不再使用的内存的情况。内存泄漏并非指内存在物理上消失，而是应用程序分配某段内存后，由于设计错误，失去了对该段内存的控制；

- **[三种常见情况](https://blog.csdn.net/weixin_44718794/article/details/107071169)**：

  - 指针的错误赋值
  - 错误的内存释放，先释放父指针导致子指针无法释放
  - 函数返回地址之后没有处理导致分配的内存块丢失

- **如何确定**：

  - 检查方法：在main函数最后面一行，加上一句`_CrtDumpMemoryLeaks()`。调试程序，自然关闭程序让其退出，查看输出：输出这样的格式`{453}normal block at 0x02432CA8,868 bytes long`，被`{}`包围的`453`就是我们需要的内存泄漏定位值，`868 bytes long`就是说这个地方有868比特内存没有释放。

  - 定位代码位置：在`main`函数第一行加上`_CrtSetBreakAlloc(453)`；**意思就是在申请453这块内存的位置中断。然后调试程序，程序中断了，查看调用堆栈。** 注意加上头文件`#include <crtdbg.h>`
  
- **检漏方法？**

  > 对象计数方法：**在对象构造时计数++，析构时--**，每隔一段时间打印对象的数量
  >
  > 重载new和delete方法：**重载new/delete，记录分配点（甚至是调用堆栈），定期打印。**



#### 10. hash_map和map区别

- 构造函数：**hash_map需要hash函数，等于函数**；map只需要比较函数(小于函数)
- 存储结构：**hash_map采用hash表存储**，map一般采用红黑树(RB Tree)实现。因此其memory数据结构是不一样的



#### 11. 如何让类对象只在栈（堆）上分配空间

首先明白类对象建立的两种方式：静态建立，如 A a ；动态建立，如 A* ptr = new A()

- 「静态建立类对象」是由编译器为对象在栈空间中分配内存，是通过直接移动栈顶指针，挪出适当的空间，然后在这片内存空间上调用构造函数形成一个栈对象。使用这种方法，**直接调用类的构造函数**。
- 「动态建立类对象」 是使用new运算符将对象建立在堆空间中。这个过程分为两步，第一步是执行operator new()函数，在堆空间中搜索合适的内存并进行分配；第二步是调用构造函数构造对象，初始化这片内存空间。这种方法，**间接调用类的构造函数**。



**限制只能在堆上分配空间**：将析构函数私有化，这样静态建立就失败了，只能动态建立，类中必须提供一个`destory`函数，来进行内存空间的释放。类对象使用完成后，必须调用`destory`函数。

- 无法解决继承问题，私有化之后子类无法访问，还好 protected 化可以缓解一下
- 类的使用很不方便，无法使用 delete，必须要调用 destroy 函数，很怪异

**限制只能在栈上分配空间**：将「new 运算符私有化」，由于 new 运算符总是通过调用 operator new() 函数分配内存，所以可以私有化这个函数，[参考](https://segmentfault.com/a/1190000009023942)

---



### :star2: 算法:star2:

| 题目                                                         | 说明                                                         | 题解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [21. 合并两个有序链表](https://leetcode.cn/problems/merge-two-sorted-lists/) | [递归](https://leetcode.cn/submissions/detail/122687963/)注意出口，迭代使用一个 dummy 节点 | [通过](https://leetcode.cn/submissions/detail/337567705/)    |
| [215. 数组中的第K个最大元素](https://leetcode-cn.com/problems/kth-largest-element-in-an-array/) | 掌握[快速排序](https://github.com/EricPengShuai/Interview/blob/main/algorithm/%E6%8E%92%E5%BA%8F.md#12-%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F)，借用 partition 的思想递归求得第 k 大元素，最小堆也可 | [快排](https://leetcode.cn/submissions/detail/399440993/) [小顶堆](https://leetcode.cn/submissions/detail/399454576/) |
| [445. 两数相加 II](https://leetcode.cn/problems/add-two-numbers-ii/) | 链表元素入栈，然后弹出过程就相当于从后面相加，**注意进位**   | [通过](https://leetcode.cn/submissions/detail/337603420/)    |
| [33. 搜索旋转排序数组](https://leetcode.cn/problems/search-in-rotated-sorted-array/) | 执行 `nums[mid] >= nums[left] ? 左边有序 : 右边有序`，然后二分，注意这里最好 `while(left <= right)` 并且内部直接判断 :fire: | [通过](https://leetcode.cn/submissions/detail/399463997/)    |
| [81. 搜索旋转排序数组 II](https://leetcode.cn/problems/search-in-rotated-sorted-array-ii/) | 在 LC.33 基础上通过 while 比较临近元素去重                   | [通过](https://leetcode.cn/submissions/detail/399466035/)    |
| [300. 最长递增子序列](https://leetcode.cn/problems/longest-increasing-subsequence/) | 常规 DP 复杂度 O(N^2)，贪心+二分 O(NlogN)                    | [LIS](https://github.com/EricPengShuai/Interview/blob/main/algorithm/%E6%9C%80%E9%95%BF%E4%B8%8A%E5%8D%87%E5%AD%90%E5%BA%8F%E5%88%97.md) |
| [剑指 Offer 11. 旋转数组的最小数字](https://leetcode.cn/problems/xuan-zhuan-shu-zu-de-zui-xiao-shu-zi-lcof/) | 只能比较 nums[mid] > nums[right] 得出 mid 在哪边排序数组中 :fire: | [K神](https://leetcode.cn/problems/xuan-zhuan-shu-zu-de-zui-xiao-shu-zi-lcof/solution/mian-shi-ti-11-xuan-zhuan-shu-zu-de-zui-xiao-shu-3/) |
| [剑指 Offer 42. 连续子数组的最大和](https://leetcode.cn/problems/lian-xu-zi-shu-zu-de-zui-da-he-lcof/) | 很常规的 DP，另外可以使用两个变量压缩空间                    | [通过](https://leetcode.cn/submissions/detail/126954195/)    |
| [25. K 个一组翻转链表](https://leetcode.cn/problems/reverse-nodes-in-k-group/) | [递归](https://leetcode.cn/submissions/detail/399578801/)比较繁琐，每 k 个节点入栈比较清晰，但是也要注意细节，比为尾结点之后为空 | [通过](https://leetcode.cn/submissions/detail/399583186/)    |
| [剑指 Offer 68 - II. 二叉树的最近公共祖先](https://leetcode.cn/problems/er-cha-shu-de-zui-jin-gong-gong-zu-xian-lcof/) | 在 root 的左右子树上面递归，注意返回的条件                   | [通过](https://leetcode.cn/submissions/detail/377104965/)    |
| [155. 最小栈](https://leetcode.cn/problems/min-stack/)       | 两个栈：一个保存数据，一个保存当前最小值，也可以一个`stack<pair<int, int>>` | [通过](https://leetcode.cn/submissions/detail/170474366/)    |
| [264. 丑数 II](https://leetcode.cn/problems/ugly-number-ii/) | 暴力，`set<long>`，三指针                                    | [通过](https://leetcode.cn/submissions/detail/399607399/)    |
| [112. 路径总和](https://leetcode.cn/problems/path-sum/)      | 常规 DFS，[BFS](https://leetcode.cn/submissions/detail/123368565/) 都可以 | [通过](https://leetcode.cn/submissions/detail/358318248/)    |
| [113. 路径总和 II](https://leetcode.cn/problems/path-sum-ii/) | DFS 回溯写法，BFS 需要弄一个记录父节点的哈希表得到路径       | [BFS](https://leetcode.cn/submissions/detail/399616187/) [DFS](https://leetcode.cn/submissions/detail/358323283/) |
| [103. 二叉树的锯齿形层序遍历](https://leetcode.cn/problems/binary-tree-zigzag-level-order-traversal/) | 很常规的 BFS，注意下标处理                                   | [通过](https://leetcode.cn/submissions/detail/399617939/)    |
| [189. 轮转数组](https://leetcode.cn/problems/rotate-array/)  | 三次翻转就可以，可以手写 reverse，也可以直接调库             | [通过](https://leetcode.cn/submissions/detail/338099191/)    |
| [剑指 Offer 48. 最长不含重复字符的子字符串](https://leetcode.cn/problems/zui-chang-bu-han-zhong-fu-zi-fu-de-zi-zi-fu-chuan-lcof/) | DP，哈希表记录字符最后一次出现的位置                         | [通过](https://leetcode.cn/submissions/detail/370443446/)    |
| [141. 环形链表](https://leetcode.cn/problems/linked-list-cycle/) | 双指针，注意细节，快指针指向头的下一个位置                   | [通过](https://leetcode.cn/submissions/detail/399627788/)    |
| [142. 环形链表 II](https://leetcode.cn/problems/linked-list-cycle-ii/) | fast = slow * 2, fast = slow + n*b, 走a+nb步一定是在环入口   | [通过](https://leetcode.cn/submissions/detail/174241823/)    |

---



### :dizzy:智力题:dizzy:

#### 1. 10亿个数中如何高效地找到最大的一个数以及最大的第 K 个数

- 第三种方法是分治法，将10亿个数据分成100份，每份100万个数据，找到每份数据中最大的10000个，**最后在剩下的100\*10000个数据里面找出最大的10000个**。如果100万数据选择足够理想，那么可以过滤掉1亿数据里面99%的数据。

  - **100万个数据里面查找最大的10000个数据的方法如下：用快速排序的方法**，将数据分为2堆，如果大的那堆个数N大于10000个，继续对大堆快速排序一次分成2堆，如果大的那堆个数N大于10000个，继续对大堆快速排序一次分成2堆，如果大堆个数N小于10000个，就在小的那堆里面快速排序一次，找第10000-n大的数字；
  - **递归以上过程**，就可以找到第1w大的数。参考上面的找出第1w大数字，就可以类似的方法找到前10000大数字了。此种方法需要每次的内存空间为10^6*4=4MB，一共需要101次这样的比较，参考：[215. 数组中的第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array/)

- 第五种方法采用**最小堆。首先读入前10000个数来创建大小为10000的最小堆**，建堆的时间复杂度为O(m)（m为数组的大小即为10000），然后遍历后续的数字，并与堆顶（最小）数字进行比较。**如果比最小的数小，则继续读取后续数字；如果比堆顶数字大，则替换堆顶元素并重新调整堆为最小堆**。整个过程直至1亿个数全部遍历完为止。然后按照中序遍历的方式输出当前堆中的所有10000个数字。该算法的时间复杂度为O(nlogm)，空间复杂度是10000（常数）。

  > 建堆时间复杂度是O(mlogm)，堆调整的时间复杂度是O(logm) ，堆排序的时间复杂度为 O(mlogm)
  >
  > 本题最终时间复杂度等于1次建堆时间+n次堆调整时间 = O(mlogm+nlogm)



#### 2. 给定 100G 的 URL 磁盘数据，使用最多 1G 内存，统计出现频率最高的 Top K 个 URL

1. **分文件**（在外存中进行）顺序读文件中，对于每个词x，取hash(x)%100，然后按照该值存到100个小文件（记为x0,x1,...x99）中。这样每个文件大概是1G左右。（**相同的单词一定出现的同一个文件中**）

2. **文件内排序**（内存中）：对每个小文件，统计每个文件中出现的词以及相应的频率（可以采用trie树/[hash_map](https://www.cnblogs.com/leebxo/p/10834933.html)等），**并取出出现频率最大的100个词**（**可以用含100个结点的最小堆**），并把100个词及相应的频率存入文件，这样又得到了100个文件。

3. **归并**：下一步就是把这100个文件进行归并（类似与归并排序）的过程了。

> 参考：https://wizardforcel.gitbooks.io/the-art-of-programming-by-july/content/06.02.html



#### 3. 64匹马8个跑道需要多少轮才能选出最快的四匹

1. 首先分8组，每组8匹马，跑8轮，淘汰每组的后4名（这样还剩32匹）
2. 将之前8个组的第一名拿出来继续比，淘汰后4名马所在组的所有马（这样还剩16匹）
3. 目前可以得到跑的最快的马，*然后淘汰一些*，得到9匹马
4. 最后选择除开$B_2$的其他8匹马比一下

> 最少10轮，最多11轮
>
> 参考：https://blog.csdn.net/u013095333/article/details/99718233



#### 4. 给定一个 foo 函数，60%的概率返回0，40%的概率返回1，如何利用 foo 函数实现一个 50% 返回 0 或 1 的函数

```c++
// 调用foo两次即可，连续两次，出现 0 1 和 1 0 的概率是一样的（24%*2=48%)
// 于是就能构造出50%（两次返回 0 0 或 1 1的结果丢掉，重新调用）（36%+16%=52%）
int half01() 
{
    while(1) {
        int a = foo();
        int b = foo();

        if(a != b) return a;
    }
}
```

> 有个函数foo，返回0和1的概率都是50%，让你自己写一个函数，使返回0和1的概率分别是30%和70%。
>
> 这道题乍一看不知道咋下手，其实还很简单的，相当于利用了遗传算法的思想，每次产生一个数字，然后以为，一共产生四个数字，这样就可以得到一个四位的数字，**即`[0,15]`，只要这个数字大于等于10就抛弃，这样产生`0~9`的数字，而且`0~9`的数字出现的概率是随机且相等的，这样，`0~2`返回0, `3~9`返回1，这样，就满足了题目要求**

------



### 项目相关

#### 1. [装饰器](https://www.runoob.com/w3cnote/python-func-decorators.html)

- 装饰器本质上是一个 Python 函数或类，它可以让其他函数或类在不需要做任何代码修改的前提下增加额外功能，装饰器的返回值也是**一个函数/类对象**。
- 它经常用于有切面需求的场景，比如：插入日志、性能测试、事务处理、缓存、权限校验等场景，装饰器是解决这类问题的绝佳设计。

> 有了装饰器，我们就可以抽离出大量与函数功能本身无关的雷同代码到装饰器中并继续重用(**较少重复代码**)。概括的讲，装饰器的作用就是为已经存在的对象添加额外的功能。
>
> - 语法糖
> - 例子：/Users/pengshuai/Documents/python/装饰器/ex1.py

> **flask框架路由实现原理是什么**
>
> 使用`app.route('url')`就相当于使用装饰器，@是一个语法糖
>
> 1. 通过装饰器调用`self.add_url_rule(rule, endpoint, f, **options)`
>
> 2. 参数处理完之后，将url规则写入url_map（创建好Rule对象，并添加到Map对象中，`werkzeug.routing:Map/Rule类`），将视图函数写入view_function字典中。
>
>    > **Flask的核心路由逻辑是在werkzeug中实现的**
>
> 3. 通过`wsgi_app`进行路由匹配：创建 RequestContext 对象，match_request 方法，最后调用`full_dispatch_request()`（重点执行`dispatch_request()`）
>
> https://www.cnblogs.com/ybjourney/p/11789983.html



#### 2. WSGI

全称 Python Web Server Gateway Interface，指定了 web 服务器和 Python web 应用或 web 框架之间的标准接口，以提高web应用在一系列web服务器间的移植性。



#### 3. 哈希表

- 构造：**① 直接定址法；②平方取中法；③折叠法；④除留取余法**
- 冲突解决：**① 开放定址法（线性探测）② 链地址法**
- 优点：能够在常数级的时间复杂度上进行查找，并且插入数据和删除数据比较容易
- 缺点：比如不支持排序，一般比用线性表存储需要更多的空间