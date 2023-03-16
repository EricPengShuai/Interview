#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// 1 --> r
// 3 --> rr
// 6 --> rrr
// 10 --> rrrr
// n(n+1) / 2 --> n = ans.size()

string ans = "";
vector<char> vt = {'r', 'e', 'd'};

void help(int n, int id) {
    if (n == 0) return;

    // 找到最大的 x : (x+1)x < 2n
    int x = sqrt(2*n);
    if (x * (x+1) > 2*n) {
        x = x - 1;
    }
    // cout << x << ' ';

    // 贪心构造
    ans += string(x, vt[id % 3]);
    id += 1;

    // 递归构造 ans
    help(n - (x+1) * x / 2, id);
}

int main()
{
    int n; cin >> n;
    help(n, 0);
    cout << ans << endl;
    return 0;
}
