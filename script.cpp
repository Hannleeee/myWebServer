#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    int requireD(vector<int> &weights, int target) {
        int ret = 1;
        int cnt = target;
        for (int i=0; i<weights.size(); ++i) {
            if (cnt<weights[i]) {
                ++ret;
                cnt = target;
            }
            cnt -= weights[i];
        }
        return ret;
    }

    int shipWithinDays(vector<int>& weights, int D) {
        int n = weights.size();
        int sum = accumulate(weights.begin(), weights.end(), 0);
        int lo = 0, hi = sum;
        while (lo<=hi) {
            int mid = lo + (hi-lo>>1);
            int d = requireD(weights, mid);
            cout << mid << " " << d << endl;
            d>D ? lo = mid + 1 : hi = mid-1;
        }
        return hi;
    }
};

int main() {
    Solution s;
    vector<int> weights = {1,2,3,4,5,6,7,8,9,10};
    cout << s.shipWithinDays(weights, 5);
}   