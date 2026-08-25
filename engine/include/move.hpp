#pragma once

#include "types.hpp"

class Move {
public:
    Move(
        int from, 
        int to, 
        PieceType movedPiece,
        int enPassantSquare = -1,
        PieceType capturedPieceType = PieceType::EMPTY,
        PieceType promotionPiece = PieceType::EMPTY,
        CastleType castleType = CastleType::NONE,
        MoveType moveType = MoveType::NORMAL
    ) : from(from),
        to(to),
        movedPiece(movedPiece),
        enPassantSquare(enPassantSquare),
        capturedPieceType(capturedPieceType),
        promotionPiece(promotionPiece),
        castleType(castleType),
        moveType(moveType) 
    {}

    ~Move() = default;

    int getFrom() const { return from; }
    int getTo() const { return to; }
    PieceType getMovedPiece() const { return movedPiece; }
    int getEnPassantSquare() const { return enPassantSquare; }
    PieceType getCapturedPieceType() const { return capturedPieceType; }
    PieceType getPromotionPiece() const { return promotionPiece; }
    CastleType getCastleType() const { return castleType; }
    MoveType getMoveType() const { return moveType; }

    void setCapturedPieceType(PieceType type) {this->capturedPieceType = type;}

private:
    int from;
    int to;    
    PieceType movedPiece;
    int enPassantSquare;
    PieceType capturedPieceType;
    PieceType promotionPiece;
    CastleType castleType;
    MoveType moveType;
};