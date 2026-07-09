#ifndef BOARD_H
#define BOARD_H

#include "types.h"

// Keeps state about previously made moves so that they can be reversed
typedef struct {
    Piece captured;
    int from_square;
    int to_square;
} Undo;

typedef struct {
    Piece board[64];
    Color whose_turn;
    int en_passant_target_square;
    char castling_rights[4]; // "KQkq"
} BoardState;


void make_move(BoardState *state, char *from, char *to, Undo *undo);
void unmake_move(BoardState *state, Undo *undo);
SquareContentType square_state(Piece *board, int position, int target);

#endif