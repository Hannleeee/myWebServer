#include <bits/stdc++.h>
using namespace std;

int main() {
    string word;
    unordered_set<string> dict;
    while (cin >> word) {
        dict.insert(word);
        if (getchar() == '\n') break;
    }
    string str;
    cin >> str;
    int n = str.size();
    vector<bool> dp(n + 1);
    dp[0] = true;
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            dp[i] = dp[i] || (dp[j] && dict.find(str.substr(j, i - j))!=dict.end());
        }
    }
    cout << (dp[n] ? "true" : "false");
}