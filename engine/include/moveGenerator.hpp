#pragma once

#include <vector>

#include "types.hpp"
#include "board.hpp"

class MoveGenerator {

public:
    MoveGenerator() {};
    ~MoveGenerator() {};

    /**
     * @brief Generates all the legal moves that can be played on board
     * 
     * @param board 
     * @return std::vector<Move> Returns all the moves in the vector
     */
    std::vector<Move> generateLegalMoves(Board &board);

private:
    
    /**
     * @brief Generates all pseudolegal moves from a given position
     *        Pseudolegal meaning that checks are not taken into consideration
     * 
     * @param movesVector 
     * @param from 
     * @param board 
     */
    void generateMoves(std::vector<Move> movesVector, int from, Board &board);
};