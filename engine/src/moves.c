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
 * @brief Generates moves for pieces that "slide" in a direction until they hit something
 *        i.e. rook, bishop or queen.
 * @param state 
 * @param position 
 * @param offsets 
 * @param offsets_length Length of the offsets array
 * @param legal_moves 
 * @return int 
 */
int generate_sliding_moves(BoardState *state, int position, const int *offsets, int offsets_length, int *legal_moves) {
    int count = 0;

    for (int i = 0; i < offsets_length; i++) {
        int offset = offsets[i];
        int target_square = position + offset;

        int prev_square = position;
        while (true) {
            if (target_square < 0 || target_square > 63) break;
            if (abs((prev_square % 8) - (target_square % 8)) > 1) break; // if the amount moved to left or right is greater than 1

            SquareContentType square =  square_state(state->board, position, target_square);
            if (square == EMPTY_SQUARE || square == ENEMY_SQUARE) legal_moves[count++] = target_square;
            if (square == FRIEND_SQUARE || square == ENEMY_SQUARE) break;
            
            prev_square = target_square;
            target_square += offset; // keep going in the direction
        }
    }
    return count;
}


/**
 * @brief Generates all pseudo-legal moves for a knight in a given position. 
 * @param state          Keeps track of board state
 * @param position       Index in the range 0-63
 * @param offsets        Where can the knight jump
 * @param offsets_length Length of the offsets array
 * @param legal_moves    Array where the legal moves are put
 * @return int How many legal moves there are for this piece 
 */
int generate_knight_moves(BoardState *state, int position, const int *offsets, int offsets_length, int *legal_moves) {
    int count = 0;
    
    for (int i = 0; i < offsets_length; i++) {
        int target_square = position + offsets[i];
        if (target_square < 0 || target_square > 63) continue;
        if (abs((position % 8) - (target_square % 8)) > 2) continue; // make sure no phantom moves happen

        SquareContentType target_square_state = square_state(state->board, position, target_square);
        if (target_square_state != FRIEND_SQUARE) {
            legal_moves[count++] = target_square;
        }
    }
    return count;
}


/**
 * @brief Generates all the pseudolegal moves for a piece, meaning checks are not checked
 * @param state       Keeps track of board state
 * @param from        From what square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @return int Number of legal moves
 */
int generate_moves(BoardState *state, char *from, int *legal_moves) {
    int position = algebraic_to_index(from);
    Piece piece = state->board[position];

    switch (piece) {
        case WP:
        case BP:
            return 0;
        case WN:
        case BN:
            return generate_knight_moves(state, position, KNIGHT_OFFSETS, 8, legal_moves);
        case WB:
        case BB:
            return generate_sliding_moves(state, position, BISHOP_OFFSETS, 4, legal_moves);
        case WR:
        case BR:
            return generate_sliding_moves(state, position, ROOK_OFFSETS, 4, legal_moves);
        case WQ:
        case BQ:
            return generate_sliding_moves(state, position, QUEEN_OFFSETS, 8, legal_moves);
        case WK:
        case BK:
            return 0;
        default: // square is empty
            return 0;
    }
}