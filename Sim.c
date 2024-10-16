#include <stdio.h>
#include <assert.h>

// Enum to represent the state of each line in the game
enum {
    NO = 0,   // No line
    RED = 1,  // Red line
    BLUE = 2  // Blue line
};

typedef char board_t[15];
typedef char player_t;  // A player should be either RED or BLUE

// Function to print the current board state
void printArray(board_t board) {
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
            case RED: printf("R "); break;
            case BLUE: printf("B "); break;
            case NO: printf("0 "); break;
        }
    }
}

// Function to check if the board is full
int is_full(board_t L) {
    for (int i = 0; i < 15; i++) {
        if (L[i] == NO) {
            return 0;
        }
    }
    return 1;
}

// Structure to store the best move and its score
typedef struct {
    int line;
    int score;
} move_t;

// Function to convert 2D coordinates to 1D index
int index_(int i, int j) {
    if (j < i) {
        int temp = i;
        i = j;
        j = temp;
    }
    i++;
    j++;
    int a = 1, count = 5, ans = 0;
    while (a != i) {
        ans += count;
        count--;
        a++;
    }
    ans += j - i;
    return ans - 1;
}

// Function to check if a player has won the game
int has_won(board_t L, player_t player) {
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            for (int k = j + 1; k < 6; k++) {
                if (L[index_(i, j)] == L[index_(j, k)] && L[index_(j, k)] == L[index_(i, k)] && L[index_(i, k)] == 3 - player) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

#define MAX_ORD (43046720)  // Maximum number of board states
move_t memo[MAX_ORD] = {};  // Memoization array

// Function to generate a unique ordinal for a given board state
int ord(board_t board) {
    int p = 1, i = 0, d;

    for (int row = 0; row < 15; ++row) {
        switch (board[row]) {
            case RED: d = 1; break;
            case BLUE: d = 2; break;
            case NO: d = 0; break;
        }
        i += d * p;
        p *= 3;
    }
    return i;
}

// Function to determine the best move for the current player
move_t best_move(board_t L, player_t player, int iter) {
    int ord_val = ord(L);

    // Return memoized result if available
    if (memo[ord_val].line != -1) {
        return memo[ord_val];
    }

    int score = -100, Move_min = -1;
    move_t move;

    for (int i = 0; i < 15; i++) {
        if (L[i] == NO) {
            L[i] = player;

            if (is_full(L)) {
                L[i] = NO;
                move.line = i;
                move.score = -16;
                memo[ord_val] = move;
                return move;
            }

            move_t var = best_move(L, 3 - player, iter + 1);

            if (has_won(L, player)) {
                L[i] = NO;
                move.line = i;
                move.score = 0;
                memo[ord_val] = move;
                return move;
            }

            score = (var.score > score) ? var.score : score;
            if (var.score >= score) {
                Move_min = i;
            }

            L[i] = NO;
        }
    }

    move.line = Move_min;
    move.score = score + 1;
    memo[ord_val] = move;
    return move;
}

int main() {
    // Initialize memoization array
    for (int i = 0; i <= MAX_ORD; i++) {
        memo[i].line = -1;
    }

    board_t L = {NO};
    int count = 0;
    int player = 2;
    int color = 1;

    printf("Enter position and color: ");
    scanf("%d %d", &player, &color);

    while (count != 15) {
        if (player == 1) {
            int x, y;
            scanf("%d %d", &x, &y);

            if (color == 1) {
                L[index_(x - 1, y - 1)] = RED;
            } else {
                L[index_(x - 1, y - 1)] = BLUE;
            }

            printArray(L);
            printf("\n");

            if (has_won(L, player)) {
                printf("Computer won\n");
                break;
            }
        } else {
            move_t a = best_move(L, 2, 11);
            printf("%d\n", a.line);

            if (color == 1) {
                L[a.line] = BLUE;
            } else {
                L[a.line] = RED;
            }

            printArray(L);
            printf("\n");

            if (has_won(L, player)) {
                printf("I won\n");
                break;
            }
        }

        count++;
        player = 3 - player;
    }

    return 0;
}
