#include <stdbool.h>
#include "board.h"
#include "utils.h"
#include "types.h"

const int KNIGHT_OFFSETS[8] = {-17, -15, -10, -6, 6, 10, 15, 17};

/**
 * @brief Generates all the pseudolegal moves for a piece, meaning checks are not checked
 * @param state Board's state
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
            break;
        case WN:
        case BN:
            return generate_knight_moves(state, position, &KNIGHT_OFFSETS);
            break;
        case WB:
        case BB:
            return 0;
            break;
        case WR:
        case BR:
            return 0;
            break;
        case WQ:
        case BQ:
            return 0;
            break;
        case WK:
        case BK:
            return 0;
            break;
        default:
            return 0;
    }
}


/**
 * @brief 
 * 
 * @param state 
 * @param position 
 * @param offsets 
 * @return int 
 */
int generate_knight_moves(BoardState *state, int position, int *offsets) {

}
