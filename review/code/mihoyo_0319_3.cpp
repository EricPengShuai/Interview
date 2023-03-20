#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int maxn = 1e6 + 555;
const int MOD = 1e9 + 7;
int A[maxn];
ll cnt[maxn];
ll DP[maxn];

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
        cnt[A[i]]++;
        DP[A[i]]++;
    }
    for (int i = 1; i <= 1000000; i++) {
        for (int j = 2; i * j <= 1000000; j++) {
            if (cnt[i * j]) {
                (DP[i * j] += DP[i]) %= MOD;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        DP[A[i]]--;
    }
    ll ans = 0;
    for (int i = 1; i <= 1000000; i++) {
        (ans += DP[i]) %= MOD;
    }
    ans = (ans + MOD) % MOD;
    cout << ans << endl;
    return 0;
}