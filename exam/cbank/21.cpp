#include <bits/stdc++.h>
using namespace std;

int solve(int k, int m) {
    vector<vector<int>> dp(k, vector<int>(m));
    for (int j=0; j<m; ++j) {
        for (int t=1; t<=9; ++t) {
            if (t==j+1) dp[0][j] = 1;
        }
    }
    for (int i=1; i<k; ++i) {
        for (int j=0; j<m; ++j) {
            for (int t=0; t<=9; ++t) {
                if (j>=t) dp[i][j] += dp[i-1][j-t];
            }
        }
    }
    return dp[k-1][m-1];
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