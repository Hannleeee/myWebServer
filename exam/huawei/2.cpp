#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 0;
    cin >> N;
    vector<int> nums(N);
    for (int i=0; i<N; ++i) cin >> nums[i];

    vector<int> dp(N);
    dp[0] = nums[0];
    int maxi = INT_MIN;
    for (int i=1; i<N; ++i) {
        dp[i] = max(dp[i-1]+nums[i], nums[i]);
        maxi = max(maxi, dp[i]);
    }
    vector<int> idxs;
    for (int i=0; i<N; ++i) {
        if (dp[i]==maxi) {
            idxs.push_back(i);
        }
    }
    int leng = INT_MAX;
    int rRet = 0;
    for (int r : idxs) {
        int sum = nums[r];
        int l = r;
        while (sum!=maxi) {
            --l;
            sum += nums[l];
        }
        if (r-l<leng) {
            leng = r-l;
            rRet = r;
        }
    }
    cout << rRet-leng << " " << rRet << " " << maxi << "\n";
}