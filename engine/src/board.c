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
void make_move(BoardState *state, char *from, char *to, Undo *undo) {
    int from_square = algebraic_to_index(from);
    int to_square = algebraic_to_index(to);

    undo->captured = state->board[to_square];
    undo->from_square = from_square;
    undo->to_square = to_square;

    // Flip whose turn it is
    state->whose_turn = state->whose_turn == WHITE ? BLACK : WHITE;

    Piece temp = state->board[from_square];
    state->board[to_square] = temp;
    state->board[from_square] = EMPTY;
}


/**
 * @brief Unmakes the previous made move
 * TODO NOT EVEN CLOSE TO BEING FINISHED
 * @param board The board to modify
 * @param undo  Struct containing info about previous move
 */
void unmake_move(BoardState *state, Undo *undo) {
    int from = undo->from_square;
    int to = undo->to_square;

    state->board[from] = state->board[to]; // move piece back
    state->board[to] = undo->captured; // move overwritten piece to its original square
}