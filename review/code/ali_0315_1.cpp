#include <iostream>
#include <vector>
#include <functional>
#include <cstring>
using namespace std;

int main()
{
    int n; cin >> n;
    vector<pair<int, int>> vt(n+1);
    for (int i = 1; i <= n; ++ i) {
        int a, b;
        cin >> a >> b;
        vt[i] = {a, b};
    }

    // 统计节点的入度，找到根节点
    int root = 1;
    int d[n+1]; memset(d, 0, sizeof d);
    for (int i = 1; i <= n; ++ i) {
        if (vt[i].first != -1) d[vt[i].first] += 1;
        if (vt[i].second != -1) d[vt[i].second] += 1;
    }
    for (int i = 1; i <= n; ++ i) {
        if (d[i] == 0) {
            root = i;
            break;
        }
    }

    vector<int> dp(n+1, 0);    // dp[i] 表示以 i 为根的子树是多少高度的满二叉树

    // dfs，遍历编号为u的节点
    // 如果是满二叉树，dp[i] = 其对应的深度，否则 dp[i] = 0
    function<void(int)> dfs = [&](int u) {
        int l = vt[u].first, r = vt[u].second;
        if (l == -1 && r == -1) {
            dp[u] = 1;
            return;
        }
        if (l != -1) dfs(l);
        if (r != -1) dfs(r);
        if (l != -1 && r != -1 && dp[l] == dp[r] && dp[l]) {
            dp[u] = dp[l] + 1;
        } else {
            dp[u] = 0;
        }
    };

    dfs(root);
    int ans = 0;
    for (int i = 1; i <= n; ++ i) {
        ans += (dp[i] != 0);
    }
    cout << ans << endl;
    return 0;
}