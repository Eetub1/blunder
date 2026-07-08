#include "types.h"
#include "utils.h"

/**
 * @brief Moves a piece from a square to another. Doesn't take move legality into consideration
 * 
 * @param from_row 
 * @param from_col 
 * @param to_row 
 * @param to_col 
 */
void make_move(Piece *board, char* from, char* to) {
    int from_square = algebraic_to_index(from);
    int to_square = algebraic_to_index(to);
    
    Piece temp = board[from_square];
    board[to_square] = temp;
    board[from_square] = EMPTY;
}