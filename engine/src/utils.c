#include "types.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Maps pieces to unicode chess glyphs. Empty square is a dot.
 */
char *piece_to_glyph(Piece p) {
    switch (p) {
        case BK: return "\u2654"; // ♔
        case BQ: return "\u2655"; // ♕
        case BR: return "\u2656"; // ♖
        case BB: return "\u2657"; // ♗
        case BN: return "\u2658"; // ♘
        case BP: return "\u2659"; // ♙
        case WK: return "\u265A"; // ♚
        case WQ: return "\u265B"; // ♛
        case WR: return "\u265C"; // ♜
        case WB: return "\u265D"; // ♝
        case WN: return "\u265E"; // ♞
        case WP: return "\u265F"; // ♟
        case EMPTY:
        default:  return ".";
    }
}


/**
 * @brief Prints the board to the console as an 8x8 grid.
 * @param board 1d array representing the board
 * @param length length of the array (64)
 */
void print_board(Piece *board, int length) {
    for (int i = 0; i < length; i++) {
        printf("%s ", piece_to_glyph(board[i]));
        if (i % 8 == 7) {
            printf("\n");
        }
    }
    printf("\n");
}


int algebraic_to_index(char* position) {
    char col = *position;
    char row = *(position + 1);
    return ('8' - row) * 8 + (col - 'a');
}