#include <stdio.h>
#include "types.h"
#include "utils.h"
#include "board.h"


int main(void) {
    printf("Program starting.\n");

    Piece start[64] = {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR
    };
    
    char *from = "d1";
    char *to = "d5";

    Undo undo;

    print_board(start, 64);

    make_move(start, from, to, &undo);
    print_board(start, 64);

    unmake_move(start, &undo);
    print_board(start, 64);

    return 0;
}