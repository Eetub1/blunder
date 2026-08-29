#pragma once

typedef enum 
{
    EMPTY, 
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
} PieceType;

typedef enum  
{
    WHITE,
    BLACK
} Color;

typedef enum 
{
    NONE, KINGSIDE, QUEENSIDE
} CastleType;

typedef enum 
{
    NORMAL, CASTLING, PROMOTION, ENPASSANT
} MoveType;

// What is in a square in relation to another square
typedef enum {
    ENEMY_SQUARE, FRIEND_SQUARE, EMPTY_SQUARE
} SquareContent;