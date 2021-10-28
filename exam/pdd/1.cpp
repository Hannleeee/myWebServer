#include <bits/stdc++.h>
using namespace std;
vector<vector<int>> dirs = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

int solve() {
    int N, M;
    cin >> N >> M;
    vector<vector<char>> board(N, vector<char> (M));
    int xk, yk;
    for (int i = 0; i < N; ++i) {
        string str;
        cin >> str;
        for (int j = 0; j < M; ++j) {
            board[i][j] = str[j];
            if (board[i][j] == 'K') { xk = i; yk = j; }
        }
    }
    int ret = INT_MAX;
    vector<vector<bool>> used(N, vector<bool>(M));
    used[xk][yk] = true;
    queue<vector<int>> q;
    q.push({xk, yk, 0});
    while (!q.empty()) {
        auto cur = q.front();
        if (board[cur[0]][cur[1]] == 'T') return cur[2] + 1;
        q.pop();
        for (int i = 0; i < 8; ++i) {
            int x = dirs[i][0] + cur[0];
            int y = dirs[i][1] + cur[1];
            if (x < 0 || x >= N || y < 0 || y >= M || used[x][y] || board[x][y] == '0') continue;
            q.push({x, y, cur[2] + 1});
            used[x][y] = true;
        }
    }
    return -1;
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) {
        cout << solve() << "\n";
    }
}