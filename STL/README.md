# C++ STL
**C++ STL** 之所以得到广泛的赞誉，也被很多人使用，不只是提供了像`vector`, `string`, `list`等方便的容器，更重要的是**STL**封装了许多复杂的数据结构算法和大量常用数据结构操作。



## vector

- [*vector.cpp*](./vector.cpp)：**vector**封装数组: 便于随机访问
- [*emplace_vs_push.cpp*](./emplace_vs_push.cpp)：**emplace_back**和**push_back**的区别：好像是前者效率高



## list

- **list**封装了链表: 便于插入和删除



## string

- *[string.cpp](./string.cpp)*：**string**基本用法



## queue vs deque

- 两者最主要的区别是：具体参考 [queue_vs_deque.md](queue_vs_deque.md)
  - deque支持push_front、pop_front、push_back、pop_back
  - queue支持push（实际上就是pop_back）、pop（实际上就是pop_front）
- *[priority_queue.cpp](./priority_queue.cpp)*：优先队列用法，默认是大顶推



## map vs set

- **map**封装了二叉树等 
    - *[mapSTL.cpp](./mapSTL.cpp)*：**map**底层是一个红黑树，有序的（按照key排序），一般处理有序的情况
    - *[unordered_map.cpp](./unordered_map.cpp)*: **unordered_map** 底层是一个哈希表，便于查找，但是内存占用比较高
    - **map** 和 **unordered_map**外部操作都是一样的，只是内部实现不一样
- **set**也是封装了二叉树，基于红黑树实现
    - 和**map**的区别就是将value作为key，map的key和value是分开的；map允许修改value不允许修改key，set的迭代器是const，不允许修改元素的值
    - *[setSTL.cpp](./setSTL.cpp)*：**multiset**用法，和**set**类似，只是前者可以保存元素，后者不保存，默认时升序排列的也就是 **multiset<int, less\<int\>>**



## sort

- 自定义sort函数：*[sort1.cpp](./sort1.cpp)*：使用**lambda表达式**和**函数**；*[sort2.cpp](./sort2.cpp)*：使用**模板**和**函数**

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



## advance

- *[advance.cpp](./advance.cpp)*：实现迭代器的加减操作



在封装这些数据结构的时候，**STL**按照程序员的使用习惯，以成员函数方式提供的常用操作，如：插入、排序、删除、查找等。让用户在**STL**使用过程中，并不会感到陌生。

