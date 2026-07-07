#include <stdio.h>
#include "types.h"
#include "utils.h"

const char STARTING_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(void) {
    printf("Program starting.\n");

    Piece start_position[64] = {
        BR, BN, BB, BQ, BK, BB, BN, BR,   // rank 8 (a8..h8) — black back rank
        BP, BP, BP, BP, BP, BP, BP, BP,   // rank 7 — black pawns
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,   // rank 6
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,   // rank 5
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,   // rank 4
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,   // rank 3
        WP, WP, WP, WP, WP, WP, WP, WP,   // rank 2 — white pawns
        WR, WN, WB, WQ, WK, WB, WN, WR    // rank 1 (a1..h1) — white back rank
    };

    print_board(start_position, 64);

    return 0;
}