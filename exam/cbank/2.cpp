#include <bits/stdc++.h>
using namespace std;

void dfs(int idx, int cur, int k, int m, int &cnt) {
    if (cur==m) ++cnt;
    else if (cur<m) {
        if (idx==0) {
            for (int i=1; i<=9; ++i) {
                dfs(idx+1, cur+i, k, m, cnt);
            }
        }
        else if (idx<k) {
            for (int i=0; i<=9; ++i) {
                dfs(idx+1, cur+i, k, m, cnt);
            }
        }
    }
}

int solve(int k, int m) {
    int cnt = 0;
    dfs(0, 0, k, m, cnt);
    return cnt;
}

int main() {
    int t;
    cin >> t;
    for (int i=0; i<t; ++i) {
        int k,m;
        cin >> k >> m;
        cout << solve(k, m) << "\n";
    }
}