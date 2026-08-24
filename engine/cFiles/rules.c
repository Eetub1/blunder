#include <stdbool.h>
#include <stdlib.h>

#include "board.h"
#include "moves.h"
#include "types.h"
#include "utils.h"


/**
 * @brief Finds the first occurrence of a specified piece on the gameboard
 *        This function is mostly used to find a king's location
 * @param state Keeps track of board state
 * @param piece What kind of piece are we looking for
 * @return int  In which square is the piece located. Returns -1 if not found
 */
int find_piece(BoardState *state, Piece piece) {

    for (int i = 0; i < 64; i++) {
        if (state->board[i] == piece) return i;
    }
    return -1;
}


/**
 * @brief Finds out if the given square is attacked by any piece of the given color
 *        Used for checking if a given sides king is in check
 * @param state    Keeps track of board state
 * @param square   What square on the board are we inspecting
 * @param by_white Which side are we examining
 * @return bool    false if not attacked by any piece, else true
 */
bool is_square_attacked(BoardState *state, int square, bool by_white) {
    int moves[256];

    for (int i = 0; i < 64; i++) {
        Piece piece = state->board[i];

        if (piece == EMPTY) continue;
        if (by_white && piece >= BP && piece <= BK) continue;   // skip black pieces when looking for white attackers
        if (!by_white && piece >= WP && piece <= WK) continue;  // skip white pieces when looking for black attackers

        // pawn attacks need to be handled separately
        if (piece == WP || piece == BP) {
            int attack_offsets[2];
            if (by_white) { attack_offsets[0] = -9; attack_offsets[1] = -7; }
            else          { attack_offsets[0] =  7; attack_offsets[1] =  9; }

            for (int k = 0; k < 2; k++) {
                int target = i + attack_offsets[k];
                if (target < 0 || target >= 64) continue;
                if (abs((i % 8) - (target % 8)) != 1) continue;
                if (target == square) return true;
            }
        } else { // generate the piece's all pseudolegal moves to see if the target square is in there
            char from[3];
            index_to_algebraic(i, from);
            int move_count = generate_moves(state, from, moves);
            for (int m = 0; m < move_count; m++) {
                if (moves[m] == square) return true;
            }
        }
    }

    return false;
}


/**
 * @brief Checks if the current side whose turn is, is in check
 * @param state Keeps track of board state
 * @return bool false if not in check, else true
 */
bool is_in_check(BoardState *state) {
    Piece target = state->whose_turn == WHITE ? WK : BK;
    bool by_white = !state->whose_turn == WHITE;
    
    int king_location = find_piece(state, target);
    return is_square_attacked(state, king_location, by_white);
}


/**
 * @brief This function generates all legal moves for a given piece
 *        generate_moves is called and all the positions where your own king is in check
 *        are filtered out
 * @param state       Keeps track of board state
 * @param from        From what square is the piece moving
 * @param legal_moves Array where the legal moves are put
 * @param undo        Keeps track of previously made moves so that they can be reversed
 * @return int Number of legal moves
 */
int generate_legal_moves(BoardState *state, char *from, int *legal_moves, Undo *undo) {
    int from_square = algebraic_to_index(from);
    Piece piece = state->board[from_square];

    if (piece == EMPTY) return 0;

    bool is_white = piece >= WP && piece <= WK;
    bool is_whites_turn = state->whose_turn == WHITE;

    if (is_whites_turn != is_white) return 0;

    int move_count = generate_moves(state, from, legal_moves);

    int legal_count = 0;
    for (int i = 0; i < move_count; i++) {
        int to_square = legal_moves[i];

        char to_str[3];
        index_to_algebraic(to_square, to_str);
        make_move(state, from, to_str, undo);

        int king_square = find_piece(state, is_white ? WK : BK);
        bool is_king_in_check = is_square_attacked(state, king_square, !is_white);

        unmake_move(state, undo);

        if (!is_king_in_check) {
            legal_moves[legal_count++] = to_square;
        }
    }

    // Still need to filter out castling out of check and through check
    // Castling into check is already handled
    if (piece == WK || piece == BK) {
        int from_col = from_square % 8;
        int king_row = from_square / 8;

        int filtered_count = 0;
        for (int i = 0; i < legal_count; i++) {
            int to_square = legal_moves[i];
            int to_col = to_square % 8;
            int col_diff = abs(from_col - to_col);

            if (col_diff == 2) { // this is a castling move
                // can't castle out of check
                if (is_in_check(state)) continue;

                int transit_col = (to_col > from_col) ? from_col + 1 : from_col - 1;
                int transit_square = king_row * 8 + transit_col;

                // can't castle through an attacked square
                if (is_square_attacked(state, transit_square, !is_white)) continue;
            }
            legal_moves[filtered_count++] = to_square;
        }
        legal_count = filtered_count;
    }

    return legal_count;
}