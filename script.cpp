#include<bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        vector<vector<bool>> cols(9, vector<bool>(9)), blocks(9, vector<bool>(9));
        for (int i=0; i<9; ++i) {
            vector<bool> row(9);
            for (int j=0; j<9; ++j) {
                if (board[i][j]!='.') {
                    int num = board[i][j] - '1';
                    if (row[num] || cols[j][num] || blocks[i/3+j-j%3][num]) return false;
                    row[num] = cols[j][num] = blocks[i/3+j-j%3][num] = true;
                }
            }
        }
        return true;
    }
};

int main() {
    vector<vector<char>> borad = {{'8','3','.','.','7','.','.','.','.'},{'6','.','.','1','9','5','.','.','.'},{'.','9','8','.','.','.','.','6','.'},{'8','.','.','.','6','.','.','.','3'},{'4','.','.','8','.','3','.','.','1'},{'7','.','.','.','2','.','.','.','6'},{'.','6','.','.','.','.','2','8','.'},{'.','.','.','4','1','9','.','.','5'},{'.','.','.','.','8','.','.','7','9'}};
    Solution s;
    cout << s.isValidSudoku(borad);
}