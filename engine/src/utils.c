#include "types.h"
#include <stddef.h>
#include <stdio.h>


/**
 * @brief Prints the board to the console
 * @param board 1d array representing the board
 * @param length of the array
 */
void print_board(Piece *board, int length) {
    for (int i = 0; i < length; i++) {
        printf("% 3d", board[i]);
        if (i % 8 == 7) {
            printf("\n");
        }
    }
}