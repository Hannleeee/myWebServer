#include <bits/stdc++.h>
using namespace std;

// int scm(int m, int n) {
//     int a = m, b = n;
//     while (a!=b) {
//         if (a>b) a = a - b;
//         else b = b - a;
//     }
//     return m*n/a;
// }

// int solve(int x) {
//     int f1 = 1+x, f2 = f1+x, f3 = f2+x;
//     int g1 = scm(f1, f2);
//     if (g1>f3) return g1;
//     else return scm(g1, f3);
// }

// int main() {
//     int T;
//     cin >> T;
//     for (int i=0; i<T; ++i) {
//         int x;
//         cin >> x;
//         cout << solve(x) << "\n";
//     }
// }

bool valid(int n, int x) {
    return 
}

int main() {
    int T;
    cin >> T;
    for (int i=0; i<T; ++i) {
        int x;
        cin >> x;
        cout << solve(x) << "\n";
    }
}