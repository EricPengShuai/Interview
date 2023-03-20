#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int INF = 0x3F3F3F3F;
const int maxn = 2e5 + 555;
const int MOD = 1e9 + 7;
int n, m;

// 并查集模板
int fa[1000 * 1000 + 5];
int seek(int x) { return fa[x] == x ? x : fa[x] = seek(fa[x]); }
void merge(int a, int b) {
    int f1 = seek(a);
    int f2 = seek(b);
    fa[f1] = f2;
}

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};
int solve(const vector<vector<char>> &V) {
    for (int i = 0; i < n * m; i++) {
        fa[i] = i;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < 4; k++) {
                int x = i + dx[k];
                int y = j + dy[k];
                // 并查集只有在当前节点和相邻节点颜色相同时才做 merge 操作
                if (x >= 0 && x < n && y >= 0 && y < m) {
                    if (V[i][j] == V[x][y]) {
                        merge(i * m + j, x * m + y);
                    }
                }
            }
        }
    }

    // 统计有多少个不同的 fa 就表示有多少颜色不同的连通块
    vector<int> F;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            F.push_back(seek(i * m + j));
        }
    }
    sort(F.begin(), F.end());

    // unique 参考：https://blog.csdn.net/qq_29344757/article/details/73744556
    F.erase(unique(F.begin(), F.end()), F.end());
    return F.size();
}

int main() {
    ios::sync_with_stdio(false);    // 关闭 C++ 兼容 C 的 stdio
    cin.tie(0); // 接触 cin 与 cout 的绑定，不用每次调用都 flush
    cout.tie(0);
    cout.precision(10); // 精度控制，保留 10 个有效位，不是小数点后10位
    cout << fixed;  // 浮点输出应该以固定点或小数点表示法显示，和 cout.precision(i) 一起使用就表示保留小数点后 i 位
    cin >> n >> m;
    vector<vector<char>> V(n, vector<char>(m));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> V[i][j];
    int before = solve(V);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (V[i][j] == 'B')
                V[i][j] = 'G';
    int after = solve(V);

    cout << before - after << endl;
    return 0;
}
