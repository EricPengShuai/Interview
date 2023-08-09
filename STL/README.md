## C++ STL

STL 全称是 Standard Template Library之所以得到广泛的赞誉，也被很多人使用，不只是提供了像`vector`, `string`, `list`等方便的容器，更重要的是**STL**封装了许多复杂的数据结构算法和大量常用数据结构操作。

STL 有六大部件：容器（Containers）、分配器（Allocators）、算法（Algorithms）、迭代器（Iterators）、适配器（Adapters）、仿函式（Functors）

上述六大部件中只有算法是函数模板（function template），其余都是类模板（class template）



### 容器|Container

#### 序列式容器|Sequence Container

##### vector

- [*vector.cpp*](vector.cpp)：动态扩容的数组

- GUN 2.9 源码中使用三根指针 start, finish, end of storage 维护 vector 中元素，如果 push_back 之后 finish 超过了 end of storage 之后需要 1.5/2 倍扩容，因此 sizeof(vector\<int>) = 12 (32 位) /24 (64 位)

- [*emplace_vs_push.cpp*](emplace_vs_push.cpp)：emplace_back 使用场景多余 push_back，[参考1](https://blog.csdn.net/iaibeyond/article/details/119153008) [参考2](https://ppipp.blog.csdn.net/article/details/84764104)
  - 在接受 `T, T&, T&&`作为参数时，empalce_back 和 push_back 没有区别，分别调用普通构造函数、拷贝构造函数和移动构造函数
  - emplace_back 支持传入 class T 的构造函数的参数，并且此时**没有任何临时变量生成**，但是 push_back 不支持



##### array

- 大小固定的数组
- GUN 2.9 源码和 vector 类似，甚至更简单，因为没有扩容的操作，sizeof(array<int, 10>) = 12 (32 位) / 24 (64 位)



##### deque

- deque 是双端队列，queue 和 stack 底层默认通过 deque 实现
  - deque 支持 push_front、pop_front、push_back、pop_back
  - queue 支持 push（实际上就是 push_back）、pop（实际上就是 pop_front），queue 底层还可以通过 list 实现
  - stack 支持 push（实际上就是 push_back）、pop（实际上就是 pop_back），stack 底层还可以通过 vector 和 list 实现
  
- GUN 2.9 源码中 deque 通过 start, finish, map, map_size 维护其中元素节点，每个节点通过四个指针 cur, first, last, node 维护相关信息，deque 内部是「**分段连续**」的，通过 deque::iterator 重载 ++、--、+= 运算符实现连续访问

  ```cpp
  class deque { // sizeof(deque<int>) = 2*16 + 2*4 = 40 (32位)
  // ...
  protected:
      iterator start;
      iterator finish;
      map_pointer map; // T **
      size_type map_size;
  // ...
  }
  
  struct __deque_iterator { // sizeof(deque<int>::iterator) = 4*4 = 16 (32位)
  // ...
      T* cur;
      T* first;
      T* last;
      map_pointer node; // T**
  // ...
  }

- *[priority_queue.cpp](priority_queue.cpp)*：优先队列用法
  
  - 默认是 `less<int>`：大顶堆，为什么升序却是大顶堆呢，[因为堆排序一个最大堆才会得到升序](https://www.zhihu.com/question/343715076)
  - [数据结构与算法--排序算法：堆排序 最大堆（大顶堆）和 最小堆（小顶堆）](https://blog.csdn.net/storyfull/article/details/102925462)



##### list/forward_list

- forward_list 是单向链表，list 是双向（循环）链表，两者都是序列容器，可以在 O(1) 时间内插入和删除元素
- GUN 2.9 源码中 list 通过一个指针 `list_node* node` 维护元素，sizeof(list\<int>) = 4 (32 位) / 8 (64 位)，list::iterator 中有两个指针 next, prev 和一个 data
- *[list.cpp](list.cpp)*：list 和 forward_list 的基本用法
  - splice 转移函数，LeetCode LRU 经典题目用法：[146. LRU 缓存](https://leetcode.cn/problems/lru-cache/)



#### 关联式容器|Associative Container


##### map/multimap

*[mapSTL.cpp](mapSTL.cpp)*：`map`底层是一个红黑树，有序的（按照`key`排序），一般处理有序的情况



##### set/multiset

- 和 `map` 的区别就是将 `value` 作为 `key`，`map` 的 `key` 和 `value` 是分开的；`map`允许修改 `value` 不允许修改 `key`，`set` 的迭代器是 `const`，不允许修改元素的值

- set 中 key 和 value 是一样的

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



#### 无序容器|Unordered Container

##### unordered_map/unordered_multimap

*[unordered_map.cpp](unordered_map.cpp)*: `unordered_map` 底层是一个哈希表，便于查找，但是内存占用比较高

- 手撕哈希表：[hashtable.h](hashtable.h)  [手撕哈希表详解](https://blog.csdn.net/m0_50945504/article/details/121557561)
- `map` 和 `unordered_map` 外部操作都是一样的，只是内部实现不一样



##### unordered_set/unordered_multiset

底层是哈希表，和 unordered_map 类似，只是它的 key 和 value 一样 

 

万用 hash function，TR1 中使用 variadic template 处理可变参数

```cpp
template<typename T>
inline void hash_combine(size_t& seed, const T& val) {
    seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T>
inline void hash_val(size_t& seed, const T& val) {
    hash_combine(seed, val);
}

// 特化
template<typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val, const Types&... args) {
    hash_combine(seed, val);
    hash_val(seed, args...);
}

// 泛化
template<typename... Types>
inline size_t hash_val(const Types&... args) {
    size_t seed = 0;
    hash_val(seed, args...);
    return seed;
}
```





### 分配器|Allocator

每个容器都有默认的分配器，每个容器都是模板类，模板类中有一个默认参数就是 std::allocator，当然还有其他的分配器，比如 `__gun_cxx::malloc_allocator` 等等这些不在标准库中，在扩展库里面，分配器底层通过 malloc 和 free 管理内存，建议不要直接分配器而是通过容器间接使用，因为单独使用分配器需要 allocator 和 deallocator 比较复杂。

operator new() 底层就是调用 malloc 来实现，一般 mallo(size) 的空间会带有额外的开销，例如头尾带有空间大小的指针，这里统称为 overhead，如果 malloc 空间越大，overhead 比例就越小；如果 malloc 空间越小，overhead 比例就越大

- VC 6+/BC++/GCC 2.9 标准库中的 allocator 只是 ::operator new 和 ::operator delete 完成 allocate() 和 deallocate()，没有任何特殊设计
- G2.9 STL 使用默认的分配器为 alloc，这个比 allocator 要优化一点，通过链表负责不同大小的区块，以 8 的倍速增长，这样每次分配的空间就不带 overhead
- G4.9 STL 使用默认的分配器为 std::allocator，其中使用了 `__gun_cxx::new_allocator` ，其实 new_allocator 就是调用 ::operator new 和 ::operator delete
- G4.9 标准库中还有很多扩展的 allocators，其中 `__pool_alloc` 就是 G2.9 的 alloc



通过 **迭代器的萃取器** 可以获取 iterator / native pointer 的五个特性，前三个比较重要：

- iterator_category：单向、双向还是随机迭代器
- value_type：迭代器所指对象的型别
- difference_type：两个迭代器之间的距离
- pointer：迭代器所指向的型别
- reference：迭代器所引用的型别

还有其他 traits，例如 type traits

```cpp
// GUN 2.9 泛化版本，trivial 是平凡的、不重要的
tempplate <class type>
struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __false_type is_POD_type; // plain old data
}

// C++ 11 新版本提供很多函数判别 type traits，通过「泛化」helper 和很多「特化」helper 实现
// remove_cv 就是 remove_const 和 remove_volatile
template<typename _Tp>
strcut is_void: public __is_void_helper<typename remove_cv<_Tp>::type>::type {};
```





### 算法|Algorithm

Algorithms 看不见 Containers，对其一无所知；所以它所需要的一切信息都必须从 Iteratrors 取得，而 Iterators （由 Containers 供应）必须能够回答 Algorithm 的所有提问，才能搭配该 Algorithm 的所有操作

iterator_category 对算法的影响：

- distance/advance 是一个函数模板，根据迭代器的类型选择怎样的计算方式
- copy 实现源码中使用了泛化、特化和强化等思想可能高效的拷贝，并结合 Iterator/Type Traits 萃取器询问 迭代器的性质

算法源码一般对 Iterator_category 有“暗示”，比如 sort 函数模板名写的是 “RamdomAccessIterator” 就暗示 sort 适用为具有随机访问迭代器的容器

#### replace

- replace(first, last, old_value, new_value): 将 [first, last) 范围内等于 old_value 的值都以 new_value 取代
- replace_if(first, last, pred, new_value): 将范围内所有满足 pred() 为 true 的元素都以 new_value 取代
- replace_copy(first, last, result, old_value, new_value): 范围内所有等于 old_value 者都以 new_value 放至新区间

#### count

- count(first, last, value): 统计 [first, last) 中等于 value 的元素个数
- count_if(first, last, pred): 统计 [first, last) 中 pred() 为 true 的元素个数
- 序列式容器内部不含有 count 成员函数，关联式容器有 count 成员函数

#### find

- find(first, last, value): 找到 [first, last) 中等于 value 的元素的迭代器并返回

- find_if(first, last, pred): 找到 [first, last) 中满足 pred() 为 true 的元素的迭代器并返回
- 序列式容器内部不含有 find 成员函数，关联式容器有 find 成员函数

#### sort

- 自定义sort函数：*[sort1.cpp](sort1.cpp)*：使用**lambda表达式**和**函数**；*[sort2.cpp](sort2.cpp)*：使用**模板**和**函数**

- 关联式容器自然形成 sorted 状态，不需要 sort，也没有 sort 成员函数

- 序列式容器只有 list 和 forward_list 具有 sort 成员函数，而且只能调用成员函数，因为算法库的 sort 要求 random access Iterator，其他的序列式容器 array、vector 和 deque 不具有 sort 成员函数，可以调用 sort 算法库

- 主要有三种自定义比较方式的写法：注意自定义的函数最好**不要加等号** :warning:
    1. 模板
       ```cpp
       template <typename T>
       struct cmp1 {
           bool operator()(const T &x, const T &y) {
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



### 迭代器|iterator

#### 定义

- 正向迭代器：`容器类名::iterator 迭代器名;`
- 常量正向迭代器：`容器类名::const_iterator 迭代器名;`
- 反向迭代器，`容器类名::reverse_iterator 迭代器名;`
- 常量反向迭代器，`容器类名::const_reverse_iterator 迭代器名;`

#### 分类

```cpp
// 五种 iterator category
struct input_iterator_tag {};  // istream_iterator<int>()
struct output_iterator_tag {}; // ostream_iterator<int>(cout, "") 

struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_iterator_tag : public bidirectional_iterator_tag {};
```

- 正向迭代器：支持 ++p, p++, *p 等操作，还可以用 == 和 != 比较，**但是不能用 < 或者 > 比较**
- 双向迭代器：只有正向迭代器的全部功能，另外还支持 --p, p-- 操作，**也不能用 < 或者 > 比较**
- 随机访问迭代器：具有双向迭代器的全部功能，另外还支持 `p += i`, `p -= i`, `p+i`, `p-i` 操作，p[i]：返回 p 后面第 i 个元素的引用，其中 i 是一个整数，**支持用 < 或者 > 比较** [参考](http://c.biancheng.net/view/338.html)

| 容器                                   | 迭代器         |
| -------------------------------------- | -------------- |
| array、vector、deque                   | 随机访问迭代器 |
| stack、queue、priority_queue           | 无             |
| list、(multi)set/map                   | 双向迭代器     |
| unordered_(multi)set/map、forward_list | 前/正向迭代器  |

#### 移动

- [advance.cpp](advance.cpp)：实现迭代器的加减操作，没有返回值
- [prev_next.cpp](prev_next.cpp)：prev 原意为“上一个”，next 原意为“下一个”，是有返回值的
- `distance(p, q)`：计算两个迭代器之间的距离，即迭代器 p 经过多少次 + + 操作后和迭代器 q 相等。如果调用时 p 已经指向 q 的后面，则这个函数会陷入死循环
- `iter_swap(p, q)`：用于交换两个迭代器 p、q 指向的值

#### 失效问题

1. vector 插入和删除都可能导致迭代器失效，因为插入可能导致扩容 [参考](https://blog.csdn.net/Ferlan/article/details/86423563)
2. list 删除时可能导致失效，插入不会导致扩容

**原因**：对于 vector 而言

- 当容器调用 erase 方法后，当前位置到容器未尾元素的所有的迭代器全部失效了
- 当容器调用 insert 方法后，当前位置到容器末尾元素的所有的迭代器全部失效了
- 对于 insert 来说，如果引起容器内存扩容，原来容器的所有的迭代器就全部失效了

**解决**：利用返回值

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



### 仿函数|Functor

仿函数就是一个模板类，也是函数对象，有如下好处：[functor.cpp](functor.cpp)

1. 通过函数对象调用 operator()，可以省略函数的调用开销，比通过函数指针调用（无法 inline **内联调用**）效率高
2. 函数对象使用类生成，所以可以添加相关的成员变量，用来记录函数对象使用时更多的信息

GUN 2.9 有 24 个仿函数，一般都会继承一些「适当的模板函数」，这样才能让仿函数（functor）具有可适配（adaptable）条件，才能让仿函数 “回答问题”

```cpp
// 算术类
template<class T>
struct plus: public binary_functino<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x + y;
    }
};

template<class T>
struct minus: public binary_functino<T, T, T> {
    T operator()(const T& x, const T& y) const {
        return x - y;
    }
};

// 逻辑运算类
template<class T>
struct logical_ans: public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x && y;
    }
};

// 相对关系类
template<class T>
struct equal_to: public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x == y;
    }
};

template<class T>
struct greater: public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x > y;
    }
};

template<class T>
struct less: public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const {
        return x < y;
    }
};
```



### 适配器|Adapter

适配器可以叫做“改造器”，一般有容器适配器（Container Adapter）、迭代器适配器（Iterator Adapter）以及仿函数适配器（Functor Adapter），把改造的东西记下来，然后思考如何修改它

#### Container Adapter

- stack 和 queue 就是 deque 的适配器，默认是 queue，因为 queue 对内存的利用率要高，queue 只需要**分段连续**的内存（）
- priority_queue 底层默认是 vector，因为大/小根堆通过下标之间的关系维护这个堆结构，需要在**内存连续**的数组上构建一个大根堆或者小根堆，deque 是分段连续的，实现没有 vector 方便
- 通过使用 deque 已有的一些成员数实现 stack、queue 的功能



#### Functor Adapter

- bind2nd：将第二个参数绑定为一个指定的值，其中有很多「询问」的过程，绑定之后只有一个参数，

  ```cpp
  // 询问 less 的第 1,2,3 个实参类型，这时候 less 能给出就回答就是因为它是 adaptable 的
  bind2nd(less<int>(), 40); // less<int>(x, 40) 即 x < 40 
  ```

- not1: 将原有的 pred 取反

- bind: 新型 bind2nd，可以绑定 functions、function object、member functions 和 data functions

  ```cpp
  double my_divide(double x, double y) {
      reutrn x / y;
  }
  
  // 绑定参数
  using namespace std::palceholders; // adds visibility of _1, _2, _3, ...
  auto fn_five = bind(my_divide, 10, 2); // return 10/2, 5
  auto fn_half = bind(my_divide, _1, 2); // return x/2
  auto fn_invert = bind(my_divide, _2, _1); // reutrn y/x
  auto fn_rounding = bind<int>(my_divide, _1, _2); // return int(x / y);
  ```



#### Iterator Adapter

- reverse_iterator: 实现逆向迭代器的取值就是重载 `operator*()` 操作符，就是正向迭代器 -- 然后取值

- insert_iterator: 如何将 assign 变为 insert 行为，重载 `operator=()` 操作符，将复制操作改为 Container 的 insert 方法

  ```cpp
  list<int> foo, bar;
  for (int i = 1; i <= 5; ++ i) {
      foo.push_back(i);
      bar.push_back(10*i);
  }
  
  list<int>::iterator it = foo.begin();
  advance(it); // *it = 4
  
  // foo: 1 2 3 4 5
  // bar: 10 20 30 40 50
  r
  // 重载 operator=() 为 vector.insert(iter，value) 操作
  copy(bar.begin(), bar.end(), inserter(foo, it)); 
  
  // copy bar --> it
  // foo: 1 2 3 10 20 30 40 50 4 5
  ```



#### X Adapter

- ostream_iterator: `ostream_iterator<int> out_it(cout, ",")` 将迭代器弄到 cout 中（输出到屏幕上），第二参数为分隔符

  ```cpp
  std::ostream_iterator<int> out_it(std::cout, ",");
  std::copy(vec.begin(), vec.end(), out_it); // 重载 copy 的 operator=()  
  ```

- istream_iteraor: 改变 cin 的行为

  ```cpp
  std::istream_iterator<double> eos; // end-of-stream iterator (instream(0))
  // 已经开始等待输入了
  std::istream_iterator<double> iit(std::cin); // stdint iterator (++*this)
  ```

  



### 其他标准库

#### string

- *[string.cpp](string.cpp)*：**string**基本用法
- GUN 2.9 中的 string 的 hashfunction 为 h = h + 5*c



#### thread

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



#### unique

该函数的作用是“去除”容器或者数组中相邻元素的重复出现的元素，注意

(1) 这里的去除并非真正意义的erase，而是将重复的元素放到容器的**末尾**，返回值是去重之后的尾地址。

(2) unique 针对的是相邻元素，所以对于顺序顺序错乱的数组成员，或者容器成员，需要先进行排序，可以调用std::sort()函数

```cpp
std::vector<int> vt;
std::sort(vt.begin(), vt.end());	// 排序
vt.erase(unique(vt.begin(), vt.end()), vt.end());	// 去重
```



#### move

std::move 并不会真正地移动对象，真正的移动操作是在**移动构造函数、移动赋值函数**等完成的，std::move 只是将参数转换为右值引用而已（相当于一个 static_cast）

更确切的说：move 的本质就是帮助编译器选择重载函数, 告诉编译器 "请尽量把此参数当做右值来处理"

参考代码： [move.cpp](move.cpp) 

- [c++ move函数到底是什么意思？](https://www.zhihu.com/question/64205844)
- [C++:move，带你从根本理解move函数是什么](https://blog.csdn.net/qq_51568363/article/details/124285294)
- https://learn.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?redirectedfrom=MSDN&view=msvc-170
- https://zh.cppreference.com/w/cpp/utility/move



#### forward

假如我们封装了一个操作，主要是用来创建对象使用（类似设计模式中的工厂模式），这个操作如下：

1. 可以接受不同类型的参数，然后构造一个对象的指针
2. 性能尽可能高

```cpp
template<typename T>
CData* Creator(T t) // 传值，每次都要拷贝内存，效率低
{
    return new CData(t);
}

template<typename T>
CData* Creator(T& t) // 无法传递右值
{
    return new CData(t);
}

template<typename T>
CData* Creator(T&& t) // 可以传递右值
{
    return new CData(t); // 传右值但还是无法调用移动构造函数，t 为左值
}

template<typename T>
CData* Creator(T&& t) // 实现完美转发
{
    return new CData(std::forward<T>(t));
}
```

**std::forward会将输入的参数原封不动地传递到下一个函数中，这个“原封不动”指的是，如果输入的参数是左值，那么传递给下一个函数的参数的也是左值；如果输入的参数是右值，那么传递给下一个函数的参数的也是右值**

- [C++ 理解std::forward完美转发](https://blog.csdn.net/xiangbaohui/article/details/103673177)

- [一文读懂C++右值引用和std::move](https://zhuanlan.zhihu.com/p/335994370)

  

#### __builtin

- `__builtin_ctz()` / `__builtin_ctzll()`：返回括号内数的二进制表示形式中末尾0的个数
- `__buitlin_clz()`  / `__buitlin_clzll()`：返回括号内数的二进制表示形式中前导0的个数（leading zero）
- `__builtin_popcount()`：返回括号内数的二进制表示形式中1的个数
- `__builtin_parity()`：返回括号内数的二进制表示形式中1的个数的奇偶性（偶：0，奇：1）
- `__builtin_ffs()`：返回括号内数的二进制表示形式中最后一个1在第几位（从后往前）
- `__builtin_sqrt()` / `__builtin_sqrtf()`：快速开平方，比一般的 `sqrt()` 要快，前者 8 位，后者 4 位

:warning: 他是 GCC 内建函数，所有带 `ll` 的名字，均为 `long long` 类型下运算，否则将当作 `int` 来算。



### 后记

在封装这些数据结构的时候，**STL **按照程序员的使用习惯，以成员函数方式提供的常用操作，如：插入、排序、删除、查找等。让用户在**STL**使用过程中，并不会感到陌生。

参考：《C++ STL 标准库和范型编程》——侯捷
