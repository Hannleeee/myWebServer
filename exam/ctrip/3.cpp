#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 0, m = 0;
    cin >> n >> m;
    vector<pair<int, int>> rules(m);
    string str = "";
    cin >> str;
    for (int i=0; i<m; ++i) cin >> rules[i].first >> rules[i].second;

    vector<int> maxCont(n);
    int cnt = 0;
    for (int i=0; i<n; ++i) {
        if (str[i] == '1') ++cnt;
        else cnt = 0;
        maxCont[i] = cnt;
    }
    vector<int> dp(n+1);
    for (int i=1; i<=n; ++i) {
        dp[i] = dp[i-1];
        for (auto rule : rules) {
            if (maxCont[i-1] >= rule.first) {
                dp[i] = max(dp[i], dp[i-rule.first] + rule.second);
            }
        }
    }
    cout << dp[n];
}