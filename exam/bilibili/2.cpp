#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /* Write Code Here */
    string reorder(string a, string b) {
        int m = a.size(), n = b.size();
        unordered_set<char> s;
        for (auto &ch : b) {
            s.insert(ch);
        }
        int j = 0;
        for (int i = 0; i < m; ++i) {
            if (s.count(a[i])) {
                a[i] = b[j++];
            }
        }
        return a;
    }
};
int main() {
    string res;

    string _a;
    getline(cin, _a);
    string _b;
    getline(cin, _b);
    Solution *s = new Solution();
    res = s->reorder(_a, _b);
    cout << res << endl;
    
    return 0;

}