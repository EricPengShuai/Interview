#include <iostream>
#include <vector>
using namespace std;

void dfs(vector<vector<int>> &vt, int x, int y, int cur, int mark) {
    if (x < 0 || x >= vt.size() || y < 0 || y >= vt[0].size()) {
        return;
    }

    // 注意一次 DFS 只是标记 cur 节点以及其相邻节点，所以遇到不是 cur 节点就直接 return
    if (vt[x][y] != cur) {
        return;
    }

    vt[x][y] = mark;
    dfs(vt, x, y+1, cur, mark);
    dfs(vt, x, y-1, cur, mark);
    dfs(vt, x-1, y, cur, mark);
    dfs(vt, x+1, y, cur, mark);
};

int main()
{
    int m, n;
    cin >> m >> n;
    vector<vector<int>> vt(m, vector<int>(n));
    vector<vector<int>> gt(m, vector<int>(n));

    for (int i = 0; i < m; ++ i) {
        for (int j = 0; j < n; ++ j) {
            char c;
            cin >> c;
            if (c == 'G') {
                vt[i][j] = 1;
                gt[i][j] = 1;
            } else if (c == 'B') {
                vt[i][j] = 2;
                gt[i][j] = 1;
            }
        }
    }

    int mark = -1;
    int ansA = 0, ansB = 0;

    for (int i = 0; i < m; ++ i) {
        for (int j = 0; j < n; ++ j) {
            if (vt[i][j] != mark) {
                dfs(vt, i, j, vt[i][j], mark);
                ++ ansA;
            }
        }
    }

    for (int i = 0; i < m; ++ i) {
        for (int j = 0; j < n; ++ j) {
            if (gt[i][j] != mark) {
                dfs(gt, i, j, gt[i][j], mark);
                ++ ansB;
            }
        }
    }

    cout << ansA - ansB << endl;
    return 0;
}