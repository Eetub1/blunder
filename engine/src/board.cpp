#include <vector>
#include <cassert>

#include "board.hpp"
#include "boardPositions.hpp"
#include "piece.hpp"
#include "types.hpp"
#include "move.hpp"
#include "moveGenerator.hpp"
#include "utils.hpp"

Board::Board() 
    : grid(64) 
{}


Board::~Board() {}


void Board::setupDefaultBoard() 
{
    for (unsigned int i = 0; i < 64; i++) {
        Piece p;
        PieceType type = defaultBoard[i];
        p.setType(type);

        Color color = WP <= type && type <= WK ? Color::WHITE : Color::BLACK;
        p.setColor(color);
        this->grid[i] = p;
    }
}


void Board::setupCustomBoardFromFen(std::string fen) 
{
    std::cout << fen << std::endl;
}


void Board::removeCastlingRights(char type) 
{
    std::string& rights = this->getCastlingRights();

    assert(rights.size() > 2);

    size_t pos = rights.find(type);
    rights.erase(pos, 1);
}


void Board::addCastlingRights(char type)
{
    std::string& rights = this->getCastlingRights();

    assert(rights.size() < 4);

    rights += type;
}


void Board::makeCastlingMove(Move &move)
{
    Color color = move.getMovedPieceColor();

    Piece empty;
    empty.setType(PieceType::EMPTY);

    switch (move.getCastleType()) {
        case CastleType::KINGSIDE:
            if (color == Color::WHITE) {
                Piece rook = this->at(algebraicToIndex("h1"));

                this->setAt(algebraicToIndex("h1"), empty);
                this->setAt(algebraicToIndex("f1"), rook);

                this->setAt(move.getTo(), this->at(move.getFrom()));
                this->setAt(algebraicToIndex("e1"), empty);

                removeCastlingRights('K');
            } else {
                Piece rook = this->at(algebraicToIndex("h8"));

                this->setAt(algebraicToIndex("h8"), empty);
                this->setAt(algebraicToIndex("f8"), rook);

                this->setAt(move.getTo(), this->at(move.getFrom()));
                this->setAt(algebraicToIndex("e8"), empty);

                removeCastlingRights('k');
            }
            break;
        case CastleType::QUEENSIDE:
            if (color == Color::WHITE) {
                Piece rook = this->at(algebraicToIndex("a1"));

                this->setAt(algebraicToIndex("a1"), empty);
                this->setAt(algebraicToIndex("d1"), rook);

                this->setAt(move.getTo(), this->at(move.getFrom()));
                this->setAt(algebraicToIndex("e1"), empty);

                removeCastlingRights('Q');
            } else {
                Piece rook = this->at(algebraicToIndex("a8"));

                this->setAt(algebraicToIndex("a8"), empty);
                this->setAt(algebraicToIndex("d8"), rook);

                this->setAt(move.getTo(), this->at(move.getFrom()));
                this->setAt(algebraicToIndex("e8"), empty);

                removeCastlingRights('q');
            }
            break;
        default:
            break;
    }
}


void Board::makeNormalMove(Move &move)
{
    this->grid[move.getTo()] = this->grid[move.getFrom()];

    Piece empty;
    empty.setType(PieceType::EMPTY);
    this->grid[move.getFrom()] = empty;
}


void Board::makeEnPassantMove(Move &move)
{
    // Move capturing pawn to en passant targe square
    Piece pawn;
    pawn.setType(move.getMovedPieceType());
    this->setAt(move.getTo(), pawn);

    // Clear the previous spot where the pawn was
    Piece empty;
    empty.setType(PieceType::EMPTY);
    this->setAt(move.getFrom(), empty);

    // Also capture the pawn that just moved 2 squares
    Color movedPawnColor = move.getMovedPieceColor();
    int pawnToCaptureSquare = movedPawnColor == Color::WHITE ? move.getTo() + 8 : move.getTo() - 8;
    this->setAt(pawnToCaptureSquare, empty);
}


void Board::makeMove(Move &move) 
{
    switch(move.getMoveType()) {
        case MoveType::CASTLING:
            makeCastlingMove(move);
            break;
        case MoveType::PROMOTION:
            break;
        case MoveType::ENPASSANT:
            makeEnPassantMove(move);
            break;
        default:
            makeNormalMove(move);
            break;
    }

    // if the move was a pawn moving forward two squares, en passant is available
    PieceType piece = move.getMovedPieceType();
    int rowDiff = abs(move.getFrom() - move.getTo()) / 8;
    if ((piece == PieceType::WP || piece == PieceType::BP) && rowDiff == 2) {
        Color color = move.getMovedPieceColor();
        int diff = color == Color::WHITE ? 8 : -8;
        this->setEnPassantSquare(move.getTo() + diff);
    } else {
        // else we set en passant unavailable
        this->setEnPassantSquare(-1);
    }

    undoStack.push_back(move);
}


void Board::unmakeNormalMove(Move &move)
{
    Piece toSquarePiece = this->grid[move.getTo()];
    this->grid[move.getFrom()] = toSquarePiece;
    Piece capturedPiece;
    capturedPiece.setType(move.getCapturedPieceType());
    this->grid[move.getTo()] = capturedPiece;
}


void Board::unmakeCastlingMove(Move &move)
{
    Color color = move.getMovedPieceColor();

    Piece empty;
    empty.setType(PieceType::EMPTY);

    switch (move.getCastleType()) {
        case CastleType::KINGSIDE:
            if (color == Color::WHITE) {
                Piece rook = this->at(algebraicToIndex("f1"));

                this->setAt(algebraicToIndex("f1"), empty);
                this->setAt(algebraicToIndex("h1"), rook);

                this->setAt(algebraicToIndex("e1"), this->at(algebraicToIndex("g1")));
                this->setAt(algebraicToIndex("g1"), empty);

                addCastlingRights('K');
            } else {
                Piece rook = this->at(algebraicToIndex("f8"));

                this->setAt(algebraicToIndex("f8"), empty);
                this->setAt(algebraicToIndex("h8"), rook);

                this->setAt(algebraicToIndex("e8"), this->at(algebraicToIndex("g8")));
                this->setAt(algebraicToIndex("g8"), empty);

                addCastlingRights('k');
            }
            break;

        case CastleType::QUEENSIDE:
            if (color == Color::WHITE) {
                Piece rook = this->at(algebraicToIndex("d1"));

                this->setAt(algebraicToIndex("d1"), empty);
                this->setAt(algebraicToIndex("a1"), rook);

                this->setAt(algebraicToIndex("e1"), this->at(algebraicToIndex("c1")));
                this->setAt(algebraicToIndex("c1"), empty);

                addCastlingRights('Q');
            } else {
                Piece rook = this->at(algebraicToIndex("d8"));

                this->setAt(algebraicToIndex("d8"), empty);
                this->setAt(algebraicToIndex("a8"), rook);

                this->setAt(algebraicToIndex("e8"), this->at(algebraicToIndex("c8")));
                this->setAt(algebraicToIndex("c8"), empty);

                addCastlingRights('q');
            }
            break;

        default:
            break;
    }
}


void Board::unmakeEnPassantMove(Move &move) {
    Piece empty;
    empty.setType(PieceType::EMPTY);

    Piece movedPiece = this->at(move.getTo());
    PieceType movedPieceType = move.getMovedPieceType();
    this->setAt(move.getTo(), empty);
    this->setAt(move.getFrom(), movedPiece);

    Piece capturedPawn;
    PieceType capturedPawnType = movedPieceType == WP ? BP : WP;
    capturedPawn.setType(capturedPawnType);

    int diff = move.getMovedPieceColor() == Color::WHITE ? 8 : -8;
    this->setAt(move.getTo() + diff, capturedPawn);

    // need to make the en passant available again
}


void Board::unmakeMove() 
{
    if (!this->undoStack.size()) {
        std::cout << "ERROR: tried to unmake a move on a fresh board" << std::endl;
        return;
    }

    Move move = this->undoStack.back();
    this->undoStack.pop_back();

    switch(move.getMoveType()) {
        case MoveType::CASTLING:
            unmakeCastlingMove(move);
            break;
        case MoveType::PROMOTION:
            break;
        case MoveType::ENPASSANT:
            unmakeEnPassantMove(move);
            break;
        default:
            unmakeNormalMove(move);
            break;
    }
    this->swapTurn();
}


SquareContent Board::squareState(int from, int target) 
{
    PieceType fromPieceType = this->at(from).getType();
    PieceType targetPieceType = this->at(target).getType();

    if (targetPieceType == PieceType::EMPTY) return SquareContent::EMPTY_SQUARE;

    Color fromColor = fromPieceType < 7 ? Color::WHITE : Color::BLACK;
    Color targetColor = targetPieceType < 7 ? Color::WHITE : Color::BLACK;

    if (fromColor == targetColor) return SquareContent::FRIEND_SQUARE;
    return SquareContent::ENEMY_SQUARE;
}


PieceType Board::getSquarePieceType(int index) 
{
    return this->at(index).getType();
}


int Board::findKing(bool white) 
{
    PieceType target = white ? PieceType::WK : PieceType::BK;
    for (int i = 0; i < 64; i++) {
        if (this->at(i).getType() == target) return i;
    }
    return -1; // There should always be both kings on board
}


bool Board::isSquareAttacked(int square, bool byWhite) 
{
    for (int i = 0; i < 64; i++) {
        Piece piece = this->at(i);
        PieceType type = piece.getType();

        if (type == EMPTY) continue;

        bool pieceIsWhite = (type >= WP && type <= WK);
        if (byWhite && !pieceIsWhite) continue;
        if (!byWhite && pieceIsWhite) continue;

        // pawns need to be handled separately because they attack differently than how they move
        if (type == WP || type == BP) {
            int rowDelta = byWhite ? -1 : 1;
            int fileOffsets[2] = { -1, 1 };

            int fromRow = i / 8;
            int fromCol = i % 8;

            for (int k = 0; k < 2; k++) {
                int targetRow = fromRow + rowDelta;
                int targetCol = fromCol + fileOffsets[k];

                if (targetRow < 0 || targetRow > 7 || targetCol < 0 || targetCol > 7) continue;

                int attackedSquare = targetRow * 8 + targetCol;
                if (attackedSquare == square) return true; // return immediately if square found
            }
        } else {
            // all the squares that are attacked by white or black depending on byWhite
            std::vector<Move> moves;
            MoveGenerator mg;
            mg.generateMoves(moves, i, *this);

            for (Move move : moves) {
                if (move.getTo() == square) return true;
            }
        }
    }
    return false;
}


bool Board::isInCheck(bool white) 
{
    int index = this->findKing(white);
    return this->isSquareAttacked(index, !white);
}


bool Board::isMoveLegal(std::vector<Move> &legalMoves, int from, int to, Move &foundMove)
{
    Color turn = this->getWhoseTurn();
    Color whoIsMoving = this->getPieceColor(from);
    std::cout << "Piece color about to move: " << whoIsMoving << std::endl;

    if (turn != whoIsMoving) {
        std::cout << "It's not your turn to move!" << std::endl;
        return false;
    }

    for (auto &legalMove : legalMoves) {
        if (legalMove.getFrom() == from && legalMove.getTo() == to) {
            foundMove = legalMove;
            return true;   
        }
    }
    return false;
}