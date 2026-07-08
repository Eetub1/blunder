#include "types.h"
#include "utils.h"
#include "board.h"

/**
 * @brief Moves a piece from a square to another. Doesn't take move legality into consideration
 * @param board The board to modify
 * @param from  Move origin square 
 * @param to    Move target square
 * @param undo  Struct to hold information about the move so that it can be reversed if necessary
 * @return Piece what was on the square that was overwritten 
 */
void make_move(Piece *board, char* from, char* to, Undo *undo) {
    int from_square = algebraic_to_index(from);
    int to_square = algebraic_to_index(to);
    
    undo->captured = board[to_square];
    undo->from_square = from_square;
    undo->to_square = to_square;

    Piece temp = board[from_square];
    board[to_square] = temp;
    board[from_square] = EMPTY;
}


/**
 * @brief Unmakes the previous made move
 * TODO NOT EVEN CLOSE TO BEING FINISHED
 * @param board The board to modify
 * @param undo  Struct containing info about previous move
 */
void unmake_move(Piece *board, Undo *undo) {
    int from = undo->from_square;
    int to = undo->to_square;

    board[from] = board[to]; // move piece back
    board[to] = undo->captured; // move overwritten piece to its original square
}