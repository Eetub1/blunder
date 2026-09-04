#pragma once

#include "types.hpp"

class Move {
public:
    Move(
        int from = -1, 
        int to = -1, 
        PieceType movedPieceType = PieceType::EMPTY,
        int enPassantSquare = -1,
        PieceType capturedPieceType = PieceType::EMPTY,
        PieceType promotionPiece = PieceType::EMPTY,
        CastleType castleType = CastleType::NONE,
        MoveType moveType = MoveType::NORMAL
    ) : from(from),
        to(to),
        movedPieceType(movedPieceType),
        enPassantSquare(enPassantSquare),
        capturedPieceType(capturedPieceType),
        promotionPiece(promotionPiece),
        castleType(castleType),
        moveType(moveType)
    {}

    ~Move() = default;

    int getFrom() const { return from; }
    int getTo() const { return to; }
    PieceType getMovedPieceType() const { return movedPieceType; }
    Color getMovedPieceColor() const 
    {
        PieceType type = this->getMovedPieceType();
        if (type >= WP && type <= WK) return Color::WHITE;
        return Color::BLACK;
    }
    int getEnPassantSquare() const { return enPassantSquare; }
    PieceType getCapturedPieceType() const { return capturedPieceType; }
    PieceType getPromotionPiece() const { return promotionPiece; }
    CastleType getCastleType() const { return castleType; }
    MoveType getMoveType() const { return moveType; }

    void setCapturedPieceType(PieceType type) {this->capturedPieceType = type;}

private:
    int from;
    int to;    
    PieceType movedPieceType;
    int enPassantSquare;
    PieceType capturedPieceType;
    PieceType promotionPiece;
    CastleType castleType;
    MoveType moveType;
};