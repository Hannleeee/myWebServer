#include <bits/stdc++.h>
using namespace std;

map<string, int> precedence;
vector<string> Transfer(string &str)
{
    vector<string> result;
    stack<string> operators;
    if(str.empty()) return result;
    str.insert(0, 1, '(');
    str.push_back(')');
    int size = str.size();
 
    for(int i = 0; i < size; ++i)
    {
        if(isdigit(str[i])) 
        {
            string temp;
            while(i < size && isdigit(str[i]))
            {
                temp += str[i];
                ++i;
            }
            result.push_back(temp);
            --i;
            continue;
        }
        else if(operators.empty() || precedence[string(1, str[i])] > precedence[operators.top()])    
            operators.push(string(1, str[i]));
        else // 当前操作符的优先级大于栈顶操作符的优先级  
        {
            // 优先级大于当前操作符的操作符都要出栈
            while(!operators.empty()) 
            {
                string op = operators.top();
                if(precedence[op] >= precedence[string(1, str[i])])
                {
                    result.push_back(op);
                    operators.pop();
                }
                else break;
            }
            operators.push(string(1, str[i])); // 将当前操作符号入栈
        }    
    }
    return result;
}

bool isNumber(string &token) {
    return !(token=="+" || token=="x" || token=="@");
}

int main() {
    string s;
    cin >> s;

    precedence["+"] = 0;
    precedence["x"] = 1;
    precedence["@"] = 2;

    vector<string> tokens = Transfer(s);
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