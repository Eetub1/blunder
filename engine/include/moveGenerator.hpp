#pragma once

#include <vector>

#include "types.hpp"
#include "board.hpp"

class MoveGenerator {

public:
    MoveGenerator() {};
    ~MoveGenerator() {};

    // Generates all the legal moves that can be played on board
    std::vector<Move> generateLegalMoves(Board &board);

private:
    // Generates all pseudolegal moves from a given position. Pseudolegal meaning that checks are not taken into consideration
    void generateMoves(std::vector<Move> &movesVector, int from, Board &board);
    void generateSteppingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS);
};