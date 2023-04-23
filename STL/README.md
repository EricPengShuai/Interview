## C++ STL
**C++ STL** 之所以得到广泛的赞誉，也被很多人使用，不只是提供了像`vector`, `string`, `list`等方便的容器，更重要的是**STL**封装了许多复杂的数据结构算法和大量常用数据结构操作。



### vector

- [*vector.cpp*](vector.cpp)：**vector**封装数组: 便于随机访问
- [*emplace_vs_push.cpp*](emplace_vs_push.cpp)：**emplace_back**和**push_back**的区别：好像是前者效率高
  - 在接受 `T, T&, T&&`作为参数时，empalce_back 和 push_back 没有区别，分别调用普通构造函数、拷贝构造函数和移动构造函数
  - emplace_back 支持传入 class T 的构造函数的参数，并且此时**没有任何临时变量生成**，但是 push_back 不支持
  > 参考1：https://blog.csdn.net/iaibeyond/article/details/119153008  
  > 参考2：https://ppipp.blog.csdn.net/article/details/84764104



### list

- forward_list 是单向链表，list 是双向链表
- 两者都是序列容器，可以在 O(1) 时间内插入和删除元素
- *[list](list.cpp)*：list 和 forward_list 的基本用法
  - splice 转移函数，LeetCode LRU 经典题目用法：[146. LRU 缓存](https://leetcode.cn/problems/lru-cache/)



### string

- *[string.cpp](string.cpp)*：**string**基本用法



### queue vs deque

- 两者最主要的区别是：具体参考 [queue_vs_deque.md](queue_vs_deque.md)
  - deque支持push_front、pop_front、push_back、pop_back
  - queue支持push（实际上就是pop_back）、pop（实际上就是pop_front）
- *[priority_queue.cpp](priority_queue.cpp)*：优先队列用法
  - 默认是 `less<int>`：大顶堆，为什么升序却是大顶堆呢，[因为堆排序一个最大堆才会得到升序](https://www.zhihu.com/question/343715076)
  - [数据结构与算法--排序算法：堆排序 最大堆（大顶堆）和 最小堆（小顶堆）](https://blog.csdn.net/storyfull/article/details/102925462)




### map vs set

- **map** 封装了二叉树等
    - *[mapSTL.cpp](mapSTL.cpp)*：`map`底层是一个红黑树，有序的（按照`key`排序），一般处理有序的情况
    - *[unordered_map.cpp](unordered_map.cpp)*: `unordered_map` 底层是一个哈希表，便于查找，但是内存占用比较高
      - 手撕哈希表：[hashtable.h](hashtable.h)  [手撕哈希表详解](https://blog.csdn.net/m0_50945504/article/details/121557561)
    - `map` 和 `unordered_map` 外部操作都是一样的，只是内部实现不一样
    
- **set** 也是封装了二叉树，基于红黑树实现
    
    - 和 `map` 的区别就是将 `value` 作为 `key`，`map` 的 `key` 和 `value` 是分开的；`map`允许修改 `value` 不允许修改 `key`，`set` 的迭代器是 `const`，不允许修改元素的值
    - *[setSTL.cpp](setSTL.cpp)*：`multiset` 用法，和 `set` 类似，只是前者可以保存重复元素，后者不保存，默认时升序排列的也就是 `multiset<int, less<int>>`
    
      ```cpp
      set<int> st;
      // set 是双向迭代器，支持 p++, ++p, p--, --p，不支持 p += i
      int b = *(++st.begin());	// ok
      int c = *(st.begin()+1); 	// error
      
      // vector 是随机访问迭代器，支持 p += i
      vector<int> vt(3);
      int d = *(vt.begin()+1);	// ok
      int e = *(++vt.begin());	// ok
      ```



### sort

- 自定义sort函数：*[sort1.cpp](sort1.cpp)*：使用**lambda表达式**和**函数**；*[sort2.cpp](sort2.cpp)*：使用**模板**和**函数**

- 主要有三种自定义比较方式的写法：
    1. 模板
       ```cpp
       template <typename T>
       struct cmp1
       {
           bool operator()(const T &x, const T &y)
           {
               return x > y;
           }
       };
       ```
    2. 函数
       ```cpp
       // 自定义sort 2.使用函数
       int cmp2(int x, int y) {
           return x > y;
       }
       ```
    3. 逆序`greater<int>`，顺序`less<int>`
       ```cpp
       sort(vt.begin(), vt.end(), greater<int>());
       ```
    4. lambda表达式
       ```cpp
       sort(b.begin(), b.end(), [](vector<int> a, vector<int> b){
               if (a[0] == b[0]) {
                   return a[1] < b[1];
               } else {
                   return a[0] < b[0];
               }
           });
       
       sort(vt.begin()+left, vt.end(), [](const int &x, const int &y) {
               return x > y;
           });
       ```

> :warning: 注意这里自定义的函数**不能加等号**！！！



### thread

> C++ 11 引入 <thread> 头文件，主要用于实现多线程编程，并结合互斥类 <mutex> 实现临界区的访问

代码参考 [thread.cpp](thread.cpp)，主要的成员函数有

- `get_id()`：获取线程ID，返回一个类型为`std::thread::id`的对象
- `joinable()`：检查线程是否可被 join 。检查 thread 对象是否标识一个活动 (active) 的可行性线程。缺省构造的 thread 对象、已经完成 join 的 thread 对象、已经 detach 的 thread 对象都不是 joinable
- `join()`：调用该函数会**阻塞当前线程**。阻塞调用者 (caller) 所在的线程直至被 join 的 std::thread 对象标识的线程执行结束
- `detach()`：将当前线程对象所代表的执行实例与该线程对象分离，**使得线程的执行可以单独进行**。一旦线程执行完毕，它所分配的资源将会被释放
- `native_handle()`：该函数返回与 `std::thread` 具体实现相关的线程句柄。`native_handle_type` 是连接thread 类和操作系统 SDK API 之间的桥梁，如在 Linux g++(libstdc++) 里，native_handle_type 其实就是pthread 里面的 pthread_t 类型，当 thread 类的功能不能满足我们的要求的时候 (比如改变某个线程的优先级)，可以通过 thread 类实例的 `native_handle()` 返回值作为参数来调用相关的 pthread 函数达到目的
- `swap(t1, t2)`：交换两个线程对象所代表的底层句柄
- `operator=`：moves the thread object
- `hardware_concurrency()`：静态成员函数，返回当前计算机最大的硬件并发线程数目。基本上可以视为处理器的核心数目

**参考：**

1. [C++ thread用法总结(整理)](https://blog.csdn.net/sevenjoin/article/details/82187127)
2. [C++ std::thread | 菜鸟教程 (runoob.com)](https://www.runoob.com/w3cnote/cpp-std-thread.html)
3. [C++ 多线程 | 菜鸟教程 (runoob.com)](https://www.runoob.com/cplusplus/cpp-multithreading.html)



### unique

该函数的作用是“去除”容器或者数组中相邻元素的重复出现的元素，注意

(1) 这里的去除并非真正意义的erase，而是将重复的元素放到容器的**末尾**，返回值是去重之后的尾地址。

(2) unique 针对的是相邻元素，所以对于顺序顺序错乱的数组成员，或者容器成员，需要先进行排序，可以调用std::sort()函数

```cpp
std::vector<int> vt;
std::sort(vt.begin(), vt.end());	// 排序
vt.erase(unique(vt.begin(), vt.end()), vt.end());	// 去重
```



### iterator

#### 定义

- 正向迭代器：`容器类名::iterator 迭代器名;`
- 常量正向迭代器：`容器类名::const_iterator 迭代器名;`
- 反向迭代器，`容器类名::reverse_iterator 迭代器名;`
- 常量反向迭代器，`容器类名::const_reverse_iterator 迭代器名;`

#### 分类

- 正向迭代器：支持 ++p, p++, *p 等操作，还可以用 == 和 != 比较，**但是不能用 < 或者 > 比较**
- 双向迭代器：只有正向迭代器的全部功能，另外还支持 --p, p-- 操作，**也不能用 < 或者 > 比较**
- 随机访问迭代器：具有双向迭代器的全部功能，另外还支持 `p += i`, `p -= i`, `p+i`, `p-i` 操作，p[i]：返回 p 后面第 i 个元素的引用，其中 i 是一个整数，**支持用 < 或者 > 比较** [参考](http://c.biancheng.net/view/338.html)

| 容器                                   | 迭代器         |
| -------------------------------------- | -------------- |
| vector、deque                          | 随机访问迭代器 |
| stack、queue、priority_queue           | 无             |
| list、(multi)set/map                   | 双向迭代器     |
| unordered_(multi)set/map、forward_list | 前/正向迭代器  |

#### 移动

- [advance.cpp](advance.cpp)：实现迭代器的加减操作，没有返回值
- [prev_next.cpp](prev_next.cpp)：prev 原意为“上一个”，next 原意为“下一个”，是有返回值的
- `distance(p, q)`：计算两个迭代器之间的距离，即迭代器 p 经过多少次 + + 操作后和迭代器 q 相等。如果调用时 p 已经指向 q 的后面，则这个函数会陷入死循环
- `iter_swap(p, q)`：用于交换两个迭代器 p、q 指向的值

#### 失效问题

- vector 插入和删除都可能导致迭代器失效，因为插入可能导致扩容 [参考](https://blog.csdn.net/Ferlan/article/details/86423563)
- list 删除时可能导致失效，插入不会导致扩容

**插入**

```cpp
// 在尾迭代器处插入 6

vector<int> vt{1,2,3,4,5};
auto it = find(vt.begin(), vt.end(), 5);
vt.insert(it, 6);
cout << *it << endl;	// 失效值，0

list<int> ls{1,2,3,4,5};
auto it = find(ls.begin(), ls.end(), 5);
ls.insert(it, 6);
cout << *it << endl;	// 正常值，5
```

**删除**

```cpp
// find 3 然后 erase 掉，然后在 it 的位置插入 0

vector<int> vt{1,2,3,4,5};
auto it = find(vt.begin(), vt.end(), 3);
vt.erase(it);
// g++ 下不会报错，VS 下会报错
vt.insert(it, 0);	// 【注意】此时 it 已经失效，解决方式 it = erase(it) 利用 erase 的返回值

list<int> ls{1,2,3,4,5};
auto it = find(ls.begin(), ls.end(), 3);
lt.erase(it);
// g++ 下就会报错
lt.insert(it, 0);	// 【注意】此时 it 已经失效，解决方式 it = erase(it) 利用 erase 的返回值
```

参考：https://bianchengsite.com/1ejUO7hlf9EWF99Tpuoj6A



### move

std::move 并不会真正地移动对象，真正的移动操作是在**移动构造函数、移动赋值函数**等完成的，std::move 只是将参数转换为右值引用而已（相当于一个 static_cast）

更确切的说：move 的本质就是帮助编译器选择重载函数, 告诉编译器 "请尽量把此参数当做右值来处理"

参考代码： [move.cpp](move.cpp) 

**参考**

- [c++ move函数到底是什么意思？](https://www.zhihu.com/question/64205844)
- [C++:move，带你从根本理解move函数是什么](https://blog.csdn.net/qq_51568363/article/details/124285294)
- https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?redirectedfrom=MSDN&view=msvc-170
- https://zh.cppreference.com/w/cpp/utility/move



### forward

TBC...

**参考**

- https://zh.cppreference.com/w/cpp/utility/forward



### __builtin（GCC 内建函数）

- `__builtin_ctz()` / `__builtin_ctzll()`：返回括号内数的二进制表示形式中末尾0的个数
- `__buitlin_clz()`  / `__buitlin_clzll()`：返回括号内数的二进制表示形式中前导0的个数（leading zero）
- `__builtin_popcount()`：返回括号内数的二进制表示形式中1的个数
- `__builtin_parity()`：返回括号内数的二进制表示形式中1的个数的奇偶性（偶：0，奇：1）
- `__builtin_ffs()`：返回括号内数的二进制表示形式中最后一个1在第几位（从后往前）
- `__builtin_sqrt()` / `__builtin_sqrtf()`：快速开平方，比一般的 `sqrt()` 要快，前者 8 位，后者 4 位

:warning:所有带 `ll` 的名字，均为 `long long` 类型下运算，否则将当作 `int` 来算。



### 后记

在封装这些数据结构的时候，**STL **按照程序员的使用习惯，以成员函数方式提供的常用操作，如：插入、排序、删除、查找等。让用户在**STL**使用过程中，并不会感到陌生。

