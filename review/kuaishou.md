## 社科部门

### 2021.5.20 一面

#### 项目

上来直接问项目

- HUAWEI新媒体技术
- 分布式温控系统



#### 算法

1. 大数加法，字符串形式，[基础：全是正数](https://www.nowcoder.com/questionTerminal/11ae12e8c6fe48f883cad618c2e81475)，[进阶：考虑负数](https://www.nowcoder.com/questionTerminal/1ac1af77536b4917aedaac4746eeb808)
   - 当时自己没有reverse，面试官提醒了
2. 二维数组向右向下的[最短路径](https://leetcode-cn.com/problems/minimum-path-sum/submissions/)
   - 当时自己没有考虑边界，逻辑有问题，面试官给过了



面试官人超好，提示算法题，虽然刚开始迟到了



### 2021.5.27 二面

#### 项目

一上来还是问一问项目



#### 算法

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



### 2021.5.28 三面

#### 简历

- pytorch实现过一个RNN，那个one-hot和embedding相比，有什么区别



#### 智力题

- 绳子：砍两刀成三段，可以组成三角形的概率是？

  - 初中的[线性规划](https://urlify.cn/v2eMvm)

    <img src="https://img-blog.csdn.net/20180906163902428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlZmVuZ2xpYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70" style="zoom:60%" />

- 随机数生成器：randn()随机生成1~n

  - 怎样通过rand10()得到rand7()

    通过rand10()得到1~10，我直接舍弃大于7的数 就可以了

  - 怎样通过[rand7()得到rand10()](https://urlify.cn/BZ3Q7z)

