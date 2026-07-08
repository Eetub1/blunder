#ifndef BOARD_H
#define BOARD_H

#include "types.h"

// Keeps state about previously made moves so that they can be reversed
typedef struct {
    Piece captured;
    int from_square;
    int to_square;
} Undo;

void make_move(Piece *board, char* from, char* to, Undo *undo);
void unmake_move(Piece *board, Undo *undo);

#endif