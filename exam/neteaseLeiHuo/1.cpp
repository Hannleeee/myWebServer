#include <bits/stdc++.h>
using namespace std;

struct pic {
    int id;
    int x;
    int y;
    int z;
    int w;

    bool operator<(pic &other) {
        return id<other.id;
    }
};

int main() {
    int N, M;
    cin >> N >> M;
    vector<pic> pics(N*M);
    for (int i=0; i<N*M; ++i) {
        cin >> pics[i].id >> pics[i].x >> pics[i].y >> pics[i].z >> pics[i].w;
    }
    sort(pics.begin(), pics.end());
    // find left top
    int left = 0;
    for (int i=0; i<N*M; ++i) {
        if (pics[i].x==0 && pics[i].y==0) {
            left = pics[i].id;
            break;
        }
    }
    for (int i=0; i<N; ++i) {
        int p = left;
        left = pics[left-1].w;
        for (int j=0; j<M; ++j) {
            cout << p << " ";
            p = pics[p-1].z;
        }
        cout << "\n";
    }
}