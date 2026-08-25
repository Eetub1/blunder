#include <vector>

#include "board.hpp"
#include "boardPositions.hpp"
#include "piece.hpp"
#include "types.hpp"

Board::Board() 
    : board(64) 
{}


Board::~Board() {}


void Board::setupDefaultBoard() 
{
    for (unsigned int i = 0; i < 64; i++) {
        Piece p;
        p.setType(defaultBoard[i]);
        this->board[i] = p;
    }
}


void Board::makeMove(Move &move) {
    // Make the move on board
    // Also save the move data to the undostack

    switch(move.moveType) {
        // Probably should handle other cases in their own functions?

        // This could also maybe be its own function?
        default: // Normal move
            Piece toSquarePiece = this->board[move.to];
            move.capturedPieceType = toSquarePiece.getType();
            this->board[move.to] = this->board[move.from];
            Piece empty;
            empty.setType(PieceType::EMPTY);
            this->board[move.from] = empty;
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
            Piece toSquarePiece = this->board[move.to];
            this->board[move.from] = toSquarePiece;
            Piece capturedPiece;
            capturedPiece.setType(move.capturedPieceType);
            this->board[move.to] = capturedPiece;
            break;
    }
}
