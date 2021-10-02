#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
vector<vector<ll>> ret;
vector<ll> nums;
vector<int> states;
void valid(int i) {
    ll prod = ll(i)*i, temp = prod;
    int state = 0;
    if (prod==0) state = 1;
    else {
        while (prod>0) {
            int m = prod%10;
            if (state & (1<<m)) return;
            state |= (1<<m);
            prod /= 10;
        }
    }
    nums.push_back(temp);
    states.push_back(state);
}

void dfs(int idx, int state, vector<ll> &cur) {
    if (state==0x3ff) {
        ret.push_back(cur);
        for (auto &nn : cur) cout << nn << " ";
        cout << endl;
        return;
    }
    for (int i=idx; i<nums.size(); ++i) {
        if ((state & states[i]) == 0) {
            cur.push_back(nums[i]);
            dfs(i+1, state | states[i], cur);
            cur.pop_back();
        }
    }
}

int main() {
    int n = sqrt(9876543210);
    for (int i=0; i<n; ++i) {
        valid(i);
    }
    vector<ll> cur;
    cout << nums.size() << "\n";
    dfs(0, 0, cur);
    cout << ret.size();
}