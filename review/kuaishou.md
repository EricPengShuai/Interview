## 快手

### 社科部门

#### 2021.5.20 一面

##### 项目

上来直接问项目

- HUAWEI新媒体技术
- 分布式温控系统



##### 算法

1. 大数加法，字符串形式，[基础：全是正数](https://www.nowcoder.com/questionTerminal/11ae12e8c6fe48f883cad618c2e81475)，[进阶：考虑负数](https://www.nowcoder.com/questionTerminal/1ac1af77536b4917aedaac4746eeb808)
   - 当时自己没有reverse，面试官提醒了
2. 二维数组向右向下的[最短路径](https://leetcode-cn.com/problems/minimum-path-sum/submissions/)
   - 当时自己没有考虑边界，逻辑有问题，面试官给过了



面试官人超好，提示算法题，虽然刚开始迟到了



#### 2021.5.27 二面

##### 项目

一上来还是问一问项目

##### 算法

1. 股票问题，[简单的一次买卖](https://leetcode-cn.com/problems/gu-piao-de-zui-da-li-run-lcof/)

   ```cpp
   // 不持有状态
   dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
   // 持有状态，注意这里：只允许交易一次，因此手上的现金数就是当天的股价的相反数，参考liweiwei
   dp[i][1] = max(dp[i-1][1], - prices[i]);
   ```

   - 进阶1：[可以多次买卖](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-ii/submissions/)：

     - 可以用比较好理解的DP，注意和只有一次买卖问题的区别

       ```cpp
       // 不持有状态
       dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
       // 持有状态
       dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i]);
       ```

     - 另外也可以是使用贪心：只要比前一天大就买

   - 进阶2：[最多两次买卖](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/)：

     - 有点难度的三维DP

     - 牛逼的Trick：

       ```cpp
       class Solution {
       public:
          	/**
             对于任意一天考虑四个变量:
             fstBuy: 在该天第一次买入股票可获得的最大收益 
             fstSell: 在该天第一次卖出股票可获得的最大收益
             secBuy: 在该天第二次买入股票可获得的最大收益
             secSell: 在该天第二次卖出股票可获得的最大收益
             分别对四个变量进行相应的更新, 最后secSell就是最大
             收益值(secSell >= fstSell)
           **/
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

       

2. 二叉树的前序遍历，[非迭代](https://leetcode-cn.com/problems/binary-tree-preorder-traversal/)



#### 2021.5.28 三面

##### 简历

- pytorch实现过一个RNN，那个one-hot和embedding相比，有什么区别
- **挂在三面**：区别没答好，三角形概率没答好

##### 智力题

- 绳子：砍两刀成三段，可以组成三角形的概率是？

  - 初中的[线性规划](https://blog.csdn.net/hefenglian/article/details/82463746)

    <img src="https://img-blog.csdn.net/20180906163902428" style="zoom:60%" />

- 随机数生成器：randn() 随机生成1~n

  - 怎样通过 rand10() 得到 rand7()

    通过 rand10() 得到 1~10，我直接舍弃大于7的数 就可以了

  - 怎样通过 [rand7() 得到 rand10()](https://blog.csdn.net/yunzhongguwu005/article/details/9191609)



### 商业化—广告引擎

> 2023.04.21 14:00-15:00

#### 八股

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
- 在多继承的情况下，**每个父类都有自己的虚表，子类的成员函数被放到了第一个父类的表中。**，也就是说当类在多重继承中时，其实例对象的内存结构并不只记录一个虚函数表指针。基类中有几个存在虚函数，则子类就会保存几个虚函数表指针

**一个类实例化后的对象中虚函数表是共用的~** [参考](https://blog.csdn.net/kezhi9195/article/details/92562186)



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



#### 算法

##### [540. 有序数组中的单一元素](https://leetcode.cn/problems/single-element-in-a-sorted-array/)

- 考虑 mid 的下标，没有多余的1个元素，**正常情况下**
  - mid 为奇数：正常情况下应该是当前和上一个元素相同
  - mid 为偶数：正常情况下应该是当前和下一个元素相同
- **无序**也是一样的思路

