#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <tuple>

#include "queue.hpp"

// Structure to represent the 3x3 puzzle board
struct board {
    char e[3][3];
};

// Function to print the current state of the board
void print_board(const board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            printf("%3d", b.e[r][c]);
        }
        printf("\n");
    }
}

// Function to read the board state from input
void read_board(board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            scanf("%c", &b.e[r][c]);
        }
    }
}

// Function to find the position of the empty space on the board
std::tuple<int, int> find_space(const board& b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (b.e[r][c] == '.') return { r, c };
    assert(0);  // Should never reach here if the board is valid
}

// Constexpr function to calculate factorial
constexpr int fact(int i) {
    int p = 1;
    for (int j = 2; j <= i; ++j) {
        p *= j;
    }
    return p;
}

// Function to calculate the ordinal value of a board state
int ord(const board& board) {
    int seen[10] = { 0 };
    int a = 0;
    int k = 8;

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int v = board.e[r][c];
            int o = 0;
            for (int i = 1; i < v; ++i) {
                if (!seen[i]) { ++o; }
            }
            a += o * fact(k);
            --k;
            seen[v] = 1;
        }
    }

    return a;
}

// Helper function to swap two integers
void swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}

// Functions to move the empty space in different directions
board up(const board& b) {
    const auto [r, c] = find_space(b);
    if (r == 0) return b;
    board o = b;
    swap(o.e[r-1][c], o.e[r][c]);
    return o;
}

board down(const board& b) {
    const auto [r, c] = find_space(b);
    if (r == 2) return b;
    board o = b;
    swap(o.e[r+1][c], o.e[r][c]);
    return o;
}

board left(const board& b) {
    const auto [r, c] = find_space(b);
    if (c == 0) return b;
    board o = b;
    swap(o.e[r][c-1], o.e[r][c]);
    return o;
}

board right(const board& b) {
    const auto [r, c] = find_space(b);
    if (c == 2) return b;
    board o = b;
    swap(o.e[r][c+1], o.e[r][c]);
    return o;
}

// Function to check if two boards are in the same state
bool is_same(const board& a, const board &b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (a.e[r][c] != b.e[r][c]) return false;
    return true;
}

// Enum to represent possible moves
enum move { L = 1, R = 2, U = 3, D = 4 };

// Function to solve the 8-puzzle problem using BFS
std::vector<int> solve(const board& src, const board& dest) {
    queue<board, fact(9)> q;
    int visited[fact(9)] = {0};
    board parent[fact(9)];

    enqueue(q, src);
    visited[ord(src)] = L;  // Use L as a sentinel value

    while (!is_empty(q)) {
        board u = dequeue(q);
        if (is_same(u, dest)) {
            // Reconstruct the path from source to destination
            std::vector<int> moves;
            board c = u;
            int o = ord(c);
            while (!is_same(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = ord(c);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
        }

        // Generate all possible next states
        board next_states[] = {up(u), down(u), left(u), right(u)};
        move moves[] = {U, D, L, R};

        for (int i = 0; i < 4; ++i) {
            int next_ord = ord(next_states[i]);
            if (!visited[next_ord]) {
                visited[next_ord] = moves[i];
                parent[next_ord] = u;
                enqueue(q, next_states[i]);
            }
        }
    }
    assert(0);  // Should never reach here if a solution exists
}

// Function to print the sequence of moves
void print_moves(const std::vector<int>& moves) {
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
    board src, dest;

    read_board(src);
    read_board(dest);

    auto moves = solve(src, dest);
    print_moves(moves);

    return 0;
}
