#include <stdbool.h>

#include "board.h"
#include "moves.h"
#include "types.h"


/**
 * @brief Finds the first occurrence of a specified piece on the gameboard
 *        This function is mostly used to find a king's location
 * @param state Keeps track of board state
 * @param piece What kind of piece are we looking for
 * @return int  In which square is the piece located
 */
int find_piece(BoardState *state, Piece piece) {

    for (int i = 0; i < 64; i++) {
        if (state->board[i] == piece) return i;
    }
}


/**
 * @brief Finds out if the given square is attacked by any piece of the given color
 *        Used for checking if a given sides king is in check
 * @param state    Keeps track of board state
 * @param square   What square on the board are we inspecting
 * @param by_white Which side are we examining
 * @return bool    false if not attacked by any piece, else true
 */
bool is_square_attacked(BoardState *state, char *square, bool by_white) {
    for (int i = 0; i < 64; i++) {
        Piece piece = state->board[i];

        // go through every piece on the board
        // if square empty of the color is wrong: continue
        // check if the piece can attack the specified square
        // if can, return immediately true
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
    bool by_white = state->whose_turn == WHITE ? false : true; 
    
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
 * @return int Number of legal moves
 */
int generate_legal_moves(BoardState *state, char *from, int *legal_moves) {
    return generate_moves(state, from, legal_moves);

    // TODO legal_moves needs to be filtered down
}