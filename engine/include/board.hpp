#pragma once

#include <vector>

#include "piece.hpp"

class Board 
{

private:
    std::vector<Piece> board;
public:
    Board();
    ~Board();

    void setupCustomBoard(std::vector<PieceType>);
    void setupDefaultBoard(); // Sets the default chessboard
    std::vector<Piece> getBoard() {return this->board;}
};