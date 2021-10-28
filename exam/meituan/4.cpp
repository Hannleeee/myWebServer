#include <bits/stdc++.h>
using namespace std;

bool isValid(int x, int a, int m, int y) {
    return int(pow(a, x)) % m == y;
}

int solve() {
    int a, m, y;
    cin >> a >> m >> y;
    for (int k = 0; k < 1000; ++k) {
        int x = log(double(m * k + y));
        if (isValid(x, a, m, y)) return x;
    }
    return -1;
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) cout << solve() << "\n";
}