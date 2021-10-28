#include <bits/stdc++.h>
using namespace std;

int solve() {
    int C, M, N;
    cin >> C >> M >> N;
    vector<int> rooms(M);
    for (int i = 0; i < C; ++i) {
        int num;
        cin >> num;
        ++rooms[num - 1];
    }
    vector<vector<int>> idxs;
    int ret = 0;
    for (int i = 0; i < M; ) {
        if (rooms[i]) {
            int j = i;
            while (rooms[j]) {
                ++j;
            }
            ret += j - i;
            idxs.push_back({i, j});
            i = j;
        }
        while (i < M && !rooms[i]) ++i;
    }
    if (idxs.size() <= N) return ret;
    
    int rest = N - idxs.size();
    vector<int> gaps;
    for (int i = 0; i < idxs.size() - 1; ++i) {
        gaps.push_back(idxs[i + 1][0] - idxs[i][1]);
    }
    sort(gaps.begin(), gaps.end());
    for (int i = 0; i < rest; ++i) ret += gaps[i];
    return ret;
}

int main() {
    cout << solve() << "\n";
}