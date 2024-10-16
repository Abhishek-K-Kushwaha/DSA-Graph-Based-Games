#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include "queue.hpp"

using namespace std;

// Note: To run the program, the stack size will need to be increased

enum Color { RED, GREEN, BLUE, WHITE, YELLOW, ORANGE };

struct Cube {
    Color e[6][2][2];
};

struct Block {
    int face;
    int row;
    int column;
};

constexpr int factorial(int i) {
    int p = 1;
    for (int j = 2; j <= i; ++j) {
        p *= j;
    }
    return p;
}

Block getBlockValues(const string& input) {
    static const vector<pair<string, Block>> blockMap = {
        {"front U right", {0, 0, 1}},
        {"front U left", {0, 0, 0}},
        {"front L right", {0, 1, 1}},
        {"front L left", {0, 1, 0}},
        {"bottom U right", {4, 0, 1}},
        {"bottom U left", {4, 0, 0}},
        {"bottom L right", {4, 1, 1}},
        {"bottom L left", {4, 1, 0}},
        {"right U right", {1, 0, 1}},
        {"right U left", {1, 0, 0}},
        {"right L right", {1, 1, 1}},
        {"right L left", {1, 1, 0}},
        {"left U right", {2, 0, 1}},
        {"left U left", {2, 0, 0}},
        {"left L right", {2, 1, 1}},
        {"left L left", {2, 1, 0}},
        {"top U right", {3, 0, 1}},
        {"top U left", {3, 0, 0}},
        {"top L right", {3, 1, 1}},
        {"top L left", {3, 1, 0}},
        {"back U right", {5, 0, 1}},
        {"back U left", {5, 0, 0}},
        {"back L right", {5, 1, 1}},
        {"back L left", {5, 1, 0}}
    };

    auto it = find_if(blockMap.begin(), blockMap.end(),
                      [&input](const auto& pair) { return pair.first == input; });

    return (it != blockMap.end()) ? it->second : Block{-1, -1, -1};
}

void printCube(const Cube& c) {
    const char* faceNames[6] = {"Front Face", "Right Face", "Left Face", "Top Face", "Bottom Face", "Back Face"};
    const char* colorNames[] = {"RED", "GREEN", "BLUE", "WHITE", "YELLOW", "ORANGE"};

    for (int face = 0; face < 6; ++face) {
        printf("%s\n", faceNames[face]);
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 2; ++col) {
                printf("%s ", colorNames[c.e[face][row][col]]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void swap(Color &a, Color &b) {
    Color temp = a;
    a = b;
    b = temp;
}

void faceCycle(int faceIdx, Cube& o) {
    swap(o.e[faceIdx][0][0], o.e[faceIdx][0][1]);
    swap(o.e[faceIdx][0][0], o.e[faceIdx][1][0]);
    swap(o.e[faceIdx][1][0], o.e[faceIdx][1][1]);
}

void faceCycleDash(int faceIdx, Cube& o) {
    swap(o.e[faceIdx][0][1], o.e[faceIdx][0][0]);
    swap(o.e[faceIdx][0][1], o.e[faceIdx][1][1]);
    swap(o.e[faceIdx][1][1], o.e[faceIdx][1][0]);
}

Cube R(const Cube& c) {
    Cube o = c;
    vector<string> L = {"front U right", "top L right", "top U right", "back L right",
                        "back U right", "bottom L right", "bottom U right", "front L right"};

    for (int i = 5; i >= 0; i -= 1) {
        Block next = getBlockValues(L[i + 2]);
        Block prev = getBlockValues(L[i]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }
    o.e[0][0][1] = c.e[4][0][1];
    o.e[3][1][1] = c.e[0][1][1];

    faceCycle(1, o);
    return o;
}

Cube U(const Cube& c) {
    Cube o = c;
    vector<string> L = {"front U right", "right U left", "right U right", "back L right",
                        "back L left", "left U left", "left U right", "front U left"};

    for (int i = 0; i <= 5; i += 1) {
        Block next = getBlockValues(L[i]);
        Block prev = getBlockValues(L[i + 2]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }
    o.e[2][0][1] = c.e[0][0][1];
    o.e[0][0][0] = c.e[1][0][0];
    faceCycle(3, o);
    return o;
}

Cube U_dash(const Cube& c) {
    Cube o = c;
    vector<string> L = {"front U right", "right U left", "right U right", "back L right",
                        "back L left", "left U left", "left U right", "front U left"};
    reverse(L.begin(), L.end());
    for (int i = 0; i <= 5; i += 1) {
        Block next = getBlockValues(L[i]);
        Block prev = getBlockValues(L[i + 2]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }
    o.e[0][0][1] = c.e[2][0][1];
    o.e[1][0][0] = c.e[0][0][0];
    faceCycleDash(3, o);
    return o;
}

Cube F(const Cube& c) {
    Cube o = c;
    vector<string> L = {"top L right", "right U left", "right L left", "bottom U right",
                        "bottom U left", "left L right", "left U right", "top L left"};
    for (int i = 7; i >= 2; i--) {
        Block next = getBlockValues(L[i]);
        Block prev = getBlockValues(L[i - 2]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }

    o.e[3][1][1] = c.e[2][0][1];
    o.e[1][0][0] = c.e[3][1][0];
    faceCycle(0, o);

    return o;
}

Cube R_dash(const Cube& c) {
    Cube o = c;
    vector<string> L = {"front U right", "top L right", "top U right", "back L right",
                        "back U right", "bottom L right", "bottom U right", "front L right"};
    reverse(L.begin(), L.end());
    for (int i = 5; i >= 0; i -= 1) {
        Block next = getBlockValues(L[i + 2]);
        Block prev = getBlockValues(L[i]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }
    o.e[4][0][1] = c.e[0][0][1];
    o.e[0][1][1] = c.e[3][1][1];

    faceCycleDash(1, o);
    return o;
}

Cube F_dash(const Cube& c) {
    Cube o = c;
    vector<string> L = {"top L right", "right U left", "right L left", "bottom U right",
                        "bottom U left", "left L right", "left U right", "top L left"};
    reverse(L.begin(), L.end());
    for (int i = 7; i >= 2; i--) {
        Block next = getBlockValues(L[i]);
        Block prev = getBlockValues(L[i - 2]);
        o.e[next.face][next.row][next.column] = o.e[prev.face][prev.row][prev.column];
    }

    o.e[2][0][1] = c.e[3][1][1];
    o.e[3][1][0] = c.e[1][0][0];
    faceCycleDash(0, o);

    return o;
}

string colorToChar(Color c) {
    const char* colorChars = "RGBWYO";
    return string(1, colorChars[c]);
}

int check(Cube& c, int cubelet) {
    string colors;
    if (cubelet == 0) {
        colors = colorToChar(c.e[0][0][0]) + colorToChar(c.e[2][0][1]) + colorToChar(c.e[3][1][0]);
    } else {
        colors = colorToChar(c.e[2][1][0]) + colorToChar(c.e[4][1][0]) + colorToChar(c.e[5][0][0]);
    }

    static const vector<pair<string, int>> colorMap = {
        {"YRB", 1}, {"RBY", 2}, {"BYR", 3}, {"GWR", 4}, {"WRG", 5}, {"RGW", 6},
        {"BRW", 7}, {"RWB", 8}, {"WBR", 9}, {"GOW", 10}, {"OWG", 11}, {"WGO", 12},
        {"YBO", 13}, {"BOY", 14}, {"OYB", 15}, {"OBW", 16}, {"BWO", 17}, {"WOB", 18},
        {"GRY", 19}, {"RYG", 20}, {"YGR", 21}, {"OGY", 22}, {"GYO", 23}, {"YOG", 24}
    };

    auto it = find_if(colorMap.begin(), colorMap.end(),
                      [&colors](const auto& pair) { return pair.first == colors; });

    return (it != colorMap.end()) ? it->second : 0;
}

int ord(Cube& c) {
    long long int ans = 0;
    int figure = 0;
    vector<Cube (*)(const Cube&)> moves = {U, F, F, nullptr, R_dash, U_dash, U};
    vector<Cube (*)(const Cube&)> undoMoves = {nullptr, F_dash, F_dash, nullptr, R, U, U_dash};

    for (int count = 0; count <= 7; count++) {
        int var = check(c, count == 4);
        ans += var * pow(10, figure);
        figure += (var < 10) ? 1 : 2;

        if (moves[count]) {
            c = moves[count](c);
            if (undoMoves[count]) c = undoMoves[count](c);
        }
    }

    return ans % 360007;
}

bool similarStrings(const string& s1, const string& s2) {
    vector<int> count(256, 0);
    for (int i = 0; i < 3; i++) {
        count[s1[i]]++;
        count[s2[i]]--;
    }
    return all_of(count.begin(), count.end(), [](int i) { return i == 0; });
}

const vector<vector<vector<char>>> solvedCube = {
    {{'W','G','O'},{'W','O','B'},{'W','B','R'},{'W','R','G'}},
    {{'Y','B','O'},{'Y','O','G'},{'Y','G','R'},{'Y','R','B'}}
};

bool isValid(Cube& c) {
    vector<vector<vector<string>>> L = {
        {{"front U left", "left U right", "top L left"},
         {"front U right", "top L right", "right U left"},
         {"front L right", "right L left", "bottom U right"},
         {"front L left", "bottom U left", "left L right"}},
        {{"back L right", "right U right", "top U right"},
         {"back L left", "top U left", "left U left"},
         {"back U left", "left L left","bottom L left"},
         {"back U right","bottom L right","right L right"}}
    };

    vector<vector<vector<char>>> orientation(2, vector<vector<char>>(4));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                Block a = getBlockValues(L[i][j][k]);
                orientation[i][j].push_back("RGBWYO"[c.e[a.face][a.row][a.column]]);
            }
        }
    }

    int sum = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            string cubelet(orientation[i][j].begin(), orientation[i][j].end());
            for (int k = 0; k < 2; ++k) {
                for (int l = 0; l < 4; ++l) {
                    string cubeletInSolved(solvedCube[k][l].begin(), solvedCube[k][l].end());
                    if (similarStrings(cubelet, cubeletInSolved)) {
                        sum += (cubelet[0] == cubeletInSolved[2]) ? 1 : 
                               (cubelet[1] == cubeletInSolved[2]) ? 2 : 0;
                    }
                }
            }
        }
    }
    return sum % 3 == 0;
}

bool isSolved(const Cube& o) {
    for (size_t face = 0; face < 6; face++) {
        Color sample = o.e[face][0][0];
        for (size_t row = 0; row < 2; row++) {
            for (size_t col = 0; col < 2; col++) {
                if (o.e[face][row][col] != sample) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool areEqual(const Cube& a, const Cube& b) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                if (a.e[i][j][k] != b.e[i][j][k]) return false;
            }
        }
    }
    return true;
}

struct CubeInfo {
    Cube parent;
    string move;
    bool visited = false;
    int ord = -1;
    Cube state;
};

const Cube DEFAULT_CUBE = {{{{RED, RED}, {RED, RED}}, {{RED, RED}, {RED, RED}}, {{RED, RED}, {RED, RED}},
                            {{RED, RED}, {RED, RED}}, {{RED, RED}, {RED, RED}}, {{RED, RED}, {RED, RED}}}};

vector<vector<CubeInfo>> hashTable(360010);

CubeInfo hashFunction(int ordA, const Cube& c) {
    auto& a = hashTable[ordA];
    auto it = find_if(a.begin(), a.end(), [&c](const CubeInfo& ci) { return areEqual(c, ci.state); });
    return (it != a.end()) ? *it : CubeInfo{DEFAULT_CUBE, "", false, -1};
}

vector<string> solve(const Cube& unsolved) {
    queue<Cube, factorial(10)> q;
    Cube c = unsolved;
    enqueue(q, c);
    int cOrd = ord(c);
    hashTable[cOrd].push_back({c, "", true, cOrd, c});

    while (!is_queue_empty(q)) {
        Cube u = dequeue(q);
        if (isSolved(u)) {
            vector<string> moves;
            Cube currentCube = u;
            int currentOrd = ord(currentCube);
            auto store = hashFunction(currentOrd, currentCube);
            while (!areEqual(unsolved, currentCube)) {
                moves.push_back(store.move);
                currentCube = store.parent;
                currentOrd = ord(currentCube);
                store = hashFunction(currentOrd, currentCube);
            }
            reverse(moves.begin(), moves.end());
            return moves;
        }

        vector<pair<Cube, string>> nextMoves = {
            {R(u), "R"}, {U(u), "U"}, {F(u), "F"},
            {R_dash(u), "R'"}, {U_dash(u), "U'"}, {F_dash(u), "F'"},
            {R(R(u)), "R2"}, {F(F(u)), "F2"}, {U(U(u)), "U2"}
        };

        for (const auto& [nextCube, moveName] : nextMoves) {
            int nextOrd = ord(nextCube);
            if (!hashFunction(nextOrd, nextCube).visited) {
                hashTable[nextOrd].push_back({u, moveName, true, nextOrd, nextCube});
                enqueue(q, nextCube);
            }
        }
    }

    assert(false); // Should never reach here if the cube is solvable
    return {};
}

int main() {
    Cube rubix2 = {{{{BLUE, YELLOW}, {YELLOW, GREEN}}, {{RED, ORANGE}, {WHITE, BLUE}}, {{GREEN, RED}, {ORANGE, BLUE}},
                    {{WHITE, GREEN}, {WHITE, GREEN}}, {{RED, RED}, {YELLOW, ORANGE}}, {{BLUE, WHITE}, {ORANGE, YELLOW}}}};
    
    printCube(rubix2);
    
    for (int i = 0; i < 360011; i++) {
        hashTable[i].clear();
    }
    
    if (!isValid(rubix2)) {
        cout << "The cube is not solvable" << endl;
        return 0;
    }
    
    cout << "The cube is solvable" << endl;
    vector<string> moves = solve(rubix2);
    
    cout << "Solution: ";
    for (const auto& move : moves) {
        cout << move << ' ';
    }
    cout << endl;

    return 0;
}
