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
    WHITESHORT, WHITELONG, BLACKSHORT, BLACKLONG
} CastleType;

typedef enum 
{
    NORMAL, CASTLE, PROMOTION, ENPASSANT
} MoveType;

typedef struct 
{
    int enPassantSquare;
    int from;
    int to;
    PieceType capturedPieceType;
    CastleType castleType;
    MoveType moveType = MoveType::NORMAL;
    // Something about promotion also?!
} Move;