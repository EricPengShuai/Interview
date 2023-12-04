## 武汉企业



### 天融信

#### 一面

> 2023.11.23 — 14:00-14:30

- 进程和线程的区别
- 代码编译过程
- TCP/IP 协议栈、TCP 三次握手和四次挥手、TIME_WAIT 状态过多
- 读写锁
- 日常爱好、如何看待加班

#### 二面

> 2023.11.29 — 16:00 - 16:30

- muduo 如何分配线程资源，IO线程和业务线程
- Socket 编程一般步骤
- 多线程和多进程区别
- 进程通信方式（**共享内存如何实现**），多线程访问如何保证安全
- IP 头和 TCP 头哪些比较常用的字段
- TCP 和 UDP 区别，UDP 有纠错吗
- 解决 BUG 实际例子
- 个人优势以及提升点在哪





### 长江证券

> 2023.11.29 — 11:05-11:25

- RPC 调用过程

- 为什么选择 protobuf，了解其他序列化和反序列化协议吗

- zookeeper 节点挂掉之后怎么办，**grpc、dubbo 框架**，**考虑多个可用节点如何选择的问题吗**

- **RTSP 协议信令交换过程**

- 单例模式如何实现

- **如何限制类模板的类型为数值类型**

  ```cpp
  #include <iostream>
  #include <type_traits> // 包含 std::enable_if
  
  template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
  class NumericOnlyTemplate {
  public:
      NumericOnlyTemplate(T value) : data(value) {}
  
      void print() {
          std::cout << data << std::endl;
      }
  
  private:
      T data;
  };
  
  int main() {
      // 编译通过，使用 int 类型
      NumericOnlyTemplate<int> intObj(42);
      intObj.print();
  
      // 编译通过，使用 double 类型
      NumericOnlyTemplate<double> doubleObj(3.14);
      doubleObj.print();
  
      // 编译错误，尝试使用 std::string 类型
      // NumericOnlyTemplate<std::string> stringObj("Hello"); // Error!
      return 0;
  }
  ```

- golang 了解吗，defer 关键字？C++ 中如何实现

- select、poll、epoll

> 一周左右出结果？