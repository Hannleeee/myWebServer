#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    /* Write Code Here */
    vector<int> convert(string str) {
        int n = str.size();
        vector<int> ret;
        for (int i=1; i<n; ) {
            int j = i;
            while (j < n && str[j]!=',' && str[j]!=']') ++j;
            ret.push_back(stoi(str.substr(i, j-i)));
            if (str[j] == ']') break;
            i = j + 1;
        }
        return ret;
    }

    int valid(int i, vector<int> &nums, vector<vector<int>> &rules) {
        int ret = 0;
        for (auto &rule : rules) {
            int T = rule[0], N = rule[1], t = rule[2];
            int cnt = 0;
            for (int j=i; j>=0 && j>=j-T; --j) {
                if (nums[i]) ++cnt;
                if (cnt >= N) {
                    ret = max(ret, t);
                    break;
                }
            }
        }
        return ret;
    }

    int check(string input, int rule_cnt) {
        vector<int> nums = convert(input);
        vector<vector<int>> rules;
        int ret = 0;
        for (int i=0; i<rule_cnt; ++i) {
            string rule;
            cin >> rule;
            rules.push_back(convert(rule));
        }
        int blockedUntil = -1;
        for (int i=0; i<nums.size(); ) {
            if (i<=blockedUntil) {
                if (nums[i]) {
                    ++ret;
                    nums[i] = 0;
                }
                ++i; 
            }
            else {
                blockedUntil = i + valid(i, nums, rules) - 1;
            }

        }

        return ret;
    }
};

int main() {
    string _input;
    cin >> _input;
    int _rule_cnt;
    cin >> _rule_cnt;

    Solution s;
    cout << s.check(_input, _rule_cnt) << endl;
    
    return 0;

}