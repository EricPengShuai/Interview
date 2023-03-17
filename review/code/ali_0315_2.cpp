#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int n; cin >> n;
    vector<int> vt(n);
    for (int i = 0; i < n; ++ i) {
        cin >> vt[i];
    }

    sort(vt.begin(), vt.end());
    int ans = 0;
    int i = 0;
    while (i < n) {
        int ret = upper_bound(vt.begin(), vt.end(), vt[i]) - vt.begin();
        
        if (ret == n) { // 后面没有数字
            break;
        }

        if (vt[ret] - vt[i] > 1) {
            i = ret;
        } else {
            int j = ret;
            while (j+1 < n && vt[j+1] == vt[ret]) ++ j;
            
            // 2*min + max
            int res1 = (ret - i) * (ret - i - 1) / 2 * (j - ret + 1); 
            
            // min + 2*max
            int res2 = (ret - i) * (j - ret + 1) * (j - ret) / 2;

            ans += res1 + res2;
            cout << res1 << ' ' << res2 << endl;

            i = ret;
        }
    }
    cout << ans << endl;
    return 0;
}