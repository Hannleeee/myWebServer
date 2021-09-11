#include <bits/stdc++.h>
using namespace std;

vector<string> dirs;

void pwd() {
    string ret = "\\";
    if (dirs.size())  {
        for (int i=0; i<dirs.size()-1; ++i) {
            ret += dirs[i] + '\\';
        }
        ret += dirs.back();
    }
    cout << ret << endl;
}

int main() {
    int n = 0;
    cin >> n;
    
    for (int i=0; i<n; ++i) {
        string cmd = "", dir = "";
        cin >> cmd;
        if (cmd=="cd") {
            cin >> dir;
            if (dir == "..") {
                if (dirs.size()) dirs.pop_back();
            }
            else {
                dirs.push_back(dir);
            }
        }
        else {
            pwd();
        }
    }
    
}