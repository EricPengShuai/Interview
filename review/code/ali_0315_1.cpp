#include <iostream>
#include <vector>
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

    vector<int> ans;

    // dfs，遍历编号为u的节点
    // 如果是满二叉树，返回其对应的深度，否则返回0
    function<int(int)> dfs = [&](int u) {
        // 叶子结点一定是满足满二叉树的
        if (vt[u].first == -1 && vt[u].second == -1) {
            ans.push_back(u);
            return 1;
        }

        // 只有一个叶子为空不满足
        if (vt[u].first == -1 || vt[u].second == -1) {
            return 0;
        }

        // 左右节点不为空递归判断
        int left = dfs(vt[u].first);
        int right = dfs(vt[u].second);
        if (left && right && left == right) {
            ans.push_back(u);
            return left + 1;
        }
        return 0;
    };

    dfs(1);
    cout << ans.size() << endl;
    return 0;
}