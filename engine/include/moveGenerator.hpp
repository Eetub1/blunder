#pragma once

#include <vector>

#include "types.hpp"
#include "board.hpp"
#include "move.hpp"

class MoveGenerator {

public:
    MoveGenerator() {};
    ~MoveGenerator() {};

    // Generates all the legal moves that can be played on board. Take checks into consideration
    std::vector<Move> generateLegalMoves(Board &board);

private:
    // Generates all pseudolegal moves from a given position. Pseudolegal meaning that checks are not taken into consideration
    void generateMoves(std::vector<Move> &movesVector, int from, Board &board);
    void generateSteppingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS);
    void generateKingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS);
    void generatePawnMoves(std::vector<Move> &movesVector, int from, Board &board);

    template <std::size_t N>
    void generateSlidingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, N> &OFFSETS) 
    {
        for (int offset : OFFSETS) {
            int targetSquare = from + offset;
            int prevSquare = from;

            while (true) {
                if (targetSquare < 0 || targetSquare > 63) break;
                if (abs((prevSquare % 8) - (targetSquare % 8)) > 1) break; // if the amount moved to left or right is greater than 1

                SquareContent square = board.squareState(from, targetSquare);
                if (square == EMPTY_SQUARE  || square == ENEMY_SQUARE) {
                    Move move(from, targetSquare, board.getGrid()[from].getType());
                    movesVector.push_back(move);
                }

                if (square == FRIEND_SQUARE || square == ENEMY_SQUARE) break;
                
                prevSquare = targetSquare;
                targetSquare += offset; // keep going in the direction
            }
        }
    }
};