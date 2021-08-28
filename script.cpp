#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    priority_queue<int, vector<int>, greater<>> pq1; 
    priority_queue<int, vector<int>, less<>> pq2;

    void balance() {
        while (pq1.size()>=pq2.size()+2) {
            pq2.push(pq1.top());
            pq1.pop();
        }
        while (pq2.size()>=pq1.size()+1) {
            pq1.push(pq2.top());
            pq2.pop();
        }
    }

    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<double> ret(n-k+1);

        for (int i=0; i<n; ++i) {
            if (pq1.empty() && pq2.empty()) pq1.push(nums[i]);
            else if (pq1.top()<=nums[i]) pq1.push(nums[i]);
            else pq2.push(nums[i]);

            if (i>=k) {
                if (nums[i-k+1]>=pq1.top()) {
                    while (pq1.top()!=nums[i-k+1]) {
                        pq2.push(pq1.top());
                        pq1.pop();
                    }
                    pq1.pop();
                    balance();
                }
                else {
                    while (pq2.top()!=nums[i-k+1]) {
                        pq1.push(pq2.top());
                        pq2.pop();
                    }
                    pq2.pop();
                    balance();
                }
                ret[i-k] = (pq1.size()>pq2.size()) ? pq1.top() : double(pq1.top()+pq2.top())/2.0;
            }
        }
        return ret;
    }
};

int main() {
    Solution s;
    vector<int> vec = {1,3,-1,-3,5,3,6,7};
    s.medianSlidingWindow(vec, 3);
}   