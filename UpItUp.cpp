#include <cstdio>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <cassert> // Added assert header for completeness
using namespace std;

#include "queue.hpp"

enum state { TOP, RIGHT, LEFT, FRONT, BACK, BOTTOM, EMPTY };

// Taking white as reference
// Opposite side has black color
/* 
   White = CCL logo
   Black = IITGN logo
*/

struct board {
    int e[3][3];
};

// A utility function to print the board
void print_board(const board& b) {
    char state_arr[7][15] = { "Top", "Right", "Left", "Front", "Back", "Bottom", "Empty" };

    for (size_t row = 0; row < 3; row++) {
        for (size_t col = 0; col < 3; col++) {
            printf("%s ", state_arr[b.e[row][col]]);
        }
        printf("\n");
    }
    printf("\n\n");
}

// Function to find the empty space on the board
// Outputs row and column of EMPTY
tuple<int, int> find_space(const board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (b.e[r][c] == EMPTY) {
                return make_tuple(r, c);
            }
        }
    }
    assert(0); // Should never reach here
}

// Moves the empty space upwards
board up(const board& b) {
    const auto [r, c] = find_space(b);
    if (r == 0) return b;

    board o = b;
    switch (o.e[r - 1][c]) {
        case TOP:    o.e[r][c] = FRONT;  o.e[r - 1][c] = EMPTY; break;
        case RIGHT:  o.e[r][c] = RIGHT;  o.e[r - 1][c] = EMPTY; break;
        case LEFT:   o.e[r][c] = LEFT;   o.e[r - 1][c] = EMPTY; break;
        case FRONT:  o.e[r][c] = BOTTOM; o.e[r - 1][c] = EMPTY; break;
        case BACK:   o.e[r][c] = TOP;    o.e[r - 1][c] = EMPTY; break;
        case BOTTOM: o.e[r][c] = BACK;   o.e[r - 1][c] = EMPTY; break;
    }
    return o;
}

// Moves the empty space downwards
board down(const board& b) {
    const auto [r, c] = find_space(b);
    if (r == 2) return b;

    board o = b;
    switch (o.e[r + 1][c]) {
        case TOP:    o.e[r][c] = BACK;   o.e[r + 1][c] = EMPTY; break;
        case RIGHT:  o.e[r][c] = RIGHT;  o.e[r + 1][c] = EMPTY; break;
        case LEFT:   o.e[r][c] = LEFT;   o.e[r + 1][c] = EMPTY; break;
        case FRONT:  o.e[r][c] = TOP;    o.e[r + 1][c] = EMPTY; break;
        case BACK:   o.e[r][c] = BOTTOM; o.e[r + 1][c] = EMPTY; break;
        case BOTTOM: o.e[r][c] = FRONT;  o.e[r + 1][c] = EMPTY; break;
    }
    return o;
}

// Moves the empty space rightwards
board right(const board& b) {
    const auto [r, c] = find_space(b);
    if (c == 2) return b;

    board o = b;
    switch (o.e[r][c + 1]) {
        case TOP:    o.e[r][c] = LEFT;   o.e[r][c + 1] = EMPTY; break;
        case RIGHT:  o.e[r][c] = TOP;    o.e[r][c + 1] = EMPTY; break;
        case LEFT:   o.e[r][c] = BOTTOM; o.e[r][c + 1] = EMPTY; break;
        case FRONT:  o.e[r][c] = FRONT;  o.e[r][c + 1] = EMPTY; break;
        case BACK:   o.e[r][c] = BACK;   o.e[r][c + 1] = EMPTY; break;
        case BOTTOM: o.e[r][c] = RIGHT;  o.e[r][c + 1] = EMPTY; break;
    }
    return o;
}

// Moves the empty space leftwards
board left(const board& b) {
    const auto [r, c] = find_space(b);
    if (c == 0) return b;

    board o = b;
    switch (o.e[r][c - 1]) {
        case TOP:    o.e[r][c] = RIGHT;  o.e[r][c - 1] = EMPTY; break;
        case RIGHT:  o.e[r][c] = BOTTOM; o.e[r][c - 1] = EMPTY; break;
        case LEFT:   o.e[r][c] = TOP;    o.e[r][c - 1] = EMPTY; break;
        case FRONT:  o.e[r][c] = FRONT;  o.e[r][c - 1] = EMPTY; break;
        case BACK:   o.e[r][c] = BACK;   o.e[r][c - 1] = EMPTY; break;
        case BOTTOM: o.e[r][c] = LEFT;   o.e[r][c - 1] = EMPTY; break;
    }
    return o;
}

// Function to find whether two boards are the same
bool board_same(const board& a, const board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (a.e[r][c] != b.e[r][c]) return false;
        }
    }
    return true;
}

// Assigns a unique integer to a given board configuration
int ord(const board& b) {
    int a = 0, p = 0;
    for (size_t r = 0; r < 3; r++) {
        for (size_t c = 0; c < 3; c++) {
            a += (b.e[r][c]) * pow(10, p);
            p++;
        }
    }
    return a;
}

enum move { L = 1, R = 2, U = 3, D = 4 };

// Returns the shortest path from src to dest
vector<int> solve(const board& src, const board& dest) {
    queue<board, 123717200> q;
    int visited[1000000000] = {0};
    board parent[1000000000];

    enqueue(q, src);
    visited[ord(src)] = L; // Initialized arbitrarily

    while (!is_queue_empty(q)) {
        board u = dequeue(q);
        if (board_same(u, dest)) {
            vector<int> moves;
            board c = u;
            int o = ord(c);

            while (!board_same(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = ord(c);
            }

            reverse(moves.begin(), moves.end());
            return moves;
        }

        board a = up(u), b = down(u), c = right(u), d = left(u);
        int aord = ord(a), bord = ord(b), cord = ord(c), dord = ord(d);

        if (!visited[aord]) {
            visited[aord] = U;
            parent[aord] = u;
            enqueue(q, a);
        }

        if (!visited[bord]) {
            visited[bord] = D;
            parent[bord] = u;
            enqueue(q, b);
        }

        if (!visited[cord]) {
            visited[cord] = R;
            parent[cord] = u;
            enqueue(q, c);
        }

        if (!visited[dord]) {
            visited[dord] = L;
            parent[dord] = u;
            enqueue(q, d);
        }
    }
    assert(0);
}

void print_moves(const vector<int>& moves) {
    for (auto m : moves) {
        switch (m) {
            case L: printf("L "); break;
            case R: printf("R "); break;
            case U: printf("U "); break;
            case D: printf("D "); break;
        }
    }
    printf("\n");
}

int main() {
    board ideal_input = {{{TOP, TOP, TOP}, {TOP, EMPTY, TOP}, {TOP, TOP, TOP}}};
    board solved = {{{BOTTOM, BOTTOM, BOTTOM}, {BOTTOM, EMPTY, BOTTOM}, {BOTTOM, BOTTOM, BOTTOM}}};

    printf("Board position given as input is:\n\n");
    print_board(ideal_input);

    auto moves = solve(ideal_input, solved);
    printf("Moves to solve the puzzle given as input are as follows:\n");
    print_moves(moves);
    printf("where moves represent the movement of the empty space.\n");

    return 0;
}
