#include <bits/stdc++.h>
using namespace std;

pair<string, string> split(const string &x) {
    string a, b;
    for (char i : x) {
        if (i == 'm' || i == 'h' || i == 'y') {
            a.push_back(i);
        } else {
            b.push_back(i);
        }
    }
    return {a, b};
}

void solve() {
    string a, b;
    cin >> a >> b;
    auto [a1, a2] = split(a);
    auto [b1, b2] = split(b);
    if (a2 != b2) {
        cout << "No" << '\n';
        return;
    }
    vector<int> mp(255);
    mp['m'] = 100000;
    mp['h'] = 100000;
    mp['y'] = 100000;
    for (char i : a1)
        mp[i]++;
    for (char i : b1) {
        mp[i]--;
    }
    int m1 = mp['m'];
    int m2 = mp['h'];
    int m3 = mp['y'];
    if (m1 == m2 && m2 == m3) {
        cout << "Yes" << '\n';
    } else {
        cout << "No" << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        solve();
    }
    return 0;
}
