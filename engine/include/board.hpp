#pragma once

#include <vector>
#include <iostream>

#include "piece.hpp"
#include "types.hpp"


class Board 
{

private:
    std::vector<Piece> board;
    std::vector<Move> undoStack;
public:
    Board();
    ~Board();

    void setupCustomBoard(std::vector<PieceType>);
    void setupDefaultBoard();
    std::vector<Piece> getBoard() {return this->board;}

    /**
     * @brief Makes the move specified by the move struct on board
     *        Assumes that the move is perfectly valid.
     * 
     * @param move 
     */
    void makeMove(Move &move);
    void unmakeMove();
};