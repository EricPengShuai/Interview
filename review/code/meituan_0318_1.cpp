#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, a, b;
    cin >> n >> a >> b;
    vector<pair<int, int>> vt(n);
    for (int i = 0; i < n; ++ i) {
        cin >> vt[i].first >> vt[i].second;
    }
    
    int ans = 0;
    for (int i = 0; i < n; ++ i) {
        // 枚举第 i 个点
        int x = vt[i].first, y = vt[i].second;
        int min_x = x, max_x = x, min_y = y, max_y = y;

        int ret = 1;
        for (int j = 0; j < n; ++ j) {
            if (j == i) continue;
            int m = vt[j].first, n = vt[j].second;
            
            if (abs(m - min_x) <= a && abs(m - max_x) <= a &&
                abs(n - min_y) <= y && abs(m - max_y) <= a) {
                // 加入之后维护 min_x_y
                min_x = min(min_x, m), max_x = max(max_x, m); 
                min_y = min(min_y, n), max_y = max(max_y, n);
                ++ ret;
            }
        }
        if (ret > ans) ans = ret;
    }
    cout << ans << endl;
    return 0;
}