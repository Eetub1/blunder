#include <vector>

#include "board.hpp"
#include "boardPositions.hpp"
#include "piece.hpp"

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