#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size();
        vector<vector<ll>> diff(n, vector<ll>(n));
        vector<vector<int>> dp(n, vector<int>(n, 1));
        unordered_map<string, vector<int>> memo;
        int cnt = 0;
        for (int row=n-1; row>=0; --row) {
            for (int col=row+1; col<n; ++col) {
                int newRow = col;
                for (int newCol=newRow+1; newCol<n; ++newCol) {
                    if ((ll)nums[newCol]+nums[row]==2*(ll)nums[col]) 
                        dp[row][col] += dp[newRow][newCol];
                }
                cnt += dp[row][col] - 1;
            }
        }
        return cnt;
    }
};

int main () {
    vector<int> vec = {1,2,3,4,5,6,7};
    Solution s;
    cout << s.numberOfArithmeticSlices(vec);
}