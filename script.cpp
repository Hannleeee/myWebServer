#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int idx = 0;
    string fetch(string s, int n) {
        string ret;
        while (n) {
            if (s[idx++] == '-') continue;
            ret += s[idx++];
            --n;
        }
        return ret;
    }
    string licenseKeyFormatting(string s, int k) {
        int n = s.size();
        int m = 0;
        for (char &c : s) if (c == '-') ++m;

        int num = n - m;
        string ret;
        if (num%k == 0) {
            for (int i = 0; i < num/k; ++i) {
                ret += fetch(s, k) + '-';
            }
            ret.pop_back();
        }
        else {
            int p = num%k;
            ret += fetch(s, p);
            for (int i = 0; i < num/k; ++i) {
                ret += '-' + fetch(s, k);
            }
        }
        return ret;
    }
};

int main() {
    Solution s;
    string str = "5F3Z-2e-9-w";
    cout << s.licenseKeyFormatting(str, 4);
}