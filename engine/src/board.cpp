#include <vector>

#include "board.hpp"
#include "boardPositions.hpp"
#include "piece.hpp"
#include "types.hpp"

Board::Board() 
    : grid(64) 
{}


Board::~Board() {}


void Board::setupDefaultBoard() 
{
    for (unsigned int i = 0; i < 64; i++) {
        Piece p;
        p.setType(defaultBoard[i]);
        this->grid[i] = p;
    }
}


void Board::makeMove(Move &move) {
    // Make the move on board
    // Also save the move data to the undostack

    switch(move.moveType) {
        // Probably should handle other cases in their own functions?

        // This could also maybe be its own function?
        default: // Normal move
            Piece toSquarePiece = this->grid[move.to];
            move.capturedPieceType = toSquarePiece.getType();
            this->grid[move.to] = this->grid[move.from];
            Piece empty;
            empty.setType(PieceType::EMPTY);
            this->grid[move.from] = empty;
            break;
    }



    undoStack.push_back(move);
}


void Board::unmakeMove() {
    if (!this->undoStack.size()) {
        std::cout << "ERROR: tried to unmake a move on a fresh board" << std::endl;
        return;
    }

    Move move = this->undoStack.back();
    this->undoStack.pop_back();

    switch(move.moveType) {
        default: // MoveType::NORMAL
            Piece toSquarePiece = this->grid[move.to];
            this->grid[move.from] = toSquarePiece;
            Piece capturedPiece;
            capturedPiece.setType(move.capturedPieceType);
            this->grid[move.to] = capturedPiece;
            break;
    }
}


SquareContent Board::squareState(int from, int target) {
    std::vector<Piece> grid = this->getGrid();
    PieceType fromPieceType = grid[from].getType();
    PieceType targetPieceType = grid[target].getType();

    if (targetPieceType == PieceType::EMPTY) return SquareContent::EMPTY_SQUARE;

    Color fromColor = fromPieceType < 7 ? Color::WHITE : Color::BLACK;
    Color targetColor = targetPieceType < 7 ? Color::WHITE : Color::BLACK;

    if (fromColor == targetColor) return SquareContent::FRIEND_SQUARE;
    return SquareContent::ENEMY_SQUARE;
}