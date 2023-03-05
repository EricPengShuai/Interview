## 一些小技巧

> 记录一些题目中的小算法，例如判断素数、质因数分解等



### 1. 判断素数

素数也叫质数，大于 1 的整数中，只能被 1 和其自身整除的数称为素数；1 既不是素组也不是合数

```cpp
bool isPrime(int x) {
    if (x == 1) return false;
    for (int i = 2; i * i <= x; ++ i) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}
```



### 2. 质因数分解

质因数分解就是将一个数分解成各个质数因数，例如 12 = 2 * 2 * 3 ，具体思路为

1. p 从 2 遍历到 x，先找到 x 的最小质因子 p
2. 当 x == p 时，说明已经找完所有的质因子
3. 当 x > p 时，x /= p，继续从 (1) 开始遍历

```cpp
// x 是待分解的数，分解出来的质因数存储在 primes 中
// 时间复杂度 O(n)
void getPrimeFactor1(long x, vector<long>& primes) {
    long p = 2;
    while (x != 1) {
        // p 一定是第一个可以整除 x 的最小质因数
        // 这里 p 一定是质数，没必要再次判断
        for (p = 2; p <= x; ++ p) {
            if (x % p == 0) {
                break;
            }
        }
        primes.push_back(p);
        x /= p;
    }
}

// 时间复杂度 O(n)
void getPrimeFactor2(long x, vector<long>& primes) {
    long p = 2;
    for (p = 2; p <= x; ++ p) {
        // 找到 x 的一个质因子 p 之后就不断的除以 p，将 x 中所有的 p 全部找出
        while (x % p == 0) {
            primes.push_back(p);
            if (x == p)  break;
            x /= p;
        }
    }
}

// 时间复杂度 O(n^1/2)
int getPrimeFactor3(long x, vector<long>& primes) {
    for (int p = 2; p*p <= x; ++ p) {
        while (x % p == 0) {
            primes.push_back(x);
            x /= p;
        }
    }
    if (x > 1) primes.push_back(x);
}
```

**以上的方法得到的质因数中会有重复的，如果需要得到不重复的，需要改一下代码：**

```cpp
void getPrimeFactor(long x, vector<long>& primes) {
    long p = 2;
    // 假设，原数已经找到了质因子（从小到大排列，不包含重复值）p1,p2...,pt
    // 此时，x = x/(p1*p2*...*pt);
    // 在 [pt+1, x] 的范围上，找 x 的最小质因数
    while (p <= x) {
        if (x % p == 0 && isPrime(p)) {
            primes.push_back(p);
            if (x == p) {
                return;
            }
            x /= p;
        }
        p += 1;
    }
}
```

**例题：**

[2507. 使用质因数之和替换后可以取到的最小值](https://leetcode.cn/problems/smallest-value-after-replacing-with-sum-of-prime-factors/)

- 参考灵神的题解，for 循环可以直接使用 `i * i < n` 判断，但是要注意推出之后和 1 判断一下

- 没必要判断是否变小，因为 $2+\frac{n}{2} \le n,(n \ge 4)$ 是一直变小的，退出条件就是 ans == n，[0x3F 暴力](https://leetcode.cn/problems/smallest-value-after-replacing-with-sum-of-prime-factors/solution/bao-li-by-endlesscheng-xh0b/)


**参考：**[C++实现 质因数分解、最大公约数、最小公倍数、求所有因数](https://blog.csdn.net/mumu1408/article/details/108557031)



### 3. 最大公约数和最小公倍数

**最大公约数（gcd）**：greatest common divisor，C++ `<numeric>`中有具体的库函数 `std::gcd`，如果自己实现可以这样写：

```cpp
int gcd(int a,int b) {
    int r;
    while(b > 0) {
        r  = a % b;
        a = b;
        b = r;
    }
    return a;
}

// 递归写法，出口条件就是 b 是否为 0
int gcd(int a,int b) {
    return b>0 ? gcd(b, a%b) : a;
}
```

**最小公倍数（lcm）**：lowest common multiple，C++ `<numeric>`中有具体的库函数 `std::lcm`，如果自己实现可以这样写：

```cpp
// lcm(a, b) = a * b / gcd(a, b)
int lcm(int a,int b){
	return a/gcd(a,b)*b;	// 注意先除可以防溢出
}
```

**参考：**[gcd函数和lcm函数（c/c++）小染大大的博客](https://blog.csdn.net/m0_50176535/article/details/115129689)



### 4. Dijkstra

迪杰斯塔拉算法用来求**单源最短路**，也就是某个节点到其他所有节点的最短路，这里可以使用**邻接矩阵或者邻接表**来存图。这里以 **[743. 网络延迟时间](https://leetcode.cn/problems/network-delay-time/)** 为例展示 Dijkstra 的用法：

> 题目给出了图的边权数组 times，图中节点数 n，指定节点 k，求节点 k 到其他节点的最大值，也就是利用 Dijkstra 求出 k 到其他节点的距离然后再取最大值

#### 邻接矩阵

```cpp
// 邻接矩阵数组，w[a][b] = c 表示 a 到 b 有权重为 c 的边
vector<vector<int>> w(n, vector<int>(n, 0));
```

源码：

```cpp
class Solution {
    int MAX_DIST = 6010;
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 邻接矩阵，注意这里初始节点从 1 开始
        vector<vector<int>> w(n+1, vector<int>(n+1, 0));	
        vector<int> dist(n+1, MAX_DIST);	// dist[x] = y 表示源点到x的最短距离为 y
        vector<int> pre(n+1, 0);	// 标记最短路径的上一个节点
        
        // 初始化邻接矩阵
        for (int i = 1; i <= n; ++ i) {
            for (int j = 1; j <= n; ++ j) {
                if (i != j) w[i][j] = w[j][i] = MAX_DIST;
            }
        }
        
        // 建图，这里是有向图
        for(auto& time: times) {
            int a = time[0], b = time[1], c = time[2];
            w[a][b] = c;
        }
        
        // 求源点 k 到其他节点的最短路
        dijkstra(w, dist, pre, k, n);
		
        int ans = 0;
        for (int i = 1; i <= n; ++ i) {
            ans = max(ans, dist[i]);
        }
        // getPath(pre, k, 4);
        return ans == MAX_DIST ? -1 : ans;
    }
    
    // 邻接矩阵形式的迪杰斯塔拉算法，传入5个参数：
    // 邻接矩阵 w，到源点的最短距离数组 dist，前一个节点数组 pre，源点/起点 k，节点数 n
    void dijkstra(vector<vector<int>>& w, vector<int>& dist, vector<int>& pre, int k, int n) {
        vector<bool> vis(n+1, false);	// 标记哪些节点已经更新过了
        
        // 初始化 dist 和 pre，也可以在外面初始化 
        for (int i = 1; i <= n; ++ i) {
            dist[i] = w[k][i];
            if (dist[i] < MAX_DIST) {	// MAX_DIST 是预设的最大距离
                pre[i] = k;
            } else {
                pre[i] = 0;
            }
        }
        dist[k] = 0;
        pre[k] = k;

        // 迭代 n 次
        for (int p = 1; p <= n; ++ p) {
            // 每次找到最短距离最小且未被更新的点 u
            int u = -1;
            for (int i = 1; i <= n; ++ i) {
                if (!vis[i] && (u == -1 || dist[i] < dist[u])) u = i;
            }
            vis[u] = true;
            // 用点 t 的最小距离更新其他点
            // k-->v <==> k-->u-->v
            // 遍历 u 相邻的节点 v
            for (auto& [v, c]: edges[u]) {
                if (!vis[v] && dist[v] > dist[u] + c) {
                    dist[v] = dist[u] + c;
                    pre[v] = u;
                }
            }
        }
    }
};
```

- 时间复杂度：两个 for 循环，$O(n^2)$
- 空间复杂度：邻接矩阵大小，$O(n^2)$

**如果要返回路径，可以遍历 pre 数组然后逆序输出**

```cpp
// 获取起点 start 到终点 end 的路径
// pre 是保存最短路径的上一个节点信息的数组
vector<int> getPath(vector<int>& pre, int start, int end) {
    vector<int> path;
    
    // 逆序获取路径
    int tmp = end;
    while (tmp != start) {
        path.push_back(tmp);
        tmp = pre[end];
        end = tmp;
    }
    path.push_back(start);
    
    reverse(path.begin(), path.end());
    return path;
}
```



#### 邻接表

当边比较少时，使用邻接矩阵会存在浪费节点信息的情况，这时可以考虑邻接表。邻接表适用于边数较少的 **「稀疏图」，当边数量接近点的数量，即 m≈n 时，可定义为「稀疏图」**

```cpp
// 相当于数组存储链表
// edges[u] = {{v1, c1}, {v2, c2}, {v3, c3}}
// 表示节点 u 和节点 v1, v2, v3 相邻，权重分别为 c1, c2, c3
vector<vector<pair<int, int>> edges;
```

源码：

```cpp
class Solution {
    int MAX_DIST = 6010;
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 邻接矩阵，注意这里初始节点从 1 开始
        vector<vector<pair<int, int>>> edges(n+1);	
        vector<int> dist(n+1, MAX_DIST);	// dist[x] = y 表示源点到x的最短距离为 y
        vector<int> pre(n+1, 0);			// 标记最短路径的上一个节点
        
        // 建图，这里是有向图
        for(auto& time: times) {
            int a = time[0], b = time[1], c = time[2];
            edges[a].push_back({b, c});
        }
        
        // 求源点 k 到其他节点的最短路
        dijkstra(w, dist, pre, k, n);
		
        int ans = 0;
        for (int i = 1; i <= n; ++ i) {
            ans = max(ans, dist[i]);
        }
        // getPath(pre, k, 4);
        return ans == MAX_DIST ? -1 : ans;
    }
    
    // 邻接表形式的迪杰斯塔拉算法，传入5个参数：
    // 邻接表 edges，到源点的最短距离数组 dist，前一个节点数组 pre，源点/起点 k，节点数 n
    void dijkstra(vector<vector<pair<int, int>>>& edges, vector<int>& dist, vector<int>& pre, int k, int n) {
        vector<bool> vis(n+1, false);	// 标记哪些节点已经更新过了
        
        // 遍历节点 k 所有相邻的节点 y
        for (auto& [y, c]: edges[k]) {
            dist[y] = c;
            pre[y] = k;
        }
        dist[k] = 0;
        pre[k] = k;

        // 迭代 n 次
        for (int p = 1; p <= n; ++ p) {
            // 每次找到最短距离最小且未被更新的点 u
            int u = -1;
            for (int i = 1; i <= n; ++ i) {
                if (!vis[i] && (u == -1 || dist[i] < dist[u])) u = i;
            }
            vis[u] = true;
            // 用点 u 的最小距离更新其他点: k-->v <==> k-->u-->v
            // 遍历 u 相邻的节点 v
            for (auto& [v, c]: edges[u]) {
                if (!vis[v] && dist[v] > dist[u] + c) {
                    dist[v] = dist[u] + c;
                    pre[v] = u;
                }
            }
        }
    }
};
```

- 时间复杂度：$O(n^2+m)$，n 为节点数，m 为边数
- 空间复杂度：$O(mn)$ 

寻找最小 `d[u]` 的过程却可以不必达到 `O(n)`的复杂度，而可以使用对优化来降低复杂度。最简单的写法是直接使用 STL 中的优先队列`priority_queue`，这样使用邻接表实现 Dijkstra 算法的时间复杂度可以降低为 $O(n\log n+m)$

```cpp
// 返回从 start 到每个点的最短路，3个参数：
// 邻接表 g, 最短路径中上一个节点的信息数组 pre, 起点 start
vector<int> dijkstra(vector<vector<pair<int, int>>>& g, vector<int>& pre, int start) {
    vector<int> dist(g.size(), MAX_DIST);
    dist[start] = 0;

    // 小顶推，在O(logn)时间内找到最小的 d[u]
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, start);

    while(!pq.empty()) {
        // 找到最短距离最小点 u
        auto[d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) {
            continue;
        }
        // 枚举节点 u 所有相邻的节点 v
        for (auto& [v, wt]: g[u]) {
            if (dist[v] > dist[u] + wt) {
                dist[v] = dist[u] + wt;
                pq.emplace(dist[v], v);
                pre[v] = u;
            }
        }
    }
    return dist;
}
```



#### 参考

- [【最短路/必背模板】743. 网络延迟时间 : 涵盖所有的「存图方式」与「最短路算法（详尽注释）](https://zhuanlan.zhihu.com/p/396576813)
- [【图解】转换成单源最短路问题（Python/Java/C++/Go）](https://leetcode.cn/problems/reachable-nodes-in-subdivided-graph/solution/tu-jie-zhuan-huan-cheng-dan-yuan-zui-dua-6l8o/)
- [C++ 求最短路径问题之Dijkstra算法（一）_YF_Li123的博客-CSDN博客](https://blog.csdn.net/YF_Li123/article/details/74090301)



**相似题目**

- [x] [882. 细分图中的可到达节点](https://leetcode.cn/problems/reachable-nodes-in-subdivided-graph/)：[代码](https://leetcode.cn/submissions/detail/384990243/)
- [x] [1976. 到达目的地的方案数](https://leetcode.cn/problems/number-of-ways-to-arrive-at-destination/)：[代码](https://leetcode.cn/submissions/detail/395127948/)
- [ ] [2203. 得到要求路径的最小带权子图](https://leetcode.cn/problems/minimum-weighted-subgraph-with-the-required-paths/)



### 5. Floyd

佛洛依德算法用来求解**多源最短路径**，即每个节点到其余节点的最短路径，相当于调用 n 次 Dijkstra 算法，这里使用邻接矩阵来写 Floyd 算法，还是以 LC743 为例

```cpp
class Solution {
    int MAX_DIST = 6010;
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 邻接矩阵，注意这里初始节点从 1 开始
        vector<vector<int>> w(n+1, vector<int>(n+1, 0));
        // 初始化邻接矩阵
        for (int i = 1; i <= n; ++ i) {
            for (int j = 1; j <= n; ++ j) {
                if (i != j) w[i][j] = w[j][i] = MAX_DIST;
            }
        }
        
        // 建图，这里是有向图
        for(auto& time: times) {
            int a = time[0], b = time[1], c = time[2];
            w[a][b] = c;
        }
        
        // 所有节点的最短路
        floyd(w, n);
		
        int ans = 0;
        for (int i = 1; i <= n; ++ i) {
            ans = max(ans, w[k][i]);
        }
        return ans == MAX_DIST ? -1 : ans;
    }
    
    // 邻接矩阵形式的 Floyd 算法，传入2个参数：邻接矩阵 w 和节点数 n
    void floyd(vector<vector<int>>& w, int n) {
        // floyd 基本流程为三层循环：
        // 枚举中转点 - 枚举起点 - 枚举终点 - 松弛操作
        for (int p = 1; p <= n; ++ p) {
            for (int i = 1; i <= n; ++ i) {
                for (int j = 1; j <= n; ++ j) {
                    if (w[i][j] > w[i][p] + w[p][j]) {
                        w[i][j] = w[i][p] + w[p][j];
                    }
                }
            }
        }
    }
};
```



### 6. 位运算

**n&(n-1)** 

> 消除数字 n 的二进制表示中的最后一个 1

```cpp
  n = ... 1 000
n-1 = ... 0 111
  & = ... 0 000 

// 判断一个数是不是 2 的指数 (1 0000)
// 1 0000 & (0 1111) = 0 0000
(n & (n-1)) == 0
```

**n&(-n)**

> 负数用补码（反码+1）表示，所以 n&(-n) 相当于保存右侧第一个1及其右侧的数，例如:

```cpp
 n = 10 1000
-n = 01 1000
 & = 00 1000   
```

**a ^= b; b ^= a; a ^ b**

> 交换 a 和 b，其实位运算是效率非常低的转换方式，[参考](https://blog.csdn.net/Solstice/article/details/5166912)

```cpp
a = a ^ b;
b = b ^ a = b ^ a ^ b = a;	// b = a;
a = a ^ b = a ^ b ^ a = b;	// a = b;
```



### 7. stringsteam

stringstream 通常用来格式化字符串，可以实现 cin 和 cout 的功能， 同时支持C风格字符串的输入输出操作

- ostringstream ： 用于执行C风格字符串的输出操作，只支持 << 操作符
- istringstream ： 用于执行C风格字符串的输入操作，只支持 >> 操作符

```cpp
void ostringstream_test()
{
    ostringstream oss;
    oss << "this is test" << 123456;
 
    oss.str("");//清空之前的内容
    //oss.clear();//并不能清空内存
 
    // 浮点数转换限制
    double tmp = 123.1234567890123;
    oss.precision(12);
    oss.setf(ios::fixed);// 将浮点数的位数限定为小数点之后的位数
    oss << tmp << "\r\n" << endl;
}

void istringstream_test()
{
    string str = "welcome to china";
    istringstream iss(str);
 
    // 把字符串中以空格隔开的内容提取出来
    string out;
    while(iss >> out) {
        cout << out << endl;
    }
    cout << endl;
}

// 实现 str.split(' ') 的操作
vector<string> stringstream_test(string &s, char delim)
{
    vector<string> res;
    string str;
    stringstream ss(s);
    
    while(getline(ss, str, delim)) {
        res.push_back(str);
    }
    return res;
}
```

