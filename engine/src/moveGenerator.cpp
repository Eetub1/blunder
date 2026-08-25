#include <array>

#include "moveGenerator.hpp"
#include "types.hpp"

std::array<int, 8> KNIGHT_OFFSETS = {-17, -15, -10, -6, 6, 10, 15, 17};
std::array<int, 4> BISHOP_OFFSETS = {-9, -7, 7, 9};
std::array<int, 4> ROOK_OFFSETS = {-8, -1, 1, 8};
std::array<int, 8> KING_AND_QUEEN_OFFSETS = {-9, -8, -7, -1, 1, 7, 8, 9};

//const int KNIGHT_OFFSETS[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
//const int BISHOP_OFFSETS[4] = {-9, -7, 7, 9};
//const int ROOK_OFFSETS[4] = {-8, -1, 1, 8};
//const int KING_AND_QUEEN_OFFSETS[8] = {-9, -8, -7, -1, 1, 7, 8, 9};


std::vector<Move> MoveGenerator::generateLegalMoves(Board &board) {
    std::vector<Move> legalMoves;
    std::vector<Piece> grid = board.getGrid();

    for (int i = 0; i < 64; i++) {
        if (grid[i].getType() == PieceType::EMPTY) continue;

        this->generateMoves(legalMoves, i, board);
    }

    return legalMoves;
}


void MoveGenerator::generateMoves(std::vector<Move> &movesVector, int from, Board &board) {
    PieceType type = board.getGrid()[from].getType();

    switch (type) {
        case WP:
        case BP:
            // generatePawnMoves(state, position, legal_moves);
            break;
        case WN:
        case BN:
            generateSteppingMoves(movesVector, from, board, KNIGHT_OFFSETS);
            break;    
        case WB:
        case BB:
            // generateSlidingMoves(state, position, BISHOP_OFFSETS, 4, legal_moves);
            break;
        case WR:
        case BR:
            // generateSlidingMoves(state, position, ROOK_OFFSETS, 4, legal_moves);
            break;    
        case WQ:
        case BQ:
            // generateSlidingMoves(state, position, KING_AND_QUEEN_OFFSETS, 8, legal_moves);
            break;
        case WK:
        case BK:
            // generateKingMoves(state, position, KING_AND_QUEEN_OFFSETS, 8, legal_moves);
            break;
        default:
            break;
    }
}


void MoveGenerator::generateSteppingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS) {
    for (size_t i = 0; i < OFFSETS.size(); i++) {
        int targetSquare = from + OFFSETS[i];
        if (targetSquare < 0 || targetSquare > 63) continue;
        if (abs((from % 8) - (targetSquare % 8)) > 2) continue;

        SquareContent targetSquareState = board.squareState(from, targetSquare);
        if (targetSquareState != FRIEND_SQUARE) {
            Move move;
            move.from = from;
            move.to = targetSquare;
            movesVector.push_back(move);
        }
    }
}