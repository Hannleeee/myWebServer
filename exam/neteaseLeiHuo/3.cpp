#include <bits/stdc++.h>
using namespace std;
// int find(char b, string a) {
//     int ret = 0;
//     int maxCnt = 0;
//     for (int i=0; i<a.size(); ) {
//         int cnt = 0;
//         while (i<a.size() && a[i]==b) {
//             ++cnt;
//             ++i;
//         }
//         if (cnt>maxCnt) {
//             ret = 
//         }

//     }
// }

int update(string &a, int M) {
    int n = a.size();
    int addScore = 0;
    while (1) {
        int i = 0;
        int maxCnt = 0;
        while (i<n) {
            int cnt = 1;
            while (i<n-1 && a[i]==a[i+1]) {
                ++cnt;
                ++i;
            }
            maxCnt = max(maxCnt, cnt);
            if (cnt >= M) {
                addScore += cnt;
                a.erase(i-cnt+1, cnt);
                break;
            }
            ++i;
        }
        if (maxCnt < M) break;
    }
    return addScore;
}

int maxScore = 0;
void dfs(string a, string b, int i, int score, int M) {
    maxScore = max(maxScore, score);
    if (i==b.size()) return;
    char toAdd = b[i];
    for (int j=0; j<a.size(); ++j) {
        string newA = a.substr(0, j) + toAdd + a.substr(j, a.size()-j);
        int addScore = update(newA, M);
        dfs(newA, b, i+1, score+addScore, M);
    }
}
int main() {
    int L1, L2, M;
    cin >> L1 >> L2 >> M;
    string a, b;
    cin >> a;
    cin >> b;
    // dfs(a, b, 0, 0, M);
    // cout << maxScore;

}