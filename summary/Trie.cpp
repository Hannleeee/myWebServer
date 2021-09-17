#include <bits/stdc++.h>
using namespace std;

class Trie {
public:
    Trie () :isEnd(false), children(26) {}

    void insert(string s) {
        Trie *cur = this;
        for (char c : s) {
            if (!cur->children[c-'a']) cur->children[c-'a'] = new Trie();
            cur = cur->children[c-'a'];
        }
        cur->isEnd = true;
    }

    int search(string s) {
        Trie *cur = this;
        for (char c : s) {
            Trie *child = cur->children[c-'a'];
            if (!child) return 0;
            cur = child;
        }
        if (!cur->isEnd) return 1;      // 只是前缀返回1
        else return 2;                  // 字典包含s
    }
private:
    bool isEnd;
    vector<Trie *> children;
};

int main() {
    Trie trie;
    vector<string> input = {"oath","pea","eat","rain"};
    for (string &str : input) {
        trie.insert(str);
        cout << trie.search(str) << "\n";
    }
    cout << trie.search("oat") << "\n";
    cout << trie.search("oathe") << "\n";
}