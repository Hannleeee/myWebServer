#include <bits/stdc++.h>
using namespace std;

struct point {
    int x;
    int y;
};

bool inCirc(point &p1, point &p2, int R) {
    return pow(p1.x-p2.x, 2)+pow(p1.y-p2.y, 2) <= R*R;
}

int main() {
    int R = 0, N = 0;
    cin >> R;
    cin >> N;
    vector<point> P(N);
    int xMax = 0, xMin = INT_MAX;
    int yMax = 0, yMin = INT_MAX;
    for (int i=0; i<N; ++i) {
        cin >> P[i].x >> P[i].y;
        xMax = max(xMax, P[i].x);
        xMin = min(xMin, P[i].x);
        yMax = max(yMax, P[i].y);
        yMin = min(yMin, P[i].y);
    }
    int maxCnt = 0;
    int retX, retY;
    for (int x=xMin; x<xMax+R; ++x) {
        for (int y=yMin; y<yMax+R; ++y) {
            point center;
            center.x = x; center.y = y;
            int cnt = 0;
            for (int i=0; i<N; ++i) {
                if (inCirc(P[i], center, R)) ++cnt;
            }
            if (cnt>maxCnt) {
                maxCnt = cnt;
                retX = x;
                retY = y;
            }
        }
    }
    cout << retX << " " << retY;
}