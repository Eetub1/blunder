
#include "moveGenerator.hpp"
#include "types.hpp"


std::vector<Move> MoveGenerator::generateLegalMoves(Board &board) {
    std::vector<Move> legalMoves;
    std::vector<Piece> grid = board.getGrid();

    for (int i = 0; i < 63; i++) {
        if (grid[i].getType() == PieceType::EMPTY) continue;

        this->generateMoves(legalMoves, i, board);
    }

    return legalMoves;
}


void generateMoves(std::vector<Move> movesVector, int from, Board &board) {

}