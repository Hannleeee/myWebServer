#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int prevSum(vector<int> &vec) {
        int n = vec.size();
        set<int> s;
        int ret = 0;
        for (int i=0; i<n; ++i) {
            if (s.empty() || *s.begin()>=vec[i]) {
                s.insert(vec[i]);
                continue;
            }
            auto p = s.lower_bound(vec[i]);
            if(p==s.end()) ret += *s.rbegin() * (i+1);
            else {
                while(*p>=vec[i]) --p;
                ret += (*p) * (i+1);
            };
            s.insert(vec[i]);
        }
        return ret;
    }
};

int main () {
    vector<int> vec = {1,6,3,3,8};
    Solution s;
    cout << s.prevSum(vec);
}