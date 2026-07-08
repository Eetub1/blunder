#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "utils.h"
#include "board.h"


int main(void) {
    BoardState state = {
        .board = {
            BR, BN, BB, BQ, BK, BB, BN, BR,
            BP, BP, BP, BP, BP, BP, BP, BP,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            WP, WP, WP, WP, WP, WP, WP, WP,
            WR, WN, WB, WQ, WK, WB, WN, WR
        },
        .whose_turn = WHITE
    };

    Undo undo;

    print_board(state.board, 64);

    while (true) {
        char from[3];
        char to[3];
        
        // asking the move. No validation as of yet
        scanf("%2s %2s", from, to);
        if (!strcmp(from, "qq")) break;

        if (!strcmp(from, "un")) {
            unmake_move(&state, &undo);
        } else {
            make_move(&state, from, to, &undo);
        }
        print_board(state.board, 64);
    }
    printf("Game quit\n");

    return 0;
}