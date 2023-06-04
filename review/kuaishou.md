## 快手

### 社科部门

#### 2021.5.20 一面

##### 1、大数加法，字符串形式

- [基础：全是正数](https://www.nowcoder.com/questionTerminal/11ae12e8c6fe48f883cad618c2e81475)，[进阶：考虑负数](https://www.nowcoder.com/questionTerminal/1ac1af77536b4917aedaac4746eeb808)
- 当时自己没有reverse，面试官提醒了

##### 2、二维数组向右向下的[最短路径](https://leetcode-cn.com/problems/minimum-path-sum/submissions/)

- 当时自己没有考虑边界，逻辑有问题，面试官给过了



#### 2021.5.27 二面

##### 1、股票问题

**基础**，[简单的一次买卖](https://leetcode-cn.com/problems/gu-piao-de-zui-da-li-run-lcof/)

```cpp
// 不持有状态
dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
// 持有状态，注意这里：只允许交易一次，因此手上的现金数就是当天的股价的相反数，参考liweiwei
dp[i][1] = max(dp[i-1][1], - prices[i]);
```

**进阶1**：[可以多次买卖](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-ii/submissions/)：

- 可以用比较好理解的DP，注意和只有一次买卖问题的区别

  ```cpp
  // 不持有状态
  dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
  // 持有状态
  dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i]);
  ```

- 另外也可以是使用贪心：只要比前一天大就买

**进阶2**：[最多两次买卖](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/)：

- 参考[代码随想录的状态定义](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii/solution/123-mai-mai-gu-piao-de-zui-jia-shi-ji-ii-zfh9/)

- 牛逼的 Trick：

  ```cpp
  class Solution {
  public:
      // 对于任意一天考虑四个变量:
      // fstBuy: 在该天第一次买入股票可获得的最大收益 
      // fstSell: 在该天第一次卖出股票可获得的最大收益
      // secBuy: 在该天第二次买入股票可获得的最大收益
      // secSell: 在该天第二次卖出股票可获得的最大收益
      // 分别对四个变量进行相应的更新, 最后secSell就是最大
      // 收益值(secSell >= fstSell)
      int maxProfit(vector<int>& prices) {
          int fstBuy = INT_MIN, fstSell = 0;
          int secBuy = INT_MIN, secSell = 0;
          
          for(int p: prices) {
              fstBuy = max(fstBuy, -p);
              fstSell = max(fstSell, fstBuy + p);
              secBuy = max(secBuy, fstSell - p);
              secSell = max(secSell, secBuy + p);
          }
          return secSell;
      }
  };
  ```


##### 2、二叉树的前序遍历，[非迭代](https://leetcode-cn.com/problems/binary-tree-preorder-traversal/)



#### 2021.5.28 三面

> **挂在三面**：区别没答好，三角形概率没答好

##### 1、PyTorch 实现过一个RNN，那个one-hot和embedding相比，有什么区别

##### 2、绳子：砍两刀成三段，可以组成三角形的概率是？

- 初中的[线性规划](https://blog.csdn.net/hefenglian/article/details/82463746)

  <img src="https://img-blog.csdn.net/20180906163902428" style="zoom:60%" />

  ##### 3、随机数生成器：randn() 随机生成1~n

  - 怎样通过 rand10() 得到 rand7()：通过 rand10() 得到 1~10，我直接舍弃大于7的数 就可以了

  - 怎样通过 [rand7() 得到 rand10()](https://blog.csdn.net/yunzhongguwu005/article/details/9191609)



### 商业化—广告引擎

> 2023.04.21 14:00-15:00

#### 一面

> 基础不太扎实，挂了

##### 1、C++ 中 vector 和 list 区别，使用场景

连续 vs 不连续

- vector 用于随机存取，list 用于高效插入和删除

- vector中的迭代器是原生态指针，也就是元素类型的指针(vs下不是，g++是)； list中的迭代器是对原生态指针(节点指针)的封装，需要重载 ++ 和 -- 运算符；



##### 2、vector 和 list 的 size() 时间复杂度

都是 O(1)，通过维护一个变量实现

- 区分 vector 的 capacity 是容量，size 是元素个数



##### 3、虚函数说一下，实例化10个对象如何找到自己的虚函数

父类指针指向子类对象，如何保证调用的子类虚函数 [参考](https://jacktang816.github.io/post/virtualfunction/)

- 一般继承时，子类的虚函数表中先将父类虚函数放在前，再放自己的虚函数指针。
- 如果子类覆盖了父类的虚函数，将被放到了虚表中**原来父类虚函数**的位置。
- 在多继承的情况下，**每个父类都有自己的虚表，子类的成员函数被放到了第一个父类的表中。**也就是说当类在多重继承中时，其实例对象的内存结构并不只记录一个虚函数表指针。基类中有几个存在虚函数，则子类就会保存几个虚函数表指针

**一个类实例化后的对象中虚函数表是共用的~** [参考](https://blog.csdn.net/kezhi9195/article/details/92562186)

:fire: 看看父类和子类虚函数表的关系：下面都是父类有虚函数的情况，

- 子类和父类的虚表不是共用的，[参考-VS调试结果](https://blog.csdn.net/qq_18127085/article/details/120022536)，[参考-图示展示可以-说明有点误差](https://www.cnblogs.com/xyf327/p/15133390.html)



##### 4、右值引用用在哪里

- 移动构造函数
- 移动赋值函数

**被声明出来的左、右值引用都是左值**，因为被声明出的左右值引用是有地址的，也位于等号左侧

```cpp
// 形参是个右值引用
void change(int&& right_value) {
    right_value = 8;
}
 
int main() {
    int a = 5; // a是个左值
    int &ref_a_left = a; // ref_a_left是个左值引用
    int &&ref_a_right = std::move(a); // ref_a_right是个右值引用
 
    change(a); // 编译不过，a是左值，change参数要求右值
    change(ref_a_left); // 编译不过，左值引用ref_a_left本身也是个左值
    change(ref_a_right); // 编译不过，右值引用ref_a_right本身也是个左值
     
    change(std::move(a)); // 编译通过
    change(std::move(ref_a_right)); // 编译通过
    change(std::move(ref_a_left)); // 编译通过
 
    change(5); // 当然可以直接接右值，编译通过
     
    cout << &a << ' ';
    cout << &ref_a_left << ' ';
    cout << &ref_a_right;
    // 打印这三个左值的地址，都是一样的
}
```

**右值引用既可以是左值也可以是右值，如果有名称就是左值，否则就是右值**，或者说：**作为函数返回值的 && 是右值，直接声明出来的 && 是左值**

```cpp
int &&ref = std::move(a); // std::move(a) 是右值，因为他没有名称，并且右值引用只能指向右值
```

:fire: 重点参考：https://zhuanlan.zhihu.com/p/335994370



##### 5、哈希表里面存什么值

存的是 pair<const Key,  T> 直接看定义方式就知道：

```cpp
template<class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class unordered_map;
```



##### 6、[LC.540-有序数组中的单一元素](https://leetcode.cn/problems/single-element-in-a-sorted-array/)

- 考虑 mid 的下标，没有多余的1个元素，**正常情况下**
  - mid 为奇数：正常情况下应该是当前和上一个元素相同
  - mid 为偶数：正常情况下应该是当前和下一个元素相同
- **无序**也是一样的思路



### 大数据引擎

#### 一面

> 2023.05.05—19:00-20:00

##### 1、[项目] 如何提升 wrk 压测



##### 2、static、const 和 volatile 关键字

volatile关键字是一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素更改，比如：操作系统、硬件或者其它线程等。遇到这个关键字声明的变量，编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问。

**volatile定义变量的值是易变的，每次用到这个变量的值的时候都要去重新读取这个变量的值，而不是读寄存器内的备份。多线程中被几个任务共享的变量需要定义为volatile类型。**



##### 3、静态多态和动态多态了解吗 [参考](https://www.cnblogs.com/lizhenghn/p/3667681.html)

动态多态就是运行时多态，动态绑定，可以处理异质对象集合

```cpp
namespace DynamicPoly
{
    class Geometry
    {
    public:
        virtual void Draw() const = 0;
    };

    class Line : public Geometry
    {
    public:
        virtual void Draw() const { std::cout << "Line Draw()\n"; }
    };

    class Circle : public Geometry
    {
    public:
        virtual void Draw() const { std::cout << "Circle Draw()\n"; }
    };

    class Rectangle : public Geometry
    {
    public:
        virtual void Draw() const { std::cout << "Rectangle Draw()\n"; }
    };

    void DrawGeometry(std::vector<DynamicPoly::Geometry *> vecGeo)
    {
        const size_t size = vecGeo.size();
        for (size_t i = 0; i < size; ++i)
            vecGeo[i]->Draw();
    }
}

DynamicPoly::Line line;
DynamicPoly::Circle circle;
DynamicPoly::Rectangle rect;

std::vector<DynamicPoly::Geometry*> vec;
vec.push_back(&line);
vec.push_back(&circle);
vec.push_back(&rect);
DynamicPoly::DrawGeometry(vec);
```



静态多态是编译器完成的，效率高，但是不能处理异质对象集合，本质是模板的具现化，静态多态中的接口调用也叫隐式接口

```cpp
namespace StaticPoly
{
    class Line
    {
    public:
        void Draw() const { std::cout << "Line Draw()\n"; }
    };

    class Circle
    {
    public:
        void Draw(const char *name = NULL) const { std::cout << "Circle Draw()\n"; }
    };

    class Rectangle
    {
    public:
        void Draw(int i = 0) const { std::cout << "Rectangle Draw()\n"; }
    };

    template <typename Geometry>
    void DrawGeometry(std::vector<Geometry> vecGeo)
    {
        const size_t size = vecGeo.size();
        for (size_t i = 0; i < size; ++i)
            vecGeo[i].Draw();
    }
}

StaticPoly::Line line1;
StaticPoly::Line line2;
StaticPoly::Circle circle;
StaticPoly::Rectangle rect;

std::vector<StaticPoly::Line> vecLines;    
vecLines.push_back(line1);
vecLines.push_back(line2);
// vecLines.push_back(&circle);  //编译错误，已不再能够处理异质对象
// vecLines.push_back(&rect);    //编译错误，已不再能够处理异质对象
```



##### 4、C++ 垃圾回收器了解吗

- 引用计数算法
- Mark & Sweep 算法
- 节点复制算法

参考：https://blog.csdn.net/u012611878/article/details/78947267



##### 5、MySQL倒排索引了解吗 [参考](https://blog.csdn.net/m0_46405589/article/details/113603721)

- 全文检索通常使用 **倒排索引（inverted index）** 来实现
- 倒排索引在辅助表（auxiliary table）中存储了单词与单词自身在一个或多个文档中所在位置之间的映射这通常利用关键数组实现，其拥有两种表现形式：
  - inverted file index：其表现形式为{单词，单词所在文档的ID}
  - full inverted index：其表现形式为{单词，(单词所在文档的ID，在文档中的具体位置)}



##### 6、描述一下红黑树，会手撕插入删除过程吗

性质1：每个节点要么是黑色，要么是红色

性质2：根节点是黑色

性质3：**每个叶子节点（NIL）是黑色** （这个其实不那么严谨，其实就是黑高

性质4：每个红色结点的两个子结点一定都是黑色

性质5：任意一结点到每个叶子结点的路径都包含数量相同的黑结点

> 参考：https://www.jianshu.com/p/e136ec79235c



##### 7、三个线程轮流打印 ABC [LC55. 跳跃游戏](https://leetcode.cn/problems/jump-game/)

用一个 int 变量控制条件变量 wait() 阻塞等待时机，用 notify_all() 唤醒条件变量

参考：https://blog.csdn.net/qq_41963107/article/details/108306090



跳跃游戏：[贪心覆盖](https://leetcode.cn/submissions/detail/430028029/)



#### 二面

> 2023.05.08—20:00-21:00

##### 1、聊项目细节

- 视野预测流程
- 项目难点和收获



##### 2、矩阵转置

- 最好给出参数化的函数
- 原地转置？额外空间转置



##### 3、如何存储海量整数，使用尽可能少的存储空间

- 不考虑数据顺序，可以集合存储，存储数据以及出现的个数，key-->cnt
- 考虑数据的顺序，变长编码（Trie 数？哈夫曼编码？）
