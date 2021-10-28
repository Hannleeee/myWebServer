#include <bits/stdc++.h>
using namespace std;

vector<int> coins;

int solve() {
    int n;
    cin >> n;
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= n; ++i) {
        for (int coin : coins) {
            if (i >= coin) dp[i] = min(dp[i], dp[i - coin]);
        }
    }
    return dp[n];
}

int main() {
    int t;
    cin >> t;
    for (int )
    for (int i = 0; i < t; ++i) {
        cout << solve() << "\n";
    }
}