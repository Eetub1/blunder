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
    int enPassantSquare = -1;

    Color whoseTurn = Color::WHITE;
public:
    Board();
    ~Board();

    void setupCustomBoard(std::vector<PieceType>);
    void setupDefaultBoard();

    void               swapTurn() {this->whoseTurn = this->whoseTurn == Color::WHITE ? Color::BLACK : Color::WHITE;}
    int                findKing(bool white);
    bool               isSquareAttacked(int from, bool byWhite);
    bool               isInCheck(bool white);
    bool               isMoveLegal();  
    SquareContent      squareState(int from, int target); // Returns what is in the target square in relation to the from square
    Piece              at(int index) {return this->grid[index];}
    void               setAt(int index, Piece piece) {this->grid[index] = piece;}

    PieceType          getSquarePieceType(int index);
    std::vector<Piece> getGrid() {return this->grid;}
    Color              getPieceColor(int index) {return this->grid[index].getColor();}
    int                getEnPassantSquare() {return this->enPassantSquare;}
    std::string        getCastlingRights() {return this->castlingRights;}
    Color              getWhoseTurn() {return this->whoseTurn;}

    // Makes the move specified by the move object on board. Assumes that the move is perfectly valid.
    void makeMove(Move &move);
    void unmakeMove();

    void makeCastlingMove(Move &move);
};