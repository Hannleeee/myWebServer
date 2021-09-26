#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 28.57%
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     * 
     * @param root TreeNode类 指向量子树的根
     * @param b int整型vector<vector<>> 表示每次交换的两个子树
     * @return TreeNode类
     */
    unordered_map<int, TreeNode *>maps;
    unordered_map<int, int> fa;
    // void preTrav(TreeNode *root) {
    //     maps[root->val] = root;
    //     if (root->left) {
    //         fa[root->left->val] = root->val;
    //         preTrav(root->left);
    //     }
    //     if (root->right) {
    //         fa[root->right->val] = root->val;
    //         preTrav(root->right);
    //     }
    // }
    void preTrav(TreeNode *root) {
        stack<TreeNode *> s;
        s.push(root);
        while (!s.empty()) {
            TreeNode *cur = s.top();
            s.pop();
            maps[cur->val] = cur;
            if (cur->left) {
                s.push(cur->left);
                fa[cur->left->val] = cur->val;
            }
            if (cur->right) {
                s.push(cur->right);
                fa[cur->right->val] = cur->val;
            }
        }
    }

    bool valid(int x, int y) {
        while (y) {
            if (x==y) return false;
            y = fa[y];
        }
        return true;
    }

    TreeNode* solve(TreeNode* root, vector<vector<int> >& b) {
        if (!root) return nullptr;
        fa[root->val] = 0;
        preTrav(root);
        for (auto &vec : b) {
            int x = vec[0], y = vec[1];
            if (valid(x, y) && valid(y, x)) {
                TreeNode *xFa = maps[fa[x]];
                TreeNode *yFa = maps[fa[y]];
                swap(fa[x], fa[y]);
                if (xFa->left->val==x && yFa->left->val==y) swap(xFa->left, yFa->left);
                else if (xFa->left->val==x && yFa->right->val==y) swap(xFa->left, yFa->right);
                else if (xFa->right->val==x && yFa->left->val==y) swap(xFa->right, yFa->left);
                else swap(xFa->right, yFa->right);
            }
        }
        return root;
    }
};