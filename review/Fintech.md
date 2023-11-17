## 金融科技类

### 中金所技术

#### 一面

> 2023.10.13—9:20-9:40

- 实习项目是如何模拟吞吐量变化的

- epoll、select 区别，**epoll 效率一定比 select 高吗**

  > 监听的 socket 比较少或者很多短连接情况下 select 一次扫描效率并不低，另外使用 epoll_wait 之后还需要使用 epoll_ctl，涉及两次系统调用？
  >
  > 参考：
  >
  > - https://blog.51cto.com/u_15404950/5413463
  > - https://blog.csdn.net/qq_24889575/article/details/103317598

- C++ 析构函数为什么一般写成虚函数

- C++ 右值引用了解吗

- 反问：面试流程（两轮线上技术面+一轮线下HR面）、具体业务



#### 二面

> 2023.11.1 — 10:25-10:45

- 项目或者实习中遇到困难

- IO 复用如何实现的，如果回调函数参数个数不同应该如何设计

  > 使用可变参模板：
  >
  > ```cpp
  > // 定义一个接受可变参数的回调函数
  > template <typename... Args>
  > void VariableArgsCallback(const std::string& message, Args... args) {
  >  std::cout << "Message: " << message << std::endl;
  >  std::cout << "Arguments: ";
  >  (std::cout << ... << args); // 使用折叠表达式输出参数
  >  std::cout << std::endl;
  > }
  > 
  > // 使用 std::bind 绑定可变参数的回调函数
  > auto callback = std::bind(VariableArgsCallback<int, double, const char*>, "Hello", 42, 3.14, "World");
  > 
  > // 使用 std::function 包装回调
  > std::function<void()> functionCallback = callback;
  > 
  > // 调用回调
  > functionCallback();
  > ```

- **CLOSE_WAIT 状态过多如何解决**

  > 主要是由于服务端这边代码逻辑出现问题导致的，一般服务器编码逻辑为：
  >
  > 1. 创建服务端 socket，bind 绑定端口、listen 监听端口
  > 2. 将服务端 socket 注册到 epoll
  > 3. epoll_wait 等待连接到来，连接到来时，调用 accpet 获取已连接的 socket
  > 4. 将已连接的 socket 注册到 epoll**（如果没有注册时间，导致后续收到 FIN 报文是无法感知的，进而无法调用 close**
  > 5. epoll_wait 等待事件发生
  > 6. 对方连接关闭时，我方调用 close

- 两个线程同时访问同一个队列：一个读、一个写，如何设计

  > 先写完元素再 end_ptr ++ 

#### 加面

- 说一个体现你设计能力的项目
- **protobuf 了解多少**
- 做过哪些性能调休的工作：负载均衡、数据缓存
- **如何做到缓存一致性...**
- 项目中使用了哪些设计模式，单例模式需要注意什么
- **工厂模式和抽象工厂区别**
- 和别人观点不一致的场景... 
- 读过哪些 C++ 书籍



#### 终面

> 2023.11.13 8:40-9:20 上海线下面试

- 无领导，提升幸福感的措施，12选4，给公司提出三个意见



### 中金所数据

> 2023.11.02 — 9:00-9:30

- 其他具体忘了...

- Lambda 表达式什么时候使用，为什么不自定义函数指针那些

- 二叉树镜像翻转口撕，完全二叉树和普通二叉树影响吗

  > 不影响的，参考 [LCR 144. 翻转二叉树](https://leetcode.cn/problems/er-cha-shu-de-jing-xiang-lcof/)

- 未来考虑工作选择？



### 国泰君安

> 2023.11.15 11:00-11:20

- 半结构面试，自我介绍
- 实习经理细问
- 交易系统了解多少
- 需要实习：两个可选时间段 11.17-12.30 或者  1.3-3.4

> 3个人半结构面试，每个人大概 10min

