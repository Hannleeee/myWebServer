#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> nums(n, vector<int>(m));
    vector<int> sums(n);
    vector<unordered_map<int, bool>> record(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> nums[i][j];
            record[i][nums[i][j]] = true;
            sums[i] += nums[i][j];
        }
    }
    
    vector<vector<int>> ret;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int diff = sums[i] - sums[j];
            for (int k = 0; k < m; ++k) {
                if (record[j][nums[i][k] - diff]) {
                    ret.push_back({i, j});
                    break;
                }
            }
        }
    }
    cout << (ret.size() == 0 ? -1 : ret.size()) << "\n";
    for (auto &vec : ret) {
        cout << vec[0] + 1 << " " << vec[1] + 1 << "\n";
    }
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) {
        solve();
    }
}