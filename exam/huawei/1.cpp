#include <bits/stdc++.h>
using namespace std;

bool judge(int k) {
    for (int m=2; m<k; ++m) {
        int upper = 2*k-m*(m-1);
        int mod = 2*m;
        if (upper%mod==0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int k = 10;
    bool ret = judge(k);
    cout << (ret ? "yes" : "no");
}