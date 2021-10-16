#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /* Write Code Here */
    int strstr(string a, string b) {
		int n1 = a.size(), n2 = b.size();
        for (int i=0; i<n1; ++i) {
            if (a[i] == b[0] && i+n2-1<n1) {
                bool flag = true;
                for (int k=0; k<n2; ++k) {
                    if (a[k+i] != b[k]) {
                        flag = false;
                        break;
                    }
                }
                if (flag) return i;
            }
        }
        return -1;
    }
};

int main() {
    int res;

    string _a;
    getline(cin, _a);
    string _b;
    getline(cin, _b);
    Solution *s = new Solution();
    res = s->strstr(_a, _b);
    cout << res << endl;
    
    return 0;

}
