#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minDistance(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        vector<vector<int>> dp(n+1, vector<int>(m+1, INT_MAX/2));
        dp[0][0] = 0;
        for (int i=0; i<=n; ++i) {
            for (int j=0; j<=m; ++j) {
                if (i==0 || j==0) dp[i][j] = max(i, j);
                else dp[i][j] = min(dp[i-1][j-1]+2*(word1[i-1]==word2[j-1]), min(dp[i][j-1]+1, dp[i-1][j]+1));
            }
        }
        return dp[n][m];
    }
};

int main() {
    Solution s;
    string a = "sea", b = "eat";
    cout << s.minDistance(a, b) << endl;
}