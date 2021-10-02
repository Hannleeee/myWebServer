#include <bits/stdc++.h>
using namespace std;

bool isNumber(string &token) {
    return !(token=="+" || token=="x" || token=="@");
}

int main() {
    string s;
    cin >> s;

    vector<string> tokens;
    int m = s.size();
    for (int i=0; i<m;) {
        if (s[i]>='0' && s[i]<='9') {
            int j = i;
            while (s[j]>='0' && s[j]<='9') ++j;
            tokens.push_back(s.substr(i, j-i));
            i = j;
        }
        else tokens.push_back(s.substr(i++, 1));
    }
    int n = tokens.size();

    stack<int> stk;
    for (int i=0; i<n; ++i) {
        string &token = tokens[i];
        if (isNumber(token)) {
            stk.push(atoi(token.c_str()));
        }
        else {
            int n2 = stk.top();
            stk.pop();
            int n1 = stk.top();
            stk.pop();
            switch (token[0]) {
                case '+':
                    stk.push(n1+n2);
                    break;
                case 'x':
                    stk.push(n1*n2);
                    break;
                case '@':
                    stk.push(n1|(n1+n2));
                    break;
            }
        }
    }
    cout << stk.top();
}