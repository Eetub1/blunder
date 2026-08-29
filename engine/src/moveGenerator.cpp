#include <array>

#include "moveGenerator.hpp"
#include "types.hpp"
#include "move.hpp"
#include "utils.hpp"

std::array<int, 8> KNIGHT_OFFSETS = {-17, -15, -10, -6, 6, 10, 15, 17};
std::array<int, 4> BISHOP_OFFSETS = {-9, -7, 7, 9};
std::array<int, 4> ROOK_OFFSETS = {-8, -1, 1, 8};
std::array<int, 8> KING_AND_QUEEN_OFFSETS = {-9, -8, -7, -1, 1, 7, 8, 9};


std::vector<Move> MoveGenerator::generateLegalMoves(Board &board) 
{
    std::vector<Move> legalMoves;
    std::vector<Piece> grid = board.getGrid();

    for (int i = 0; i < 64; i++) {
        if (grid[i].getType() == PieceType::EMPTY) continue;

        // this returns all the pseudolegal moves in the position
        this->generateMoves(legalMoves, i, board);
    }
    return legalMoves;
}


void MoveGenerator::generateMoves(std::vector<Move> &movesVector, int from, Board &board) 
{
    PieceType type = board.getSquarePieceType(from);

    switch (type) {
        case WP:
        case BP:
            generatePawnMoves(movesVector, from, board);
            break;
        case WN:
        case BN:
            generateSteppingMoves(movesVector, from, board, KNIGHT_OFFSETS);
            break;    
        case WB:
        case BB:
            generateSlidingMoves(movesVector, from, board, BISHOP_OFFSETS);
            break;
        case WR:
        case BR:
            generateSlidingMoves(movesVector, from, board, ROOK_OFFSETS);
            break;    
        case WQ:
        case BQ:
            generateSlidingMoves(movesVector, from, board, KING_AND_QUEEN_OFFSETS);
            break;
        case WK:
        case BK:
            generateKingMoves(movesVector, from, board, KING_AND_QUEEN_OFFSETS);
            break;
        default:
            break;
    }
}


void MoveGenerator::generateSteppingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS) 
{
    for (size_t i = 0; i < OFFSETS.size(); i++) {
        int targetSquare = from + OFFSETS[i];
        if (targetSquare < 0 || targetSquare > 63) continue;
        if (abs((from % 8) - (targetSquare % 8)) > 2) continue;

        SquareContent targetSquareState = board.squareState(from, targetSquare);
        if (targetSquareState != FRIEND_SQUARE) {
            Move move(from, targetSquare, board.getSquarePieceType(from));
            movesVector.push_back(move);
        }
    }
}


void MoveGenerator::generateKingMoves(std::vector<Move> &movesVector, int from, Board &board, std::array<int, 8> &OFFSETS)
{
    // Calculate the squares the king can move to normally
    generateSteppingMoves(movesVector, from, board, OFFSETS); 

    // All the castling moves need to be handled here as special cases
    std::string castlingRights = board.getCastlingRights();
    if (castlingRights == "-") return;

    if (board.getWhoseTurn() == Color::WHITE) {
        // Kingside white
        if (castlingRights.find("K") != std::string::npos) {
            SquareContent f1 = board.squareState(from, algebraicToIndex("f1"));
            SquareContent g1 = board.squareState(from, algebraicToIndex("g1"));
            if (f1 == g1 && g1 == SquareContent::EMPTY_SQUARE) {
                movesVector.push_back(Move(
                    from,                        // e1 index
                    algebraicToIndex("g1"),      // destination square
                    PieceType::WK,               // moved piece
                    -1,                          // enPassantSquare
                    PieceType::EMPTY,            // capturedPieceType
                    PieceType::EMPTY,            // promotionPiece
                    CastleType::KINGSIDE,        // castleType
                    MoveType::CASTLING           // moveType
                ));
            }
        }

        // Queenside white
        if (castlingRights.find("Q") != std::string::npos) {
            SquareContent b1 = board.squareState(from, algebraicToIndex("b1"));
            SquareContent c1 = board.squareState(from, algebraicToIndex("c1"));
            SquareContent d1 = board.squareState(from, algebraicToIndex("d1"));
            if (b1 == c1 && c1 == d1 && d1 == SquareContent::EMPTY_SQUARE) {
                movesVector.push_back(Move(
                    from, 
                    algebraicToIndex("c1"), 
                    PieceType::WK,
                    -1, 
                    PieceType::EMPTY, 
                    PieceType::EMPTY, 
                    CastleType::QUEENSIDE, 
                    MoveType::CASTLING
                ));
            }
        }
    } else {
        // Kingside black
        if (castlingRights.find("k") != std::string::npos) {
            SquareContent f8 = board.squareState(from, algebraicToIndex("f8"));
            SquareContent g8 = board.squareState(from, algebraicToIndex("g8"));
            if (f8 == g8 && g8 == SquareContent::EMPTY_SQUARE) {
                movesVector.push_back(Move(
                    from, 
                    algebraicToIndex("g8"), 
                    PieceType::BK,
                    -1, 
                    PieceType::EMPTY, 
                    PieceType::EMPTY, 
                    CastleType::KINGSIDE, 
                    MoveType::CASTLING
                ));
            }
        }

        // Queenside black
        if (castlingRights.find("q") != std::string::npos) {
            SquareContent b8 = board.squareState(from, algebraicToIndex("b8"));
            SquareContent c8 = board.squareState(from, algebraicToIndex("c8"));
            SquareContent d8 = board.squareState(from, algebraicToIndex("d8"));
            if (b8 == c8 && c8 == d8 && d8 == SquareContent::EMPTY_SQUARE) {
                movesVector.push_back(Move(
                    from, 
                    algebraicToIndex("c8"), 
                    PieceType::BK,
                    -1,
                    PieceType::EMPTY, 
                    PieceType::EMPTY, 
                    CastleType::QUEENSIDE,
                    MoveType::CASTLING
                ));
            }
        }
    }
}


void MoveGenerator::generatePawnMoves(std::vector<Move> &movesVector, int from, Board &board) {
    bool isWhite = board.getPieceColor(from) == Color::WHITE;

    int forward = isWhite ? -8 : 8; // which direction is forward depends on piece color
    int startRow = isWhite ? 6 : 1; // in which row does the pawn start the game
    int row = from / 8;

    // can pawn go one square forward
    int oneAhead = from + forward;
    if (oneAhead >= 0 && oneAhead < 64) {
        if (board.squareState(from, oneAhead) == SquareContent::EMPTY_SQUARE) {
            movesVector.push_back(Move(
                from, 
                oneAhead, 
                isWhite ? PieceType::WP : PieceType::BP,
                -1,
                PieceType::EMPTY, 
                PieceType::EMPTY, 
                CastleType::NONE,
                MoveType::NORMAL
            ));

            // can pawn go two moves forward
            if (row == startRow) {
                int twoAhead = from + 2 * forward;
                if (board.squareState(from, twoAhead) == SquareContent::EMPTY_SQUARE) {
                    movesVector.push_back(Move(
                        from, 
                        twoAhead, 
                        isWhite ? PieceType::WP : PieceType::BP,
                        -1,
                        PieceType::EMPTY, 
                        PieceType::EMPTY, 
                        CastleType::NONE,
                        MoveType::NORMAL
                    ));
                }
            }
        }
    }

    // can pawn capture anything
    int capture_offsets[2] = { forward - 1, forward + 1};
    for (int i = 0; i < 2; i++) {
        int target = from + capture_offsets[i];

        if (target < 0 || target >= 64) continue;
        if (abs((from % 8) - (target % 8)) != 1) continue;

        if (board.squareState(from, target) == SquareContent::ENEMY_SQUARE) {
            movesVector.push_back(Move(
                from, 
                target, 
                isWhite ? PieceType::WP : PieceType::BP,
                -1,
                board.getSquarePieceType(target), 
                PieceType::EMPTY, 
                CastleType::NONE,
                MoveType::NORMAL
            ));
        }

    }

    // is en passant possible
    // this enPassantSquare is one behind the pawn that moved on the previous turn
    // check if we are attacking this square with a pawn. So check the pawn's left and right attack
    int enPassantSquare = board.getEnPassantSquare();
    if (enPassantSquare >= 0) {
        for (int i = 0; i < 2; i++) {
            int target = from + capture_offsets[i];

            if (target < 0 || target >= 64) continue;
            if (abs((from % 8) - (target % 8)) != 1) continue;

            if (target == enPassantSquare) {
                movesVector.push_back(Move(
                    from, 
                    target, 
                    isWhite ? PieceType::WP : PieceType::BP,
                    enPassantSquare,
                    isWhite ? PieceType::BP : PieceType::WP, 
                    PieceType::EMPTY, 
                    CastleType::NONE,
                    MoveType::ENPASSANT
                ));
            }
        }
    }
}