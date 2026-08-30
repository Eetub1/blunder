#pragma once

#include <string>
#include <iostream>

#include "board.hpp"
#include "piece.hpp"


inline std::string pieceToGlyph(PieceType type) {
    switch (type) {
        case WK: return "\u265A"; // ♚
        case WQ: return "\u265B"; // ♛
        case WR: return "\u265C"; // ♜
        case WB: return "\u265D"; // ♝
        case WN: return "\u265E"; // ♞
        case WP: return "\u265F"; // ♟
        case BK: return "\u2654"; // ♔
        case BQ: return "\u2655"; // ♕
        case BR: return "\u2656"; // ♖
        case BB: return "\u2657"; // ♗
        case BN: return "\u2658"; // ♘
        case BP: return "\u2659"; // ♙
        case EMPTY:
        default:  return ".";
    }
}


inline void printBoard(Board board) 
{
    for (unsigned int i = 0; i < 64; i++) {
        std::cout << pieceToGlyph(board.at(i).getType()) << " ";
        if (i % 8 == 7) {
            std::cout << '\n';
        }
    }
    std::cout << '\n';
}


inline int algebraicToIndex(std::string position) {
    if (position.size() != 2) return -1;
    return ('8' - position[1]) * 8 + (position[0] - 'a');
}


inline std::string indexToAlgebraic(int index) {
    int row = index / 8;
    int col = index % 8;

    std::string result = "";

    char colStr = 'a' + col;
    char rowStr = '8' - row; 
    result += colStr;
    result += rowStr;

    return result; 
}