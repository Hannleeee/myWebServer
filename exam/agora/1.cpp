#include <bits/stdc++.h>
using namespace std;

void printVec(vector<vector<bool>> &table) {
    for (auto &row : table) {
        for (auto has : row) {
            cout << has;
        }
        cout << "\n";
    }
}

void solve() {
    int h, w;
    cin >> h >> w;
    vector<vector<bool>> table(h, vector<bool>(w));
    for (int j = 0; j < w; j += 2) {
        table[0][j] = table[h - 1][j] = 1;
    }
    if (h < 5) {
        printVec(table);
    }
    else {
        for (int i = 2; i < h - 2; i += 2) {
            table[i][0] = table[i][w - 1] = true;
        }
        printVec(table);
    }
}

int main() {
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        solve();
        cout << "\n";
    }
}