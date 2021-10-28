#include<bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<bool>> noFee(n, vector<bool>(n, true));
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        noFee[a-1][b-1] = false;
        noFee[b-1][a-1] = false;;
    }
    for (int i = 0; i < n; ++i) noFee[i][i] = false;
    int ret = 0;
    vector<bool> used(n);
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            int root = i;
            queue<int> q;
            q.push(root);
            vector<bool> s(n);
            while (!q.empty()) {
                int cur = q.front();
                q.pop();
                for (int j = 0; j < n; ++j) {
                    if (noFee[cur][j]) {
                        if (!s[j]) {
                            q.push(j);
                            s[j] = true;
                        }
                    }
                }
            }
            int cnt = 0;
            for (int i = 0; i < n; ++i) {
                if (s[i]) {
                    ++cnt;
                    used[i] = true;
                }
            }
            ret += cnt*(cnt-1)/2;
        }
    }
    cout << ret;
}