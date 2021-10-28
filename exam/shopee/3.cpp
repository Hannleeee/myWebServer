#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /**
     * Note: 类名、方法名、参数名已经指定，请勿修改
     *
     * 
     * if string is balanced then return true, else return false
     * @param inputStr string字符串 input string
     * @return bool布尔型
     */
    bool isStringBalance(string inputStr) {
        if (inputStr.size() == 0) return true;
        vector<int> cnt(26);
        for (char c : inputStr) {
            ++cnt[c - 'a'];
        }
        map<int, int> m;
        for (int c : cnt) {
            if (c) {
                ++m[c];
            }
        }
        if (m.size() > 2) return false;
        if (m.size() == 1) return true;
        auto p = m.begin();
        int a1 = p->first, a2 = p->second;
        ++p;
        int b1 = p->first, b2 = p->second;
        return b2 == 1 && b1-a1==1;
    }
};

int main() {
    Solution s;
    cout << s.isStringBalance("shopee");
}