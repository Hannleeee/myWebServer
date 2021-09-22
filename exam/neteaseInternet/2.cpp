#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    int m;
    cin >> s >> m;
    int n = s.size();
    int maxi = 0, sum = 0, ans = 1;
    for (int i=1; i<n; ++i) {
        int dis = abs(s[i]-s[i-1]);
        if (s[i]<s[i-1]) {
            dis = min(dis, s[i]+26-s[i-1]);
        }
        else {
            dis = min(dis, s[i-1]+26-s[i]);
        }
        if (i<=m) {
            sum += dis;
        }
        else {
            int dism = abs(s[i-m]-s[i-m-1]);
            if (s[i-m])
    }
    ans -= max(maxi-m, 0);
    cout << ans;
}