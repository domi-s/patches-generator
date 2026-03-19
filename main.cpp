#include <bits/stdc++.h>
#include "domirand.h"

using namespace std;

const int NMAX = 1000, ITERMAX = 1000, SEED = 27;
const int dx[] = {-1, 1, 0, 0},
          dy[] = {0, 0, -1, 1};

const char TB = 179;
const char TR = 192;
const char TL = 217;
const char BL = 191;
const char BR = 218;
const char LR = 196;
const char TLR = 193;
const char BLR = 194;
const char TBR = 195;
const char TBL = 180;
const char TBLR = 197;

enum Wall {
    TOP = 1,
    BOTTOM = 2,
    LEFT = 4,
    RIGHT = 8,
};

struct Region {
    int top, bottom, left, right, corner;

    int width() { return right - left + 1; }
    int height() { return bottom - top + 1; }
    int area() { return width() * height(); }

    pair<int, int> cornerCoords() {
        switch(corner) {
        case 0:
            return {top, left};
        case 1:
            return {top, right};
        case 2:
            return {bottom, left};
        case 3:
            return {bottom, right};
        }
        return {0, 0};
    }
};

DomiRand domiRand;

int n, m, regions, cornerState,
    a[NMAX + 1][NMAX + 1],
    wall[NMAX + 1][NMAX + 1];
bool isCorner[NMAX + 1][NMAX + 1];
vector<Region> frontier;

void split() {
    memset(a, 0, sizeof(a));
    memset(isCorner, 0, sizeof(isCorner));
    frontier.clear();

    frontier.push_back({1, n, 1, m, (++cornerState) % 4});

    while(frontier.size() < regions) {
        int selectedRegionIndex, iterations = 0;
        Region randomRegion;

        do {
            selectedRegionIndex = domiRand.generateRandom(frontier.size());
            randomRegion = frontier[selectedRegionIndex];

            iterations++;
        } while((randomRegion.top == randomRegion.bottom ||
                randomRegion.left == randomRegion.right) &&
                iterations <= ITERMAX);

        if(iterations == ITERMAX + 1) break;

        frontier.erase(frontier.begin() + selectedRegionIndex);

        const bool splitDirection = randomRegion.width() > randomRegion.height();
        const int top = randomRegion.top,
                  bottom = randomRegion.bottom,
                  left = randomRegion.left,
                  right = randomRegion.right;

        if(splitDirection == 0) { // Top-bottom
            const int topHeight = domiRand.generateRandom(bottom - top) + 1;

            frontier.push_back({ top, top + topHeight - 1, left, right, (++cornerState) % 4 });
            frontier.push_back({ top + topHeight, bottom, left, right, (++cornerState) % 4 });
        } else { // Left-right
            const int leftWidth = domiRand.generateRandom(right - left) + 1;

            frontier.push_back({ top, bottom, left, left + leftWidth - 1, (++cornerState) % 4});
            frontier.push_back({ top, bottom, left + leftWidth, right, (++cornerState) % 4 });
        }
    }

    int index = 0;
    for(Region &region : frontier) {
        for(int i = region.top; i <= region.bottom; i++)
            for(int j = region.left; j <= region.right; j++)
                a[i][j] = index;
        index++;
        isCorner[region.cornerCoords().first][region.cornerCoords().second] = 1;
    }
}

void walls() {
    memset(wall, 0, sizeof(wall));

    for(int i = 1; i <= n; i++)
        wall[i][1] |= LEFT,
        wall[i][m] |= RIGHT;

    for(int j = 1; j <= m; j++)
        wall[1][j] |= TOP,
        wall[n][j] |= BOTTOM;

    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            for(int k = 0; k < 4; k++) {
                const int nx = i + dx[k], ny = j + dy[k];

                if(a[i][j] != a[nx][ny])
                    wall[i][j] |= (1 << k);
            }
}

char wallChar(const int wall) {
    if(wall & TOP) {
        if(wall & BOTTOM) {
            if(wall & LEFT) {
                if(wall & RIGHT) return TBLR;
                else return TBL;
            } else {
                if(wall & RIGHT) return TBR;
                else return TB;
            }
        } else {
            if(wall & LEFT) {
                if(wall & RIGHT) return TLR;
                else return TL;
            } else {
                if(wall & RIGHT) return TR;
                else return 'T';
            }
        }
    } else {
        if(wall & BOTTOM) {
            if(wall & LEFT) {
                if(wall & RIGHT) return BLR;
                else return BL;
            } else {
                if(wall & RIGHT) return BR;
                else return 'B';
            }
        } else {
            if(wall & LEFT) {
                if(wall & RIGHT) return LR;
                else return 'L';
            } else {
                if(wall & RIGHT) return 'R';
                else return ' ';
            }
        }
    }

    return ' ';
}

void displayGrid(const int adder) {
    for(int i = 1; i <= n; i++) {
        cout << wallChar(
            TOP * (i != 1) +
            BOTTOM +
            RIGHT * !!((wall[i][1] | adder) & TOP)
        );

        for(int j = 1; j <= m; j++) {
            if((wall[i][j] | adder) & TOP) cout << LR << LR;
            else cout << "  ";

            cout << wallChar(
                TOP * (i > 1) * !!((wall[i - 1][j] | adder) & RIGHT) +
                BOTTOM * !!((wall[i][j] | adder) & RIGHT) +
                LEFT * !!((wall[i][j] | adder) & TOP) +
                RIGHT * (j < m) * !!((wall[i][j + 1] | adder) & TOP)
            );
        }
        cout << "\n";

        if((wall[i][1] | adder) & LEFT) cout << TB;
        else cout << " ";

        for(int j = 1; j <= m; j++) {
            if(isCorner[i][j]) {
                if(frontier[a[i][j]].area() < 10) cout << " ";
                cout << frontier[a[i][j]].area();
            } else
                cout << "  ";

            if((wall[i][j] | adder) & RIGHT) cout << TB;
            else cout << " ";
        }
        cout << "\n";
    }

    cout << TR;
    for(int j = 1; j <= m; j++) {
        cout << LR << LR << wallChar(
            TOP * !!((wall[n][j] | adder) & RIGHT) +
            LEFT +
            RIGHT * (j != m)
        );
    }
    cout << "\n";
}

int main()
{
    domiRand.Init(SEED);
    n = 5, m = 5;
    regions = 5;

    do {
        split();
        walls();
        system("cls");

        cout << "Patches game generator [";
        cout << "n: " << n << "\tm: " << m << "\tregions: " << regions << "\t c to exit]\n\n";

        displayGrid(TOP + BOTTOM + LEFT + RIGHT);

        cout << "\nRules: Connect the boxes together into rectangles of the indicated areas. For instance, a 9 would be part of a rectangle of sizes 1x9, 3x3, or 9x1. Any two numbers belong to different rectangles. (Press enter to reveal solution)";
        cin.get();
        cout << "The solution for this board is:\n\n";

        displayGrid(0);
    } while(cin.get() != 'c');
    return 0;
}
