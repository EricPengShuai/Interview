## JD

### 京东广告 C++

> 2023.04.18 13:00-13:40 广告后台开发

#### 项目

- 自我介绍 8min
- 数据包重传有次数限制吗，不重传会发生什么
- 丢包很多会发生什么



#### 八股

##### 1、STL 常用容器说一下

- 序列式容器：顺序容器的元素排列次序与元素值无关，而是由元素添加到容器里的次序决定，例如：vector, list, deque

- 关联式容器：元素是有序的集合，默认在插入的时候按升序排列，例如：map, set, multimap, multiset，对应的也有无序容器，例如：unordered_map, unordered_set, unordered_multimap, unordered_multiset
- 容器适配器：适配器是容器的接口，它本身不能直接保存元素，它保存元素的机制是调用另一种顺序容器去实现，例如：stack , queue, priority_queue

**容器类自动申请和释放内存**，因此无需 new 和 delete 操作

> 参考：https://blog.csdn.net/u014465639/article/details/70241850



##### 2、hashtable 具体是一个怎样的过程

STL中的hashtable使用的是**开链法**解决hash冲突问题，如下图所示。

![hashtable](https://oss.interviewguide.cn/img/202205220035271.png)

bucket 基于 vector 实现，因为可以动态扩容，冲突链表 bucket list 是自己定义的 node 节点组成的 linked-list。

在 hashtable 设计 bucket 的数量上，其内置了 28 个质数 [53, 97, 193,...,429496729]，在创建 hashtable 时，会根据存入的元素个数选择大于等于元素个数的质数作为 hashtable 的容量（vector的长度），**其中每个bucket所维护的linked-list 长度也等于 hashtable 的容量**。如果插入 hashtable 的元素个数超过了 bucket 的容量，就要进行重建 table 操作，即找出下一个质数，创建新的 buckets vector，重新计算元素在新 hashtable 的位置。



##### 3、auto了解吗？编译期确定还是运行期间确定

auto 让编译器通过初始值来进行类型推演。从而获得定义变量的类型，所以说 auto 定义的变量必须有初始值。类型推导发生在**编译期**。



##### 4、手写一个智能指针类

> 先是问我了不了解阻塞队列

首先要明确它是一个类模板，使用 `template<typename T>` 打头

```cpp
template<typename T>
class sharedPtr {
private:
    T* _ptr;        // 原始指针
    int* _pcount;   // 引用计数指针
    
public:
    // 默认构造函数
    sharedPtr(T* ptr = nullptr): _ptr(ptr), _pcount(new int(1)) {}
    
    // 拷贝构造函数
    sharedPtr(const sharedPtr& s): _ptr(s._ptr), _pcount(s._pcount) {
        (*_pcount) ++;
    }
    
    // 重载赋值 = 操作符
    sharedPtr<T>& operator=(const sharedPtr& s) {
        if (this != &s) {
            -- (*(this->_pcount));
            if (*(this->_pcount) == 0) {
                delete this->_ptr;
                delete this->_pcnt;
            }
            _ptr = s._ptr;
            _pcount = s._pcount;
            *(_pcount) ++;
        }
        return *this;
    }
    
    // 重载取地址 & 操作符
    T& operator*() {
        return *(this->_ptr);
    }
    
    // 重载取原始指针 * 操作符
    T* operator->() {
        return this->_ptr;
    }
    
    ~sharedPtr() {
        -- (*(this->_pcount));
        if (*(this->_pcount) == 0) {
            delete _ptr;
            _ptr = nullptr;
            delete _pcount;
            _pcount = nullptr;
        }
    }
};
```



### 京东云

> 2023.12.11 — 15:00-15:30

- 实习里面 Nginx 以及开发点
- RPC 了解多少，主要不在流程，在于如何做**服务发现、负载均衡以及高可用**
- 面向对象程序设计思想
- 引用和指针概念及区别
- **开发过程遇到难点如何解决的案例，说了线程池...**

> 主要做 DNS、网关优化，涉及容器、云原生技术，go 比较多，C++ 也有 envoi

