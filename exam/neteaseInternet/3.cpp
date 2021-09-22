#include <bits/stdc++.h>
using namespace std;

int numberOfOne(long long n) {
    int cnt = 0;
    while (n>0) {
        ++cnt;
        n = n&(n-1);
    }
    return cnt;
}

int main() {
    long long s;
    cin >> s;
    if (!s) cout << -1;
    else {
        int n1 = numberOfOne(s);
        if (n1==1) cout << n1;
        else {
            int a = log2(s)+1;
            int n2 = numberOfOne(pow(2, a)-s);
            cout << min(n1, n2+1);
        }
    }
}