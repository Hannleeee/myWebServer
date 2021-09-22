#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 0;
    cin >> n;
    int tmp = n;
    int cnt = 0;
    while (tmp>0) {
        int cur = tmp%10;
        if (cur && n%cur==0) ++cnt;
        tmp /= 10;
    }
    cout << cnt;
}