## 游戏类公司

### 米哈游—0319笔试

#### 选择题

1. 二叉树、树和森林之间的相互转换：

   - 树转二叉树：编号之后，首个子节点边为左子节点，**其余兄弟节点变为右子节点**
   - 森林转二叉树：先将每个树转二叉树，然后第一个二叉树不动，其余二叉树根节点作为**右子节点**接到上一个二叉树根节点处
   - 二叉树转树：按照“树转二叉树”逆向操作
   - 二叉树转森林：**前提是这个二叉树的根节点有右子节点**，否则的话就只能转换成一棵树；如果有右子节点就将“所有右子节点”全部断开，这就转换成 n 个二叉树，然后将这 n 个二叉树分别转换成 n 个树
   
   > 参考：https://www.cnblogs.com/zhanggui/p/4649114.html

2. 基数排序：按照**低位**先排序，然后收集；再按照高位排序，然后再收集；依次类推，直到最高位。又称为 [桶子法](https://zq99299.github.io/dsalg-tutorial/dsalg-java-hsp/07/09.html)，是稳定的

3. 移位：

   - 对于正数：不管左移还是右移，缺失的数位都补 0，因为正数的原码、反码、补码都是一样的
   - 对于负数：左移补0，右移补1（**左0右1**），因为负数的反码是其原码符号位不变其余取反，负数的补码就是其反码加1（负数一般使用补码表示）

   > 【-10 >> 4 == -1】-10 原码是 1 1010，反码是 1 0101，补码是 1 0110，补码右移四位 1 1111（补四个1）

4. running 变为 ready 状态两个条件：时间片用完或者被高优先级的进程抢占，另外注意 waiting 状态是进程**主动请求**才能达到，例如进程用”系统调用“的方式申请某种系统资源或者”请求等待“某个事件发生才会有 running 变为 waiting



#### 算法T1—升级版的岛屿数量

> [200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/)：查找连通块的数量

```bash
# 样例
# 输入：
2 6 	# 接下来输入 2行6列 的矩阵
RRGGBB	# R G B 分别表示红、绿、蓝三种颜色的格子
RGBGRR
# 输出：
6	# 6 个连通块，颜色不同记为一个连通块
```

- 理解一下 DFS 何时 return，要知道的是这里一个 DFS 就是对**某个特定格子**以及**周围相同颜色**的格子标记，所以遇到不是该格子就直接 return
- 所以另外也还需要一个二重循环遍历所有格子，**枚举所有颜色的格子**，统计的就是有打了多次标记（几次 DFS），好好理解一下！！！笔试的时候没有认真思考就会陷入 segmentation fault
- 另外对于并查集也就是类似的思路，并查集就是合并两个节点，什么时候合并呢？那肯定是**颜色相同**时候才合并，然后统计所有节点的 parent 节点，有几个不同的 parent 就有多少个连通块

> 代码参考： [并查集](code/mihoyo_0319_1_uf.cpp)  [DFS](code/mihoyo_0319_1_dfs.cpp)



#### 算法T2—消除字符串

1e3个询问，每个询问给两个字符串(长度1e3)，问能否通过对第一个串进行任意次操作使得第一个串变成第二个串。每次操作可以从以下两个中选择一个。

1. 删除一个子序列mhy，mahbyc => abc

2. 插入一个子序列mhy，abc => mahbyc

这个操作的本质就是说**mhy三个字符可以任意移动**。以m为例 `m_____ => m__m___hy => __m___`

因此首先把mhy三个字符分离出来，不含mhy的字符串必须相等，然后就是匹配mhy。因为最后可以删除，所以可以先给原来的串后面加上足够多的mhy，最后必须使得剩下的mhy数目相等(这样才可以使用操作1删干净)，单次询问复杂度 O(n)

> 当时想的时候思路错了，以为 ymh 这个顺序不对是无法匹配的，但是可以通过加入 mhy 之后变成 mhymhy 然后再删除2次完成匹配，没有理解题目的**本质**，代码参考：[mihoyo_0319_2.cpp](code/mihoyo_0319_2.cpp)



#### 算法T3—升级版的子集

> [2597. 美丽子集的数目](https://leetcode.cn/problems/the-number-of-beautiful-subsets/)：数据中等使用 DFS 爆搜也可以，经典应该使用 DP，参考打家劫舍

给一个长度小于1e5的集合，元素小于等于1e6，元素不重复，问存在多少大小大于等于2的子集，使得子集中元素两两互为倍数。

子集中元素两两互为倍数，意思是子集排序后一定是如下状态 a,ab,abc ... 也就是说后一个一定是前一个的倍数。

这样就可以DP了，DP(i)表示集合最后一个元素为i的方案数。复杂度是调和级数乘上n的前缀和，约等于nlogn (n=1e6)。由于要求的是大于等于2的集合，因此还要把大小为1的删掉。

> 感觉有点抽象：`dp[i*j] += dp[i]` ，代码参考：[mihoyo_0319_3.cpp](code/mihoyo_0319_3.cpp)



### 网易游戏—测开

> 2023.11.17 - 15:50-16:40

- this 指针有什么用

- mysql 索引作用、索引分类

- **设计一个排行榜：用户id以及score，排行榜显示最多 50000 条数据，按照 score 逆序排列**

  ```cpp
  struct User {
      int userId;
      int score;
  
      User(int id, int s) : userId(id), score(s) {}
  
      // 自定义 less 小于重载，注意需要 const 修饰
      bool operator<(const User& other) const {
          if (score == other.score) {
              return userId < other.userId; // 升序
          } else {
              return score > other.score; // 逆序
          }
      }
  };
  
  // 第二种自定义<重载方法
  bool operator<(const Item& a, const Item& b) {
      return a.score > b.score; // 逆序
  }
  
  int main() {
      std::set<User> leaderboard = {{1, 150}, {2, 120}, {3, 180}, {5, 120}, /*...*/};
      
      // 输出排行榜
      for (const auto& user : leaderboard) {
          std::cout << "User ID: " << user.userId << ", Score: " << user.score << std::endl;
      }
  }
  ```

- **排行榜功能如何测试呢**

  > 基本排行榜功能测试、边界条件测试、性能测试、线程安全测试、日志测试

- 问游戏经历：三消、射击、MOBA

> 奇特的面试体验，竟然还问王者连招顺序
