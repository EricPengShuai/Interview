## 数位DP

### 0. 概念

数位DP其实就是一种优化之后的暴力枚举方法，有点类似于记忆化搜索。字面意思是就是在数位上进行DP枚举，对于数字的每一位进行枚举，通过相应的约束条件看这次枚举是否符合题意。

对于有一类题型：给定闭区间 \[l, r\]，需要求出区间中满足某种条件的数的总数，这个区间如果比较大，是无法通过简单的暴力枚举解决的，这时候就需要使用数位DP加以优化，具体通过 [2376. 统计特殊整数](https://leetcode.cn/problems/count-special-integers/) 来理解一下，主要参考灵神的 [视频讲解](https://www.bilibili.com/video/BV1rS4y1s721/?vd_source=286032bc2c5715c8b50b608028ce57df) 



**题目**：如果一个正整数每一个数位都是 **互不相同** 的，我们称它是 **特殊整数** 。给你一个 **正** 整数 `n` ，请你返回区间 `[1, n]` 之间特殊整数的数目。



**题解**：

将 n 转换成字符串 s，定义 `f(i, mask, isLimit, isNum)` 表示数位的构造，即构造从左往右第 i 位及其之后数位的合法方案数，其中每个参数的含义为：

- `i`：表示此时构造的位置

- `mask`：表示前面选过的数字集合，也就是说第 `i` 位要选择的数字不能再集合中。注意这里是通过二进制来表示集合的，因为**二进制与集合是可以相互转换**，比如二进制 10011，从高到低的比特位是 43210，用 1 表示这个比特位出现在集合中，0 表示不在集合中，那么 10011 就表示集合 {0,1,4}
- `isLimit`：表示当前是否受到了 `n` 的约束。若为真，则第 `i` 位填入的数字至多为 `s[i]`，否则可以是 9。如果在受到约束的情况下填了 `s[i]`，那么后续填入的数字仍会受到 `n` 的约束
- `isNum`：表示 `i` 前面的数位是否填了数字。若为 `false`，则当前位可以跳过（不填数字），或者要填入的数字至少为 1；若为 `true`，则必须填数字，且要填入的数字可以从 0 开始。

> 后面两个参数可适用于其它数位 DP 题目，mask 参数根据题意取舍

C++ 代码只需要记忆化 `(i,mask)` 这个状态，因为：

1. 对于一个固定的 `(i,mask)`，这个状态受到 `isLimit` 或 `isNum` 的约束在整个递归过程中至多会出现一次，没必要记忆化。
2. 另外，如果只记忆化 `(i,mask)`，`dp` 数组的含义就变成在不受到约束时的合法方案数，所以要在 `!isLimit && isNum` 成立时才去记忆化。



**时间复杂度**：

- 对于 DP 来说，等于**状态个数** * **转移个数**
- O(len(s)) =  O(log n) * **1024** * **10**



### 1. 代码

```cpp
class Solution {
public:
    int countSpecialNumbers(int n) {
        string s = to_string(n);
        int m = s.length();
        // 注意这里使用 10位二进制数 表示 前面已经填的数字的集合
        int dp[m][1 << 10];
        memset(dp, -1, sizeof(dp));

        // 返回从第 i 位开始填数字，i 前面填的数字集合是 mask，能构造出的特殊整数的数目
        // isLimit 表示前面填的数字是否都是 n 对应位上的，如果为 true，那么当前位至多为 int(s[i])，否则可以至多为 9
        // isNum 表示前面是否已经填了数字（是否跳过），如果为 true，那么当前位可以从 0 开始，否则为 false，就表示可以跳过或者从 1 开始填数字
        // 注意这里的 mask 是前面已择数字的状态，是一个10位的二进制数，如:0000000010就代表前面已经选了1”，也就是 前面数字的集合 可以使用 10位二进制数(0-9) 表示
        function<int(int, int, bool, bool)> dfs = [&](int i, int mask, bool isLimit, bool isNum) -> int {
            // 填到了最后一位，直接返回
            if (i == m) {
                return isNum;
            }

            // 记忆化搜索前面是否已经计算结果
            // 当然只有在不受到 isLimit 或 !isNum 约束的前提下才可以记忆化
            if (!isLimit && isNum && dp[i][mask] >= 0){
                return dp[i][mask];
            }

            int res = 0;
            if (!isNum) {
                res = dfs(i + 1, mask, false, false); // 可以跳过当前数位
            }

            // 枚举目前可以填的数字范围
            // 当 isNum=False 时表示前面已经跳过了，可以选择跳过也可以从 1 开始
            // 当 isNum=True 时表示前面没有跳过，当前位可以从 0 开始
            for (int d = 1-isNum, up = isLimit ? s[i]-'0' : 9; d <= up; ++ d) {
                // d 不在 mask 中
                if ((mask >> d & 1) == 0) {
                    // mask|(1 << d) 表示将d填入mask中
                    res += dfs(i+1, mask|(1 << d), isLimit && (d == up), true);
                }
            }
            // 当 isLimit == true || isNum == False 时不会发生重复访问同一个状态的情况
            if (!isLimit && isNum) dp[i][mask] = res;

            return res;
        };

        // 初始第一位填的数字是有限制的，而且不能填前导0
        return dfs(0, 0, true, false);
    }
};
```



### 2. 例题

- [ ] [233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one/)：[题解](https://leetcode.cn/problems/number-of-digit-one/solution/by-endlesscheng-h9ua/)
- [ ] [面试题 17.06. 2出现的次数](https://leetcode.cn/problems/number-of-2s-in-range-lcci/)：[题解](https://leetcode.cn/problems/number-of-2s-in-range-lcci/solution/by-endlesscheng-x4mf/)
- [ ] [600. 不含连续1的非负整数](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/)：[题解](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/solution/by-endlesscheng-1egu/)
- [x] [902. 最大为 N 的数字组合](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)
- [ ] 1012.至少有 1 位重复的数字
- [ ] 1067.范围内的数字计数
- [ ] 1397.找到所有好字符串（有难度，需要结合一个知名字符串算法）



#### [902. 最大为 N 的数字组合](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)

给定一个按 非递减顺序 排列的数字数组 digits 。你可以用任意次数 `digits[i]` 来写的数字。例如，如果 `digits = ['1','3','5']`，我们可以写数字，如 '13', '551', 和 '1351315'。返回 可以生成的小于或等于给定整数 n 的正整数的个数 。

> 这里是直接在 digits 里面选择数字所以不需要 mask

```cpp
int atMostNGivenDigitSet(vector<string>& digits, int n) {
    string s = to_string(n);
    int m = s.length();
    int dp[m];
    memset(dp, -1, sizeof(dp));

    // 返回从第 i 位开始填数字，i 前面填的数字集合是 mask，能构造出的特殊整数的数目
    // isLimit 表示前面填的数字是否都是 n 对应位上的，如果为 true，那么当前位至多为 int(s[i])，否则至多为 9
    // isNum 表示前面是否都已经填了数字（是否跳过），如果为 true，那么当前位可以从 0 开始，否则为 false，就表示可以跳过或者从 1 开始填数字
    function<int(int, bool, bool)> dfs = [&](int i, bool isLimit, bool isNum) -> int {
        // 填到了最后一位，直接返回
        if (i == m) {
            return isNum;
        }

        // 记忆化搜索前面是否已经计算结果
        if (!isLimit && isNum && dp[i] >= 0){
            return dp[i];
        }

        int res = 0;
        // 前面不填数字，那么可以跳过当前数位，也不填数字
        // isLimit 改为 false，因为没有填数字，位数都比 n 要短，自然不会受到 n 的约束
        // isNum 仍然为 false，因为没有填任何数字
        if (!isNum) {
            res = dfs(i + 1, false, false);
        }

        char up = isLimit ? s[i] : '9';
        for (auto& d: digits) {
            // d 超过上限，由于 digits 是有序的，后面的 d 都会超过上限，故退出循环
            if (d[0] > up) break;   

            // isLimit：如果当前受到 n 的约束，且填的数字等于上限，那么后面仍然会受到 n 的约束
            // isNum 为 true，因为填了数字
            res += dfs(i + 1, isLimit && d[0] == up, true);
        }

        // 当 isLimit == true || isNum == false 时没必要记忆化因为不会重复计算
        if (!isLimit && isNum) dp[i] = res;
        
        return res;
    };

    // 初始第一位填的数字是有限制的，而且不能填前导0
    return dfs(0, true, false);
}
```



### 3. 参考

- [数位DP学习整理——数位DP看完这篇你就会了 CSDN](https://blog.csdn.net/hzf0701/article/details/116717851)
- [数位 DP 通用模板，附题单（Python/Java/C++/Go）-- 0x3F](https://leetcode.cn/problems/count-special-integers/solution/shu-wei-dp-mo-ban-by-endlesscheng-xtgx/)