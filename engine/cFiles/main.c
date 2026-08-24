#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "utils.h"
#include "board.h"
#include "moves.h"
#include "rules.h"


BoardState setup_default_board_state() {
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

    return state;
}


void play_game_with_input() {
    BoardState state = setup_default_board_state();
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
}


void play_game_from_array(char **game, int game_length) {
    BoardState state = setup_default_board_state();
    Undo undo;

    for (int i = 0; i < game_length; i++) {
        char *move = game[i];

        char from[3];
        from[0] = move[0];
        from[1] = move[1];
        from[2] = '\0';

        char to[3];
        to[0] = move[3];
        to[1] = move[4];
        to[2] = '\0';

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
                printf("VALID!\n");
                make_move(&state, from, to, &undo);
                valid = true;
                break;
            }
        }

        if (!valid) {
            printf("NOT VALID!\n");
        }
    }

}


int main(void) {

    char *game[] = {
        "e2 e4", "e7 e5",
        "g1 f3", "b8 c6",
        "f1 c4", "f8 c5",
        "d2 d3", "g8 f6",
        "b1 c3", "d7 d6",
        "c1 g5", "c8 g4",
        "f3 d2", "c6 d4",
        "g5 f6", "g7 f6",
        "d2 f1", "d4 f3",
        "d1 f3", "g4 f3",
        "g2 f3", "d8 d7",
        "c3 d5", "e8 d8",
        "d5 f6", "d7 g4",
        "f6 g4", "h7 h5",
        "g4 f6", "d8 e7",
        "f6 h5", "h8 h5",
        "f3 g4", "h5 h4"
    };
    int game_length = sizeof(game) / sizeof(game[0]);


    //play_game_with_input();
    play_game_from_array(game, game_length);

    return 0;
}