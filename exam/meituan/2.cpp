#include <bits/stdc++.h>
using namespace std;

bool isValid(vector<int> &nums) {
    for (int i = 0; i < nums.size(); ++i) {
        if (nums[i] == i + 1) return false;
    }
    return true;
}

int main() {
    int n = 0;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) nums[i] = i + 1;
    vector<vector<int>> ret;
    vector<int> dp(n + 1);
    dp[1] = 0;
    for (int i = 2; i <= n; ++i) {
        dp[i] = i * dp[i - 1] + (i&1 ? -1 : 1);
    }
    cout << dp[n] << "\n";
    int cnt = 0;
    while (next_permutation(nums.begin(), nums.end()) && cnt < 100) {
        if (isValid(nums)) {
            ++cnt;
            for (int &num : nums) cout << num << " ";
            cout << "\n";
        }
    }
}