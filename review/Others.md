## Others

> 一些小厂面经整理

### 质谱华章

> 时间：2023.02.07—30min—技术栈不太匹配，二面不想约了
>

#### 一面

1、自我介绍 5 min

2、博客写的什么内容

3、视野预测项目遇到的难点

4、智力题：100! 有多少个0

考虑 0 通过 5 * 2 = 10 产生，每个 5 都会产生一个 0，由于 2 和 5 中 2 的数目比较多，没那么重要，统计可以提供 5 的数量，100 / 5 = 20

另外 25 可以提供两个 5，100 / 25 = 4，所以最终可以产生 **20+4 = 24** 个零



### 熠智科技

> 时间：2023.02.20 时长：60min
>

- 自我介绍 15min
- 项目 15min
  - 流式数据和静态数据的区别：视野预测模型如果针对一段一段数据的输出结果，如何处理，取均值？
  - 模型设计有什么特别点

#### 一面—C++

**1、一个类中默认的函数有哪些？**

6个默认成员函数：

- 构造函数

- 析构函数

- 拷贝构造函数：默认的拷贝构造函数对象按内存存储按字节序完成拷贝，是一种浅拷贝

  - 浅拷贝：也称位拷贝，编译器只是将对象中的值拷贝过来，就是简单的赋值。如果释放一个对象，访问另一个对象就会出现违规访问
  - 深拷贝：如果一个类中涉及到资源的管理，其**拷贝构造函数、赋值运算符重载以及析构函数必须要显式给出**，需要使用 new 构造新空间以及 strcpy 等拷贝变量

  > 参考：https://blog.csdn.net/Byersen/article/details/119704235

- 赋值运算符重载

  ```cpp
  void TestDate2()
  {
      Date d1(2022, 5, 18);
      Date d2(2022, 4, 18);
      Date d3 = d1;	// 等价于 Date d3(d1); 所以这里调用的是拷贝构造函数
      d2 = d1;		// 这里调用的是赋值构造函数
  }
  ```

- const 成员：注意 const 对象不能调用非 const 成员函数

  ```cpp
  // T::Print(T * const this) 参数类型为 T*, const 修饰的是 this
  void T::Print() { cout << "Print\n"; }
  
  // 实际上调用的是 t.Print(&t), &t 是 const T* 类型
  // const T* 传给 T* 是不对的，权限放大了，前者不可修改，后者可以 
  void func(const T& t) {
      t.Print();	// error
  }
  
  // const 对象只能调用 const 成员，所以改为
  // 实际上为 void T::Print(const T* const this)
  void T::Print_const() const { cout << "Print\n"; }
  
  // 权限可以不变或缩小
  void func(T& t) {
      t.Print();	// correct, 权限不变：T* 传给 T* 可以
      t.Print_const();	// correct, 权限缩小：T* 传给 const T* 可以
  }
  ```

- 取地址以及 const 取地址操作符重载：这两个运算符一般不需要重载，使用编译器生成的默认取地址的重载即可，只有特殊情况，才需要重载，比如想让别人获取到指定的内容

> 参考：https://blog.csdn.net/bit_zyx/article/details/124825804

**2、const 有什么作用，函数参数中使用这个干啥？**

```cpp
// 这两个函数有区别吗
void func(const string& str, char c)
void func(const string& str, const char c)
```

**3、指针常量和常量指针**

```cpp
// const 修饰是就近原则
const char* p; char const *p;	// 常量指针：指向常量的指针，const 修饰 *p
char * const p;	// 指针常量：指针本身是常量，const 修饰 p
```

**4、auto 关键字，类型推导的原则**

- auto 和 decltype 关键字实现类型推导，获取复杂类型，还可以简化书写
- auto定义的变量，可以根据初始化的值，在编译时推导出变量名的类型
- 可以同指针、引用结合起来使用，可以带上cv限定符 const 和 volatile

**推导规则**

（1）当不申明为指针或者引用时，auto的推导结果和初始化表达式抛弃引用和cv限定符后类型一致

（2）当声明为指针或引用时，auto的推导结果将保持初始化表达式的cv属性

> 参考：https://blog.csdn.net/weixin_58368590/article/details/124871252

**限制**

- auto不能用于函数参数
- auto不能用于非静态成员变量
- auto无法定义数组

**5、move 关键字，其中参数是什么，用来干什么的，里面具体实现细节**

move 可以用来实现转移语义，**但是 `std::move` 本身不会进行任何移动**，真正的移动操作是在移动构造函数、移动赋值函数等完成的，move 的作用就是(将左值)强制转换成右值引用（相当于一个 static_cast）

```cpp
// <utility> 头文件定义
// 以右值作为参数，但可以将左值强转为右值引用
template< class T >
typename std::remove_reference<T>::type&& move( T&& t ) noexcept;

// 可以自己实现
template <class _Ty>
constexpr std::remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
    return static_cast<std::remove_reference_t<_Ty>&&>(_Arg);
}
```

参考：

- [c++ move函数到底是什么意思？](https://www.zhihu.com/question/64205844)
- https://blog.csdn.net/qq_51568363/article/details/124285294

**6、万能引用是啥**

万能引用(universal reference)，也称为未定义引用，如果一个变量或参数被声明为 `T&&`，并且需要推导 `T` 的类型，那么该变量或参数即为万能引用。

万能引用，既可以是左值引用（绑定左值），也可以是右值引用（绑定右值）。

```cpp
#include <iostream>
using namespace std;

template <typename T>    // 函数模板
void myfunc(T&& val)     // 万能引用
{
    cout << val << endl;
    return;
}

int main()
{
    myfunc(120); // ok，自动类型推导，120是右值，T为int类型，val为int&&类型
    int i = 180;
    myfunc(i); // ok，自动类型推导，i是左值，T为int&类型，val为int&类型
    return 0;
}
```

注意：

- 区分右值引用和万能引用
- const 会剥夺万能引用的资格

> 参考：[[深入理解C++]万能引用、引用折叠](https://blog.csdn.net/qq_42815188/article/details/122751869)



#### 一面—操作系统

1、虚拟内存是干什么的

2、分页地址是什么形式，如果缺页如何替换

> 参考：[cyc2018——机操作系统 - 内存管理](http://www.cyc2018.xyz/计算机基础/操作系统基础/计算机操作系统%20-%20内存管理.html#虚拟内存)



#### HR 面

1、自我介绍 5min

2、实习时间点，为什么选择投该公司，之前还面过哪些公司

3、我们的业务点主要在于隐私保护，你之前主要关注流媒体



### Momenta 实习

> 2023.02.08—30min—答的挺好的莫名其妙给挂了

#### 一面

1、自我介绍 5min

2、视野预测的项目：怎样建模，多个用户请求怎么处理（多个模型）

3、非递归实现二叉树的前序遍历，[参考](https://github.com/EricPengShuai/Interview/blob/main/algorithm/%E4%BA%8C%E5%8F%89%E6%A0%91%E9%81%8D%E5%8E%86%E6%96%B9%E5%BC%8F.md)

> 反问：部门具体做啥（C++处理已经训练好的模型或者 feature map）



### 虹软科技

> 2023.09.08—14:20-14:40—20min 多少有点离谱

#### 一面

1、自我介绍 8min，工作 base 啥的

2、实习：播放系统部署在 Linux 上还是 windows 上

3、动态链接和静态链接

> 自我介绍就花了 10min 左右，然后一个问题之后直接结束了，20min 时长多少有点离谱



#### 二面

> 2023.09.21—15:15-15:45

- 自我介绍
- 实习项目以及实验室方向具体介绍
- 两个词描述自己
- 有 offer 吗，地点以及方向的选择

> 说是技术面，但是没怎么问 C++ 项目以及八股，有点迷...



### 4399

> 2023.09.12—15:25-16:00—AI研发工程师

#### 一面

1、视野预测项目如何设计模型的

2、TCP 项目如何设计的

3、了解模型部署的后端框架吗（寄）、服务端呢

4、负载均衡算法有哪些（轮询说错了

5、反问：部门主要做啥（AI算法研究+AI算法落地部署）

6、能来实习吗

> 大概率寄



### 奇安信

#### 一面

> 2023.09.18 17:00-17:30

- 数据结构：快速排序思想、堆排序思想、复杂度

- 使用 wireshark 抓过包吗，具体场景是什么

- 异步同步IO，select、epoll 怎么使用

- gdb 如何调试一个已经运行的程序、查看堆栈命令、finish/return 区别

  > ps 查看运行进程的 pid，attach pid 即可调试该进程
  >
  > backtrace 查看调用堆栈信息，可以查看 coredump 具体函数位置
  >
  > finish(fi) 结束当前正在执行的函数，并在跳出函数后暂停程序的执行，**执行函数到正常退出**
  >
  > return 结束当前调用函数并返回指定值，到上一层函数调用处停止程序执行，**当前函数还有剩余的代码未执行完毕，也不会执行了，并且可以指定返回值**

- git 命令，git push/pull/reset

- gcc 编译文件中如何获取函数调用点的地址信息（不懂...

> 一面竟然还问薪资期望，还不让我反问“薪资等级构成，想要中等”...



### 文远知行

#### 一面

> 2023.09.20 14:00-15:10

- 实习项目说一下，难点是什么，最终效果怎么样

- LRU 了解吗，具体可以怎么实现，时间复杂度

- select、poll、epoll 是什么，有什么不同

- 模板原理是什么，虚函数可以写成模板函数吗

  > 编译期间和运行期间角度

- shared_ptr 线程安全问题，weak_ptr 作用

- TCP 和 UDP 不同

- 算法题：变形背包问题，给定 n 个物品的重量 w_i, 价值 v_i，背包容量 m，其中 1 <= n <= 40, 0 <= w_i, v_i, m <= 10^15

  > 由于背包容量高达10^15，因此不能用简单 dp 来思考
  >
  > 可以考虑从 n 的范围入手，直接枚举 n 的子集，最多 2^40 复杂的，还是太高
  >
  > - 考虑分成两堆，每堆最多 20 个元素，复杂度为 2^20
  > - 枚举 20 个元素的子集问题，考虑用集合的思想，1 << 20
  > - 预处理第二个集合，按照重量排序，重量前缀和，维护最大的价值，这样在枚举第一个集合元素的时候就可以二分查找指定重量的最大价值

> 反问业务做什么，介绍了自动驾驶一般流程：日志、回放、仿真等，总的来说体验很好



### 得物

#### 一面

> 2023.09.23 09:30-10:00 算法工程

- 自我介绍之后直接做题，[102. 二叉树的层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)
- 项目中遇到难点
- MySQL  索引结构，为什么要用 B+ 树
- MySQL 事务的理解
- MySQL MVCC 机制
- 为什么在本校读研了，本科没考虑直接就业吗
- 反问：做什么业务？推荐相关，Java 比较多，C++ 也有

#### 二面

> 2023.10.21 10:50-11:15 算法工程

- 场景题：**两地机房分别有一个很大文件，如何得到这两个文件不同的数据项**

  > 1. 读取一条数据 A，求 A 的 hashcode，然后取模，即 X = a.hashcode % 10000，X 的范围为 0~9999，将数据 A 保存到 fileX 文件中
  >
  > 2. 按步骤 1 进行文件的遍历，这样 2T 的文件被分割成 10000 个小文件，但相同的 2 行数据肯定在同一个小文件中
  > 3. 一次处理每个小文件，找出在一个小文件中是否存在相同的两行

- **static 修饰函数**和变量有什么不同

  > static 修饰函数作用就是限制作用域，和修饰变量不太一样（运行期间不会销毁），具体来说
  >
  > - 静态全局变量：限制变量的作用域，仅在本文件中访问，其他文件不可访问
  > - 静态局部变量：仅在本函数体内访问，本文件其他函数体内不可访问；但静态局部变量的值在程序运行期间不会销毁
  > - 静态函数：仅在本文件中调用，其他文件中不可调用，即程序员不用担心编写的函数与其他文件的函数同名

- static 修饰局部变量有线程安全问题吗

- 实现先进先出的队列，O(1) 时间访问两段元素，如何实现 isFull() 和 isEmpty()

- C++ shared_ptr、weak_ptr 以及 unique_ptr

- 数据库查找每个班级男生女生数量 sql 写法

#### 三面

> 2023.12.09 — 14:50 - 15:20

- 挑一个自己项目展开说说

- 集群服务器场景下如何控制每台服务器的流量稳定在 40% 左右

  > 负载均衡：让每台服务器负载相对均衡
  >
  > 自定义流量控制算法、队列管理（类似于线程池中任务队列拒绝服务
  >
  > 自适应控制系统：通过反馈机制来实时监测流量和服务器负载情况

- 线程池设计需要考虑哪些点

- vector list map 存储 1000 个元素哪个占内存更多

- 数据库索引

- 如何判断 sql 语句使用了索引，filesort 是什么原因

  > - explain 语句关注几个重要的字段：type（查询类型）、key（使用的索引）、rows（估计的扫描行数）、extra（额外开销）
  > - 出现 filesort 原因很多，例如 order by、group by 以及连接操作

- 自己优缺点？？？？

> 纯纯被捞





### 富途

#### 一面

> 2023.10.12—17:20-18:45 后台开发

- 挑一个项目具体说一下
- epoll、select、poll 区别（回答的很流畅面试官说是不是背的...
- 线程池如何设计的，线程数量是多少
- 高并发处理如何做到线程安全，聊了聊异步日志的实现
- RPC 分布式框架如何实现高可用
- zookeeper 如何管理服务注册以及服务发现
- 排序算法有哪些、归并排序实现、堆排序实现
- 数据库隔离级别、索引查询优化
- HTTP 和 HTTPS 区别、如何加密、加密算法知道吗
- 操作系统内存管理机制、LRU 如何实现
- 两个数组，如何找出公共元素，如果数组元素很大在两个大文件中呢
- 场景题：设计一个高并发的服务器
  - 很多地区访问，如何让时延降到最低
  - 图片等数据如何存储
  - 高并发如何实现、高可用呢
- 算法：简单的回文串判断（除了双指针还有什么方法

> 大概迟到20min左右，然后面了一个半小时，面试真的就是纯纯“拷打”，按照简历一个一个输出八股，有时候回答的太流畅还不太行，面试官以为我是直接背的... 

#### 二面

> 2023.10.17—15:00-15:45

- 自我介绍之后直接手撕代码

  ```cpp
  // 先给了一道 sql 题，看我不太熟练又出了一道背包问题
  
  // 游戏角色, 有技能列表和魔法值, 求能造成的最大伤害
  // 例1：
  // 输入 skill_list: [{cost:10, damage:10}, {cost:12, damage:13}], cur_cost: 20, 
  // 输出 max_damage: 20
  
  // 例2：
  // 输入 skill_list: [{cost:10, damage:10}, {cost:12, damage:13}], cur_cost: 25
  // 输出 max_damage: 26
  
  // 例3：
  // 输入 skill_list: [{cost:2, damage:5}, {cost:4, damage:11}, {cost:7, damage:20}], cur_cost: 13
  // 输出 max_damage: 36
  
  // 记忆化搜索或者 DP
  代码参考：https://www.codepaste.cn/#/cd/a9416624-c740-42d0-b361-eeba5207f7d0
  ```

- 视频压缩算法了解吗（x

- 网络 IO 分类

- 主机字节序由什么决定的（CPU 架构

- HTTP 了解多少

- **密钥交换算法或者加密算法**

  > - Diffie-Hellman（迪菲-赫尔曼）算法：用于在公开信道上安全地交换密钥
  > - RSA（Rivest-Shamir-Adleman）算法：基于大数分解难题，用于加密通信和数字签名。
  > - ECDHE（Elliptic Curve Diffie-Hellman椭圆曲线迪菲-赫尔曼）算法：基于椭圆曲线离散对数难题，提供更高的安全性和效率
  > - DSA（Digital Signature Algorithm）算法：用于数字签名，基于离散对数问题
  > - IKE（Internet Key Exchange）协议：用于在IPsec虚拟专用网络中进行密钥交换

  

### 招联金融

> 2023.10.21 — 10:15-10:40
>
> 招联这个面试系统等待真的是一眼难尽，这次本约的是 9:50 结果又等到 10:15，上次面试实习也是这样，另外感觉还是希望招 Java 选手，当晚没有收到二面应该是寄了

- 自我介绍，工作地点啥的
- 面向对象思想了解吗
- 观察者模式说一下
- 出现异常如何处理

> 面试官看我 cpp 选手，也不太好意思问专门 Java 八股，就挑了一些 cpp 和 java 的公有知识



### 深信服

> 2023.10.25 — 10:40-11:20

- C++ 中哈希表，解决冲突的方式，其**中再次散列法如何查找元素**

  > 再次哈希查找过程通常会有一个预测的最大迭代哈希次数，防止进入死循环

- 算法：链表反转问题

- 实习项目具体类型，难点

- 有关注抖音那些短视频传输技术中的奇数吗，和 DASH 有关吗

- DASH 协议可以用于远程桌面技术吗

> 9月初笔试，10月底线下约面（回去等结果估计直接寄了），而且体验不太好，面试官有些问题”不知所云“



### 亚控科技

> 2023.10.27 — 18:00-18:30

- 实验室做的什么方向，难点在哪

- epoll 原理，**什么情况比 select 效率低**

  > 当监听的套接字数量较少，并且全部活跃时 select 可能优于 epoll，因为套接字全部活跃时，epoll 设置的回调函数调用开销比较大

- RPC 介绍一下，**RPC 优缺点**

  > - 优点：protobuf 序列化相比于 json **性能高**，不关心底层协议；利用中间件实现服务注册和发现，**扩展性强**；帮助开发人员在不同节点之间调用远程服务，**简化了分布式系统开发**
  > - 缺点：需要维护 client 和 server 端的接口一致性，定义接口和数据结构导致耦合度较高

- 多态的理解，虚函数底层实现的

- 线程安全问题，自旋锁和互斥锁区别

- 工作地点以及期望薪资，性格优缺点

- 反问：主要做数据库相关的开发



### TPLink 联洲

#### 一面

> 2023.10.31 — 16:55-17:20 后端开发岗位 Java 偏多

- OSI 七层模型，每层有哪些协议以及功能

- 进程和线程区别

- 进程通信方式

  > 管道、共享内存、信号量、消息队列、套接字

- TCP 和UDP 的区别

- 哈希表了解吗，解决哈希冲突方式

- 红黑树了解吗，是平衡二叉树吗，使用场景

#### 二面

> 2023.11.23 — 10:30-11:20

- DASH 和 RTSP 协议、推拉流的过程
- **微服务 —> RPC 的过程**
- 智能指针使用
- static 关键字作用
- C++11 新语法特性
- 慢查询日志，具体使用过吗
- 排序算法有哪些、时间复杂度、稳定性
- 算法：两个栈实现一个队列、手撕快排、[1143. 最长公共子序列](https://leetcode.cn/problems/longest-common-subsequence/)
- 接受转语言吗

#### 三面

> 2023.11.28 — 14:25-14:50

- 自我介绍，创新创业项目具体实现
- 成绩排名，实习项目 DASH 和 WebRTC
- 后端HC满了，调至应用开发岗

> 大概一周左右出结果？





### Momenta 秋招

#### 一面

> 2023.11.07 — 10:00-11:00，C++ 开发

- 实习难点，主要语言是啥

- C++ 11 新特性

- 线程和进程的区别，何时使用多进程、何时使用多线程

  > 参考腾讯 review

- 索引底层原理，何时失效，**何时冲突**

  > MySQL中索引冲突通常是指并发操作中由于锁定索引资源而导致的性能问题。以下是一些可能导致索引冲突的情况：
  >
  > 1. **并发写入操作**：当多个事务或查询同时尝试修改同一张表的索引时，会出现索引冲突。例如，多个INSERT、UPDATE或DELETE语句同时影响同一行数据。
  > 2. **表锁和行锁冲突**：MySQL支持表锁和行锁。如果多个事务同时尝试锁定相同的资源，可能会导致锁冲突。行锁通常更细粒度，但也更容易导致死锁问题。
  > 3. **不同存储引擎的锁机制**：MySQL支持多种存储引擎，不同存储引擎可能采用不同的锁机制。在涉及多个存储引擎的查询中，可能会出现锁冲突。
  > 4. **长事务**：长时间运行的事务可能会持有锁资源，导致其他事务无法访问相同的资源，从而引发索引冲突。
  > 5. **索引尺寸和性能**：较大的索引通常需要更长的时间来锁定，可能会增加锁冲突的机会，尤其是在高负载情况下。
  > 6. **查询优化不当**：复杂的查询可能会导致锁定更多资源，如果不合理地设计查询或索引，可能会增加锁冲突的风险。
  >
  > 为了减少索引冲突，可以考虑以下方法：
  >
  > 1. 使用合适的存储引擎：根据应用程序的需求选择合适的存储引擎，不同存储引擎有不同的锁机制。
  > 2. 使用合适的索引：为经常查询的列创建适当的索引，以提高查询性能。
  > 3. 尽量缩短事务时间：长事务可能会持有锁资源，导致锁冲突。因此，尽量将事务保持简短。
  > 4. 使用行锁：在需要并发访问相同表的情况下，使用行锁而不是表锁。
  > 5. 避免死锁：实现良好的事务管理，避免死锁的发生，例如通过为事务中的查询操作加锁。
  > 6. 监控和优化：定期监控数据库性能，分析潜在的锁冲突问题，并进行适当的优化。
  >
  > 索引冲突是数据库管理的一部分，需要根据具体情况和数据库引擎来管理和解决。正确的数据库设计和查询优化可以帮助减少索引冲突的风险。

- 浮点数存储原理、int 占几个字节

- 算法：循环报数问题（思路很简单，注意边界处理）



### 联想C++后台开发

> 2023.10.13—19:00-19:30

- 自我介绍、工作地点有要求吗、C++ 方面学过什么

- STL 中常见的序列容器有哪些、list 和 deque 区别以及使用场景

- 共享屏幕展示 muduo 网络库展开说说

  - 你的编码在哪：绑定器以及函数对象、thread
  - **Buffer 线程安全问题**

- **设计模式的观察者模式如何实现**

  > 参考 [DesignPattern](../DesignPattern.md)



### 南京三星电子

#### 一面

> 2023.11.3 — 11:00-11:30

- 英文自我介绍
- C++ vector 和 list 区别，查找性能有区别吗，如何改进方式
- 智能指针相关
- Dash 协议具体原理，如何切换码率的
- 遇到吞吐量突变的情况如何处理（混合考虑 buffer 和 throughout？
- **Dash 传输过密流吗**
- HUAWEI项目 Dash 分块传输视频，几条流，同步和时延问题

> 反问得知只有一轮技术面+一轮HR面

#### 二面

> 2023.11.17 人事面





### 帆软

> 2023.12.05 — 10:00 - 10:40

- 浮点数相加 0.1 + 0.2 为什么是0.30000000000000004，为什么会导致精度不准

  > 有点复杂：https://zhuanlan.zhihu.com/p/54991069

- 一个文件，每行都是一个整数，一共一千万行，如何快速去重，时间复杂度和空间复杂度尽量低

  > 1. 哈希表
  > 1. bitmap：利用一个整数可以存放 32 个整数的状态节省空间，[参考](https://blog.csdn.net/MBuger/article/details/61628572)

- MySQL **优化器**相关：索引如何选择的问题（A，B两个字段都有索引，where A and B...此时索引是选择 A 还是选择B）

  > - 如果只有单独的索引，会分别使用两个索引进行查询然后进行索引合并
  > - 如果有联合索引就会优先使用联合索引进行查询

- 为什么写博客、如何学习一门新的技术、关注哪些技术网站、了解帆软的产品吗

- 你负责的产品客户在使用过程中发现了很多bug，你如何应对

> 3 天左右出结果



### 柠檬微趣

#### 一面

> 2023.12.13 — 14:30 - 15:30

- muduo 库线程池、使用了哪些C++11新特性、智能指针用在哪、红黑树定时器（单线程使用）

- 移动语义，**除了 std::move 产生右值还有哪些方式**

  > 除了 `std::move` 之外，还有以下几种方式可以产生右值：
  >
  > 1. 使用右值引用：通过声明和使用右值引用（`&&`）来将左值转换为右值。
  >
  > ```cpp
  > int x = 10;
  > int&& y = std::move(x);  // 使用 std::move 将左值转换为右值引用
  > ```
  >
  > 2. 使用 `std::forward`：`std::forward` 可以根据传入的参数类型决定是将其转发为左值引用还是右值引用。
  >
  > ```cpp
  > template<typename T>
  > void process(T&& arg) {
  >     // 使用 std::forward 将 arg 转发为左值引用或右值引用
  >     some_function(std::forward<T>(arg));
  > }
  > ```
  >
  > 3. 使用临时对象：在某些情况下，编译器会为表达式生成临时对象作为右值。
  >
  > ```cpp
  > std::string concatenate(std::string&& str1, std::string&& str2) {
  >     return std::move(str1) + std::move(str2);  // 临时对象作为右值
  > }
  > ```
  >

- **std:: move 产生的右值和临时右值的区别**

  > 1. 生命周期：临时对象的右值是由编译器在需要时创建的临时对象，其生命周期仅限于表达式的求值过程。一旦表达式求值完成，临时对象将被销毁。而使用 `std::move` 产生的右值是通过将左值转换为右值引用延长了其生命周期，使其可以继续使用。
  >
  > 2. 可修改性：临时对象的右值通常是不可修改的，因为它们是由常量或临时对象生成的。而使用 `std::move` 产生的右值是可以修改的，因为它们实际上是左值的右值引用。
  >
  > 3. 使用场景：临时对象的右值通常用于传递给函数或操作符，并且在表达式求值完成后即被销毁。而 `std::move` 产生的右值通常用于转移资源的所有权，例如将对象移动到另一个对象或容器中。
  >
  > 总之，临时对象的右值是由编译器生成的临时对象，其生命周期短暂且不可修改。而 `std::move` 产生的右值是通过将左值转换为右值引用延长了其生命周期，并且可以进行修改。

- 线程和进程区别，fork 子进程会额外增加内存吗（Copy On Write）

- 排序算法，**如何避免快速排序极端的时间复杂度**

  > C++ 标准库中的 `std::sort` 函数通常采用一种混合使用快速排序（QuickSort）和插入排序（Insertion Sort）的排序算法。这个排序算法被称为 Introsort。
  >
  > Introsort 是一种混合排序算法，它结合了快速排序的高性能和**插入排序**的稳定性。在实际应用中，当递归深度达到一定阈值时，`std::sort` 会切换到使用插入排序。插入排序对小规模的数据集有较好的性能，因为它的常数因子相对较小。
  >
  > 在 C++ 标准库的实现中，`std::sort` 通常还会对递归深度进行检测，如果检测到递归深度过大，它可能会转而使用**堆排序**（Heap Sort）来避免快速排序的最坏情况时间复杂度 O(N^2)。

- 物理内存和虚拟内存，5大分区，如果修改 .text 段会报 segmentfault 错误，**这个中断是哪个触发的**

- 算法题：[LCR 170. 交易逆序对的总数](https://leetcode.cn/problems/shu-zu-zhong-de-ni-xu-dui-lcof/)



#### 二面

> 2023.12.15 — 10:30 - 11:00

- 10G整数文件需找中位数，内存限制 2G

  > 思路1：基于字节的桶排序思想，取整数的前8位也就是一个字节作为区分不同桶的key，也就是最多 2^8 = 256 个文件/桶 [参考](https://blog.csdn.net/sytu_hzj/article/details/6856775)
  >
  > - 首先把 10G 整数每 2G 读入内存，放入对应的文件/桶中
  > - 根据每个文件/桶的数量计算中位数落在哪个桶中，假设10G文件有 N 个数，那么中位数就在第 N/2 个，统计每个文件/桶中元素个数得出「目标桶」
  > - 如果目标桶中的元素所占内存小于 2G 直接排序即可，否则再次按照以上思路分桶计算
  >
  > 思路2：外排序+归并
  >
  > 	- 分成小文件之后，对每个小文件排序
  >  - 直接归并所有小文件内存应该不支持？ 可以考虑使用堆
  >    - **初始化堆：** 从每个小块中读入第一个元素，构建一个最小堆。堆中的每个元素都包含两部分信息：值和所属的块号。
  >    - **找到中位数的位置：** 假设元素个数为 N，那么中位数就是排序之后的第 N/2 个元素
  >    - **选择中位数：** 从堆中依次取出元素，每次取出一个元素，就将对应块的下一个元素加入堆中。当取出的元素个数达到中位数的位置时，堆顶元素即为中位数。
  >
  > 思路3：堆排序
  >
  > - 转换成求 top N/2 大的元素（最大堆求前n小，最小堆求前n大）
  > - 由于内存限制可能无法一次性构建大小为 N/2 的堆，可以依次次求第1G大、第2G大... [215. 数组中的第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array/)
  >
  > [海量数据中寻找中位数-知乎](https://zhuanlan.zhihu.com/p/75397875)

- 高并发请求如何处理（Multi-Reactor + 多线程）、任务和线程有对应关系吗

- 多线程写日志如何设计（生产者消费者模型的日志系统，[logger.cc](https://github1s.com/EricPengShuai/MPRPC/blob/HEAD/src/logger.cc)

  - 生产者：多线程写消息到安全队列
  - 消费者：刷盘线程，目前是一个单线程的