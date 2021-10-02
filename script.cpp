#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int val;
    Node *last;
    Node *next;

    Node(int k, int v) : key(k), val(v), last(nullptr), next(nullptr) {}
};

class LRUCache {
public:
    LRUCache(int capacity) {
        _capacity = capacity;
        _size = 0;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->last = head;
        cout << "Initialized\n";
    }
    
    int get(int key) {
        cout << "get " << key << "\n";
        if (!dict.count(key)) return -1;
        insertAtFront(del(dict[key]));
        return dict[key]->val;
    }
    
    void put(int key, int value) {
        cout << "put {" << key << " " << value << "}\n";
        if (!dict.count(key)) {
            if (_size==_capacity) {
                dict.erase(tail->last->key);
                del(tail->last);
                delete tail->last;
                --_size;
            }
            Node *cur = new Node(key, value);
            dict[key] = cur;
            insertAtFront(cur);
            ++_size;
        }
        else {
            dict[key]->val = value;
            insertAtFront(del(dict[key]));
        }

    }
private:
    int _capacity;
    int _size;
    Node *head;
    Node *tail;
    unordered_map<int, Node *> dict;
    void insertAtFront(Node *node) {
        cout << "updating front {" << node->key << "}\n";
        node->next = head->next;
        node->next->last = node;
        head->next = node;
        node->last = head;
    }
    Node *del(Node *node) {
        cout << "Deleting " << node->key << "\n";
        node->last->next = node->next;
        node->next->last = node->last;
        return node;
    }
};

int main() {
    LRUCache lru(2);
    lru.put(1, 1);
    lru.put(2, 2);
    cout << lru.get(1) << "\n";
    lru.put(3, 3);
    cout << lru.get(2) << "\n";
    lru.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    lru.get(1);    // 返回 -1 (未找到)
    lru.get(3);    // 返回 3
    lru.get(4);    // 返回 4
}