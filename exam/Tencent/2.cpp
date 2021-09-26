#include <bits/stdc++.h>
using namespace std;

// 66.67%
unordered_map<int, int> scores;
int getScore(int idx, const vector<int> &nums) {
    int score = 0, init = idx;
    while (idx<nums.size()) {
        if (scores[idx]) {
            score += scores[idx];
            scores[init] = score;
            return score;
        }
        else {
            score += nums[idx];
            idx += nums[idx];
        }
    }
    return score;
}

void solve() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i=0; i<n; ++i) cin >> nums[i];
    int ret = 0;
    for (int i=n-1; i>=0; --i) {
        ret = max(ret, getScore(i, nums));
    }
    cout << ret << "\n";
}

int main() {
    int T;
    cin >> T;
    for (int i=0; i<T; ++i) {
        solve();
    }
}