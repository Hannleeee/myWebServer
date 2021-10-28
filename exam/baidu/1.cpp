#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> d(n);
    for (int i = 0; i < n; ++i) {
        cin >> d[i].first;
        d[i].second = i;
    }
    sort(d.begin(), d.end());
    int cnt = 0;
    for (int i = n - 1; i >= 0; --i) {
        int m = d[i].first;
        for (int j = i - 1; j >= i - m; --j) {
            cout << d[i].second+1 << " " << d[j].second+1 << "\n";
            ++cnt;
            if (cnt == n-1) break;
        }
        if (cnt == n-1) break;
    }
}