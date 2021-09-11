#include <bits/stdc++.h>
using namespace std;

int requiredK(vector<int> &nums, int target) {
    int ret = 0;
    int mini = INT_MAX, maxi = INT_MIN;
    for (int i=0; i<nums.size(); ++i) {
        mini = min(mini, nums[i]);
        maxi = max(maxi, nums[i]);
        if (maxi-mini>target) {
            maxi = nums[i];
            mini = nums[i];
            ++ret;
        }
    }
    return ret + 1;
}

int main() {
    int n = 0, k = 0;
    cin >> n >> k;
    vector<int> nums(n);
    int lo = INT_MAX, hi = INT_MIN;
    for (int i=0; i<n; ++i) {
        cin >> nums[i];
        lo = min(lo, nums[i]);
        hi = max(hi, nums[i]);
    }
    hi -= lo;
    lo = 0;
    while (lo<hi) {
        int mid = lo + (hi-lo>>1);
        int res = requiredK(nums, mid);
        if (res > k) lo = mid + 1;
        else hi = mid;
    }
    cout << lo;
}