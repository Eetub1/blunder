#include <stdbool.h>
#include <stdlib.h>
#include "board.h"
#include "utils.h"
#include "types.h"

const int KNIGHT_OFFSETS[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
const int BISHOP_OFFSETS[4] = {-9, -7, 7, 9};
const int ROOK_OFFSETS[4] = {-8, -1, 1, 8};
const int QUEEN_OFFSETS[8] = {-9, -8, -7, -1, 1, 7, 8, 9};

/**
 * @brief Generates all the pseudolegal moves for a piece, meaning checks are not checked
 * @param state Keeps track of board state
 * @param from From what square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @return int Number of legal moves
 */
int generate_legal_moves(BoardState *state, char *from, int *legal_moves) {
    int position = algebraic_to_index(from);
    Piece piece = state->board[position];

    switch (piece) {
        case WP:
        case BP:
            return 0;
        case WN:
        case BN:
            return generate_knight_moves(state, position, KNIGHT_OFFSETS, legal_moves);
        case WB:
        case BB:
            return generate_sliding_moves(state, position, BISHOP_OFFSETS, legal_moves);
        case WR:
        case BR:
            return generate_sliding_moves(state, position, ROOK_OFFSETS, legal_moves);
        case WQ:
        case BQ:
            return generate_sliding_moves(state, position, QUEEN_OFFSETS, legal_moves);
        case WK:
        case BK:
            return 0;
        default: // square is empty
            return 0;
    }
}


/**
 * @brief 
 * @param state 
 * @param position 
 * @param offsets 
 * @param legal_moves 
 * @return int 
 */
int generate_sliding_moves(BoardState *state, int position, int *offsets, int *legal_moves) {

}


/**
 * @brief Generates all pseudo-legal moves for a knight in a given position. 
 * @param state Keeps track of board state
 * @param position Index in the range 0-63
 * @param offsets Where can the knight jump
 * @param legal_moves Array where the legal moves are put
 * @return int How many legal moves there are for this piece 
 */
int generate_knight_moves(BoardState *state, int position, int *offsets, int *legal_moves) {
    int count = 0;
    
    for (int i = 0; i < 8; i++) { // a knight always has 8 offsets
        int target_square = position + offsets[i];
        if (abs((position % 8) - (target_square % 8)) > 2) continue;

        SquareContentType target_square_state = square_state(state->board, position, target_square);
        if (target_square_state != FRIEND_SQUARE) {
            legal_moves[count++] = target_square;
        }
    }
    return count;
}
