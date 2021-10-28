#include <bits/stdc++.h>
using namespace std;

bool isTree() {
    int n, m;
    cin >> n >> m;
    if (m != n - 1) return false;
    vector<vector<int>> graph(n);
    for (int i = 0; i < m; ++i) {
        string a, b;
        cin >> a >> b;
        int u = stoi(a.substr(1, a.size() - 2)) - 1;
        int v = stoi(b.substr(0, b.size() - 1)) - 1;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<bool> used(n);
    queue<int> q;
    q.push(0);
    used[0] = true;
    int cnt = 1;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int &child : graph[cur]) {
            if (!used[child]) {
                q.push(child);
                used[child] = true;
                ++cnt;
            }
        }
    }
    return cnt == n;
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) cout << (isTree() ? "Yes" : "No") << "\n";
}