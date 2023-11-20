## 蚂蚁集团

### CTO线—计算智能技术部

> 2023.03.21—19:00—1h20m

#### 自我介绍

##### 1、自我介绍，5 min

##### 2、项目介绍，10min



#### 八股和算法

##### 1、TCP 拆包和粘包

拆包是一个完整的业务可能会被TCP拆分成多个包进行发送； 粘包是把多个小的包封装成一个大的数据包发送

**原因**

- 应用程序写入数据的字节大小大于套接字发送缓冲区的大小

- 进行 MSS 大小的 TCP 分段 ( MSS = TCP报文段长度 - TCP首部长度)

- 以太网的 payload 大于 MTU 进行 IP分片（ MTU指：一种通信协议的某一层上面所能通过的最大数据包大小）

**解决**

- 消息定长
- 在包尾部增加回车或者空格符等特殊字符进行分割
- 将消息分为消息头和消息尾
- 使用其它复杂的协议，如RTMP协议等



##### 2、Linux 中的 cache 到内存是如何映射的

- 全相联映射：每个内存块数据可以映射到**任意**缓存行中，利用率高，速度较慢，内存地址：[标记位|块内地址]
- 直接映射：每个内存块数据只映射到**固定**的缓存行中，利用率低，速度较快，内存地址：[标记位|cache行号|块内地址]
- 组相联映射：每个内存块数据可以映射到**固定组**的**任意**缓存行中，利用率较高，速度也还行，内存地址：[标记位|组内地址|块内地址]

> 参考：http://kaito-kidd.com/2018/08/23/computer-system-cpu-cache/



##### 3、fork 函数

父进程返回值>0，子进程返回值=0，失败时=-1

##### 4、IO 复用操作 :fire:

select、poll 和 epoll [参考](http://www.cyc2018.xyz/%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%9F%BA%E7%A1%80/Socket/Socket.html)

##### 5、memory map 是干啥的



##### 6、锁底层实现

锁是一种用于多线程编程的同步机制，用于确保在多个线程同时访问共享资源时，每次只有一个线程能够访问资源。底层实现原理可以分为两种：硬件锁和软件锁。

- **硬件锁**是通过CPU的支持来实现的。当一个线程正在访问共享资源时，CPU会禁止其他线程访问该资源，直到该线程释放锁。硬件锁的实现非常高效，但需要硬件支持。
- **软件锁**是通过在操作系统内核中实现的。当一个线程需要访问共享资源时，它会向内核请求锁。如果锁已经被另一个线程持有，则该线程会被阻塞，直到锁被释放。软件锁的实现比硬件锁更加灵活，但是由于需要进行系统调用，所以开销比硬件锁更大。

除了基本的锁实现之外，还有一些高级锁的实现，如读写锁、自旋锁和条件变量等。这些锁的实现原理略有不同，但本质上都是通过协调不同线程之间的访问来确保共享资源的正确性。



##### 7、如何自己实现一个智能指针类

引用计数变量维护需要加锁吗，或者说 shared_ptr 是线程安全的吗

首先明确线程安全的定义：多线程操作一个共享数据的时候，能够保证所有线程的行为是符合预期的。

**对于引用计数的探讨**

```cpp
shared_ptr<A> sp1 = make_shared<A>();
shared_ptr<A> sp2 = sp1;
shared_ptr<A> sp3 = sp1; // 此时 sp1 sp2 sp3 引用计数都为 3
```

当 sp3 初始化时为什么 sp2 感知到引用计数加1呢？首先引用计数肯定不能是静态变量，因为如果是静态变量的话：两个不相干的 sp1 和 sp2 只要模板参数 T 是同一类型，那么就会共享同一计数，显然这是错误的...

```cpp
shared_ptr<A> sp1 = make_shared<A>(); // 1
shared_ptr<A> sp2 = make_shared<A>(); // 1
```

实际上，shared_ptr 中除了有一个指针，指向所管理数据的地址。**还有一个指针执行一个控制块的地址，里面存放了所管理数据的数量（常说的引用计数）、weak_ptr 的数量、删除器、分配器等。**

也就是说对于引用计数这一变量的存储，是在堆上的，多个shared_ptr的对象都指向同一个堆地址。在多线程环境下，管理同一个数据的shared_ptr 在进行计数的增加或减少的时候是线程安全的吗？

答案是肯定的，这一操作是原子操作。

**延生：修改 shared_ptr 的指向时是否是线程安全的呢？**

参考：https://www.zhihu.com/question/56836057/answer/2158966805



##### 8、[算法] 完全背包问题：[322. 零钱兑换](https://leetcode.cn/problems/coin-change/)
