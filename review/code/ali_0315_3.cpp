#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int n, k; 
    cin >> n >> k;
    vector<int> vt(n);
    for (int i = 0; i < n; ++ i) {
        cin >> vt[i];
    }

    sort(vt.begin(), vt.end());

    int left = 0, right = n - k - 1;
    int lmin = 0, lmax = 0; // 左边块的 min 和 max
    int rmin = 0, rmax = 0; // 右边块的 min 和 max

    int res = INT_MAX;
    while (right < n) {
        // 中间块的 min 和 max
        int dmin = vt[left], dmax = vt[right];

        if (left != 0) {
            lmin = vt[0] * 2;
            lmax = vt[left-1] * 2;
        } else {
            lmin = INT_MAX;
            lmax = 0;
        }

        if (right != n-1) {
            rmin = vt[right+1] / 2;
            rmax = vt[n-1] / 2;
        } else {
            rmin = INT_MAX;
            rmax = 0;
        }

        // 更新极差
        res = min(res, max({lmax, dmax, rmax}) - min({rmin, dmin, rmin}));
        ++ left;
        ++ right;
    }
    cout << res << endl;
    return 0;
}