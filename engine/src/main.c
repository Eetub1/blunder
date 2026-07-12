#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "utils.h"
#include "board.h"
#include "moves.h"
#include "rules.h"


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
            int legal_moves[256];
            int move_count = generate_legal_moves(&state, from, legal_moves, &undo);

            // print all valid moves
            printf("Valid squares to move to:\n");
            for (int i = 0; i < move_count; i++) {
                char index_storage[3];
                index_to_algebraic(legal_moves[i], index_storage);
                printf("%s ", index_storage);
            }
            printf("\n\n");

            bool valid = false;
            for (int i = 0; i < move_count; i++) {
                if (legal_moves[i] == algebraic_to_index(to)) {
                    printf("The move you chose was valid!\n");
                    make_move(&state, from, to, &undo);
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                printf("The move you chose was not valid!\n");
            }
            
        }
        print_board(state.board, 64);
    }
    printf("Game quit\n");

    return 0;
}