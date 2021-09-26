#include <bits/stdc++.h>
using namespace std;

struct score {
    int upper;
    int under;

    bool operator<(score &other) {
        return upper*other.under > other.upper*under;
    }
};

int main() {
    int n;
    cin >> n;
    vector<score> scores(n);
    for (int i=0; i<n; ++i) {
        cin >> scores[i].upper >> scores[i].under;
    }
    sort(scores.begin(), scores.end());
    for (auto &s : scores) {
        cout << s.upper << " " << s.under << "\n";
    }
}