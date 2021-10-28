#include <bits/stdc++.h>

using namespace std;
class Solution {
public:
    /* Write Code Here */
    unordered_map<int, vector<int>> fa;
    
    bool check(int a, int b) {
        queue<int> q;
        q.push(a);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (auto father : fa[cur]) {
                if (father == b) return true;
                q.push(father);
            }
        }
        return false;
    }

    vector<int> checkDependency(int n, vector<vector<int>> relations, vector<vector<int>> queries) {
        for (auto &relation : relations) {
            fa[relation[1]].push_back(relation[0]);
        }
        vector<int> ret(queries.size());
        for (int i = 0; i < queries.size(); ++i) {
            if (check(queries[i][1], queries[i][0])) ret[i] = 1;
        }
        return ret;
    }
};
int main() {
    vector < int > res;

    int n;
    cin >> n;
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');


    int relations_rows = 0;
    int relations_cols = 0;
    cin >> relations_rows >> relations_cols;
    vector< vector < int > > relations(relations_rows);
    for(int relations_i=0; relations_i<relations_rows; relations_i++) {
        for(int relations_j=0; relations_j<relations_cols; relations_j++) {
            int relations_tmp;
            cin >> relations_tmp;
            relations[relations_i].push_back(relations_tmp);
        }
    }
    int queries_rows = 0;
    int queries_cols = 0;
    cin >> queries_rows >> queries_cols;
    vector< vector < int > > queries(queries_rows);
    for(int queries_i=0; queries_i<queries_rows; queries_i++) {
        for(int queries_j=0; queries_j<queries_cols; queries_j++) {
            int queries_tmp;
            cin >> queries_tmp;
            queries[queries_i].push_back(queries_tmp);
        }
    }
    Solution *s = new Solution();
    res = s->checkDependency(n, relations, queries);
    for(int res_i=0; res_i < res.size(); res_i++) {
    	cout << res[res_i] << endl;;
    }
    
    return 0;

}
