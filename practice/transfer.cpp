#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ll num = 0;
    cin >> num;
    string ret = "";
    bool isNeg = 0;
    if (num<0) {
        isNeg = 1;
        num = -num;
    }
    else if (!num) ret = "0";
    int i = 0;
    while (num>0) {
        ++i;
        if (i%3==0 && num>=10) {
            ret = ',' + to_string(num%10) + ret;;
        }
        else ret = to_string(num%10) + ret;
        num /= 10;
    }
    if (isNeg) ret = '-' + ret;
    cout << ret << "\n";
}