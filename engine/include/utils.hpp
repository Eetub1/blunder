#pragma once

#include <string>
#include <iostream>

#include "board.hpp"
#include "piece.hpp"

std::string pieceToGlyph(PieceType type) {
    switch (type) {
        case BK: return "\u2654"; // ♔
        case BQ: return "\u2655"; // ♕
        case BR: return "\u2656"; // ♖
        case BB: return "\u2657"; // ♗
        case BN: return "\u2658"; // ♘
        case BP: return "\u2659"; // ♙
        case WK: return "\u265A"; // ♚
        case WQ: return "\u265B"; // ♛
        case WR: return "\u265C"; // ♜
        case WB: return "\u265D"; // ♝
        case WN: return "\u265E"; // ♞
        case WP: return "\u265F"; // ♟
        case EMPTY:
        default:  return ".";
    }
}


void printBoard(Board board) 
{
    std::vector<Piece> pieceBoard = board.getBoard();
    for (unsigned int i = 0; i < 64; i++) {
        std::cout << pieceToGlyph(pieceBoard[i].getType());
        if (i % 8 == 7) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}

