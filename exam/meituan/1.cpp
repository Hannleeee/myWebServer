#include <bits/stdc++.h>
using namespace std;

#define MOD 1000000007
typedef long long ll;

// int main() {
//     int n = 0;
//     cin >> n;
//     vector<pair<int, int>> a(n);
//     vector<ll> ret(n);
//     ret[0] = 1;
//     for (int i = 1; i < n; ++i) cin >> a[i].second;
//     for (int i = 1; i < n; ++i) cin >> a[i].first;
//     sort(a.begin(), a.end());
//     cout << 1;
//     for (int i = 1; i < n; ++i) {
//         ret[i] = a[i].second * ret[a[i].first - 1];
//         cout << " " << (ret[i]%MOD);
//     }
// }

ll get(int i, vector<int> &a, vector<int> &b) {
    if (i == 0) return 1;
    return a[i] * get(b[i] - 1 , a, b);
}

int main() {
    int n = 0;
    cin >> n;
    vector<int> a(n), b(n);
    vector<ll> ret(n);
    ret[0] = 1;
    for (int i = 1; i < n; ++i) cin >> a[i];
    for (int i = 1; i < n; ++i) cin >> b[i];
    cout << 1;
    for (int i = 1; i < n; ++i) {
        cout << " " << get(i, a, b)%MOD;
    }
}