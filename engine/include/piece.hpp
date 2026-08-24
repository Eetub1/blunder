#pragma once

typedef enum {
    EMPTY,                  // 0
    WP, WN, WB, WR, WQ, WK, // 1-6
    BP, BN, BB, BR, BQ, BK  // 7-12
} PieceType;

typedef enum {
    WHITE,
    BLACK
} Color;


class Piece {

private:
    PieceType type;
    Color color;
public:
    PieceType getType() {return this->type;}
};