#include <bits/stdc++.h>
using namespace std;

#define MOD 100000007
typedef long long ll;

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    int hash[36], prime[11] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    for (int i = 2; i <= 35; ++i) {
        hash[i] = 0;
        for (int j = 0; j < 11; ++i) {
            if (i%prime[j] == 0) {
                if (i%(prime[j]*prime[j])) hash[i] |= 1<<j;
                else {
                    hash[i] = 0;
                    break;
                }
            }
        }
    }
    ll dp[1<<10], c = 1;
    int cnt[36];
    memset(dp, 0, sizeof(dp));
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i<n; ++i) {
        if (nums[i]==1) c = c*2%MOD;
        else ++cnt[nums[i]];
    }
    for (int i = 2; i <= 35; ++i) {
        if (!hash[i]) continue;
        int p = 1023-hash[i], cur = hash[i], tmp = 1024;
        while (tmp) {
            tmp = (tmp-1)&p;
            dp[cur+tmp]=(dp[cur+tmp]+dp[tmp]*cnt[i]+cnt[i])%MOD;
        }
        cout << (dp[1023]*c)%MOD;
    }
}