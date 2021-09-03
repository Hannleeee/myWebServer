#include<bits/stdc++.h>
using namespace std;

class heap {
public:
    heap() {
        nums = new vector<int>();
        _capacity = 0;
        _size = 0;
    }

    heap(vector<int> &vec) {
        // nums.assign(vec.begin(), vec.end());
        nums = &vec;
        _capacity = _size = nums->size();
        heapify();
    }

    bool empty() {return _size==0;}
    int size() { return _size; }
    int top() { return (*nums)[0]; }

    void push(int num) {
        if (_size==_capacity) {
            nums->push_back(num);
            ++_capacity;
        }
        else (*nums)[_size] = num;
        ++_size;
        filterUp(_size-1);
    }

    void pop() {
        swap((*nums)[0], (*nums)[_size-1]);
        --_size;
        filterDown(0);
    }

    vector<int>::iterator getHeap() { return nums->begin(); }

    void heapSort() {
        while (_size) pop();
    }
private:
    vector<int> *nums;
    int _size, _capacity;
    bool inHeap(int i) { return i>=0 && i<_size; }
    int father(int i) { return (i-1)>>1; }
    int lChild(int i) { return (i<<1)+1; }
    int rChild(int i) { return (i+1)<<1; }
    int properFather(int i) {
        int father = i, l = lChild(i), r = rChild(i);
        if (inHeap(l)&&(*nums)[l]>(*nums)[father]) father = l;
        if (inHeap(r)&&(*nums)[r]>(*nums)[father]) father = r;
        return father;
    }
    void filterUp(int i) {
        int temp = (*nums)[i];
        while (i>0 && temp>(*nums)[father(i)]) {
            (*nums)[i] = (*nums)[father(i)];
            i = father(i);
        }
        (*nums)[i] = temp;
    }

    void filterDown(int i) {
        int father;
        while ((father=properFather(i)) != i) {
            swap((*nums)[i], (*nums)[father]);
            i = father;
        }
    }

    void heapify() {
        for (int i=father(_size-1); inHeap(i); --i) filterDown(i);
    }
};

class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        heap h;
        for (int num : arr) {
            if (h.size()<k) h.push(num);
            else if (h.top()>num) {
                h.pop();
                h.push(num);
            }
        }
        auto it = h.getHeap();
        return vector<int> (it, it+h.size());
    }
};

int main() {
    Solution s;
    vector<int> a = {1,7,6,4,5,6,11,9};
    s.smallestK(a, 4);
}   