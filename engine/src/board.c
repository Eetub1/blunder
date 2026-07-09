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
 *        TODO NOT EVEN CLOSE TO BEING FINISHED
 * @param board The board to modify
 * @param undo  Struct containing info about previous move
 */
void unmake_move(BoardState *state, Undo *undo) {
    int from = undo->from_square;
    int to = undo->to_square;

    state->board[from] = state->board[to]; // move piece back
    state->board[to] = undo->captured; // move overwritten piece to its original square
}


/**
 * @brief Returns what type of piece is on the target square relative to the current position
 * @param board    Game board
 * @param position From what square is a piece moving
 * @param target   To what square is a piece moving
 * @return SquareContentType Enemy, Friend or Empty
 */
SquareContentType square_state(Piece *board, int position, int target) {
    // Piece cant be empty, the function generate_legal_moves in moves.c checks this
    
    Piece cur_piece = board[position];
    Piece target_piece = board[target];

    if (!target_piece) return EMPTY_SQUARE;

    Color piece_color;
    if (cur_piece < 7) piece_color = WHITE;
    else piece_color = BLACK;

    Color target_color;
    if (target_piece < 7) target_color = WHITE;
    else target_color = BLACK;

    if (piece_color == target_color) return FRIEND_SQUARE;
    return ENEMY_SQUARE;
}