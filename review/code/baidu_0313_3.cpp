#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n; cin >> n;
    string s; cin >> s;
    vector<vector<int>> gt(n+1);  // 邻接矩阵
    for (int i = 0; i < n-1; ++ i) {
        int x, y;
        cin >> x >> y;
        gt[x].push_back(y);
        gt[y].push_back(x);
    }

    // dp[i] 表示以 i 点为根的子树中同色连通块的个数
    int dp[n+1]; memset(dp, 0, sizeof n);

    function<void(int, int)> dfs = [&](int u, int fa) {
        dp[u] = 1;  // 初始节点 u 的连通分量为 1
        for (int v: gt[u]) {
            if (v == fa) continue;
            dfs(v, u);
            dp[u] += dp[v]; // 加上所有子节点的同色连通块数

            // 如何同色，当前节点和子节点连通块是相同的
            if (s[u-1] == s[v-1]) dp[u] -= 1;
        }
    };

    dfs(1, -1); // 第一遍dfs求出dp数组

    long long ans = 0;
    function<void(int, int)> dfs1 = [&](int u, int fa) {
        for (int v: gt[u]) {
            if (v == fa) continue;
            dfs1(v, u);

            // dp[v补集]
            int x = dp[1]-dp[v];
            if (s[u-1] == s[v-1]) x += 1;   // 同色断开之后，dp[v补集] 应加一
            ans += abs(x - dp[v]);
        }
    };

    // 第二遍dfs计算每条边的贡献(也可以直接枚举邻接矩阵 gt)
    dfs1(1, -1); 
    
    cout << ans << '\n';
    return 0;
}