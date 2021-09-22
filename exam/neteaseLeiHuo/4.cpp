#include <bits/stdc++.h>
using namespace std;

bool check(int time, int n, int m, vector<int> &a, vector<int> &b) {
    vector<vector<int>> f(n, vector<int>(m+1));
    int last = 0;
    for (int i=0; i<n; ++i) {
        if (i>0) {
            for (int j=0; j<=min(m, time/a[i]); ++j) {
                for (int k=0; k<=last; ++k) {
                    if (j+k>m) break;
                    f[i][j+k] = max(f[i][j+k], f[i-1][k]+(time-a[i]*j)/b[i]);
                }
            }
            last = min(m, last+time/a[i]);
        }
        else {
            last = min(m, time/a[i]);
            for (int j=0; j<=last; ++j) {
                f[i][j] = (time-a[i]*j)/b[i];
            }
        }
    }
    return f[n-1][m]>=m;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(n);
    for (int i=0; i<n; ++i) {
        cin >> a[i] >> b[i];
    }
    int left = 0, right = 40000;
    while (left<right) {
        int time = left + (right-left>>1);
        if (check(time, n, m, a, b)) {
            right = time;
        }
        else left = time+1;
    }
    cout << right;
}