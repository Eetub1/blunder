#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "piece.hpp"
#include "types.hpp"
#include "move.hpp"


class Board 
{

private:
    std::vector<Piece> grid;
    std::vector<Move> undoStack;
    std::string castlingRights = "KQkq";

    Color whoseTurn = Color::WHITE;
public:
    Board();
    ~Board();

    std::string getCastlingRights() {return this->castlingRights;}
    Color getWhoseTurn() {return this->whoseTurn;}

    void setupCustomBoard(std::vector<PieceType>);
    void setupDefaultBoard();

    void swapTurn() {this->whoseTurn = this->whoseTurn == Color::WHITE ? Color::BLACK : Color::WHITE;}
    std::vector<Piece> getGrid() {return this->grid;}
    SquareContent squareState(int from, int target); // Returns what is in the target square in relation to the from square
    PieceType getSquarePieceType(int index);

    // Makes the move specified by the move struct on board. Assumes that the move is perfectly valid.
    void makeMove(Move &move);
    void unmakeMove();
};