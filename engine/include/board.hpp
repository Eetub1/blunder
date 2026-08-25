#pragma once

#include <vector>
#include <iostream>

#include "piece.hpp"
#include "types.hpp"


class Board 
{

private:
    std::vector<Piece> grid;
    std::vector<Move> undoStack;
    Color whoseTurn = Color::WHITE;
public:
    Board();
    ~Board();

    void setupCustomBoard(std::vector<PieceType>);
    void setupDefaultBoard();

    void swapTurn() {this->whoseTurn = this->whoseTurn == Color::WHITE ? Color::BLACK : Color::WHITE;}
    std::vector<Piece> getGrid() {return this->grid;}
    SquareContent squareState(int from, int target); // Returns what is in the target square in relation to the from square

    /**
     * @brief Makes the move specified by the move struct on board
     *        Assumes that the move is perfectly valid.
     * 
     * @param move 
     */
    void makeMove(Move &move);
    void unmakeMove();
};