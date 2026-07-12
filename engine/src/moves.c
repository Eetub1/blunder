#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "utils.h"
#include "types.h"

const int KNIGHT_OFFSETS[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
const int BISHOP_OFFSETS[4] = {-9, -7, 7, 9};
const int ROOK_OFFSETS[4] = {-8, -1, 1, 8};
const int KING_AND_QUEEN_OFFSETS[8] = {-9, -8, -7, -1, 1, 7, 8, 9};


bool is_white_piece(Piece p) {
    return p >= WP && p <= WK;
}


/**
 * @brief Generates moves for pieces that "slide" in a direction until they hit something
 *        i.e. rook, bishop or queen.
 * @param state          Keeps track of board state
 * @param position       From which square is the piece moving
 * @param offsets        Where can the piece possibly go
 * @param offsets_length Length of the offsets array
 * @param legal_moves    Pointer to an array filled with possible moves
 * @return int           The count of possible moves
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
 * @brief Generates all pseudo-legal moves for a stepping piece (king and knight) in a given position. 
 * @param state          Keeps track of board state
 * @param position       From which square is the piece moving
 * @param offsets        Where can the knight jump
 * @param offsets_length Length of the offsets array
 * @param legal_moves    Array where the legal moves are put
 * @return int           How many legal moves there are for this piece 
 */
int generate_stepping_moves(BoardState *state, int position, const int *offsets, int offsets_length, int *legal_moves) {
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
 * @brief Generates all pseudo-legal pawn moves
 * @param state       Keeps track of board state
 * @param position    From which square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @return int        How many legal moves there are for this piece 
 */
int generate_pawn_moves(BoardState *state, int position, int *legal_moves) {
    int count = 0;
    Piece pawn = state->board[position];
    bool is_white = is_white_piece(pawn);

    int forward = is_white ? -8 : 8; // which direction is forward depends on piece color
    int start_row = is_white ? 6 : 1; // in which row does the pawn start the game
    int row = position / 8;
    int col = position % 8;

    // can pawn go one square forward
    int one_ahead = position + forward;
    if (one_ahead >= 0 && one_ahead < 64) {
        if (square_state(state->board, position, one_ahead) == EMPTY_SQUARE) {
            legal_moves[count++] = one_ahead;

            // can pawn go two moves forward
            if (row == start_row) {
                int two_ahead = position + 2 * forward;
                if (square_state(state->board, position, two_ahead) == EMPTY_SQUARE) {
                    legal_moves[count++] = two_ahead;
                }
            }
        }
    }

    // can pawn capture anything
    int capture_offsets[2] = { forward - 1, forward + 1};
    for (int i = 0; i < 2; i++) {
        int target = position + capture_offsets[i];

        if (target < 0 || target >= 64) continue;
        if (abs((position % 8) - (target % 8)) != 1) continue;

        if (square_state(state->board, position, target) == ENEMY_SQUARE) {
            legal_moves[count++] = target;
        }

    }

    // is en passant possible
    if (state->en_passant_target_square >= 0) {
        int ep = state->en_passant_target_square;
        for (int i = 0; i < 2; i++) {
            int target = position + capture_offsets[i];

            if (target < 0 || target >= 64) continue;
            if (abs((position % 8) - (target % 8)) != 1) continue;

            if (target == ep) {
                legal_moves[count++] = ep;
            }
        }
    }

    return count;
}


/**
 * @brief Generates all pseudo-legal king moves
 * @param state       Keeps track of board state
 * @param position    From which square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @return int        How many legal moves there are for this piece 
 */
int generate_king_moves(BoardState *state, int position, const int *offsets, int offsets_length,  int *legal_moves) {
    int count = generate_stepping_moves(state, position, offsets, offsets_length, legal_moves);

    // need to check if castling is possible
    Piece king = state->board[position];
    bool is_white = is_white_piece(king);
    char *rights = state->castling_rights;

    // strchr checks if the castling rights string, which is of the form "KQkq", contains a char
    // also need to check that the squares between the rook and the king are not occupied
    if (is_white) {
        if (strchr(rights, 'K') &&
            state->board[algebraic_to_index("f1")] == EMPTY &&
            state->board[algebraic_to_index("g1")] == EMPTY) {
            legal_moves[count++] = algebraic_to_index("g1");
        }
        if (strchr(rights, 'Q') &&
            state->board[algebraic_to_index("b1")] == EMPTY &&
            state->board[algebraic_to_index("c1")] == EMPTY &&
            state->board[algebraic_to_index("d1")] == EMPTY) {
            legal_moves[count++] = algebraic_to_index("c1");
        }
    } else {
        if (strchr(rights, 'k') &&
            state->board[algebraic_to_index("f8")] == EMPTY &&
            state->board[algebraic_to_index("g8")] == EMPTY) {
            legal_moves[count++] = algebraic_to_index("g8");
        }
        if (strchr(rights, 'q') &&
            state->board[algebraic_to_index("b8")] == EMPTY &&
            state->board[algebraic_to_index("c8")] == EMPTY &&
            state->board[algebraic_to_index("d8")] == EMPTY) {
            legal_moves[count++] = algebraic_to_index("c8");
        }
    }

    return count;
}


/**
 * @brief Generates all the pseudolegal moves for a piece, meaning checks are not checked
 * @param state       Keeps track of board state
 * @param from        From what square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @return int        Number of legal moves
 */
int generate_moves(BoardState *state, char *from, int *legal_moves) {
    int position = algebraic_to_index(from);
    Piece piece = state->board[position];

    switch (piece) {
        case WP:
        case BP:
            return generate_pawn_moves(state, position, legal_moves);
        case WN:
        case BN:
            return generate_stepping_moves(state, position, KNIGHT_OFFSETS, 8, legal_moves);
        case WB:
        case BB:
            return generate_sliding_moves(state, position, BISHOP_OFFSETS, 4, legal_moves);
        case WR:
        case BR:
            return generate_sliding_moves(state, position, ROOK_OFFSETS, 4, legal_moves);
        case WQ:
        case BQ:
            return generate_sliding_moves(state, position, KING_AND_QUEEN_OFFSETS, 8, legal_moves);
        case WK:
        case BK:
            return generate_king_moves(state, position, KING_AND_QUEEN_OFFSETS, 8, legal_moves);
        default: // square is empty
            return 0;
    }
}