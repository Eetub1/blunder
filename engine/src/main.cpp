#include <utility>
#include <sstream>

#include "utils.hpp"
#include "types.hpp"
#include "board.hpp"
#include "moveGenerator.hpp"
#include "move.hpp"


std::pair<int, int> readInput(int &state)
{
    std::string line;
    std::getline(std::cin, line);

    if (line == "un") {
        state = InputState::UNMAKEMOVE;
        return {-1, -1};
    } else if (line == "quit") {
        state = InputState::QUIT;
        return {-1, -1};
    } else if (line.size() == 5) {
        std::string from;
        std::string to;
        std::stringstream ss(line);
        ss >> from >> to;

        int fromIndex = algebraicToIndex(from);
        int toIndex = algebraicToIndex(to);
        
        if (!(0 <= fromIndex && fromIndex < 64) || !(0 <= toIndex && toIndex < 64)) {
            state = InputState::INVALID;
            return {-1, -1};
        }

        state = InputState::MAKEMOVE;
        return {fromIndex, toIndex};
    } else {
        state = InputState::INVALID;
        return {-1, -1};
    }
}


void playGameWithInput() 
{
    Board board;
    board.setupDefaultBoard();
    printBoard(board);

    while (true) {
        int state = -1;
        std::pair<int, int> move = readInput(state);
        int from = move.first;
        int to = move.second;

        switch (state) {
            case InputState::INVALID:
                std::cout << "INPUT WAS INVALID" << std::endl;
                printBoard(board);
                continue;

            case InputState::QUIT:
                std::cout << "Exiting game" << std::endl;
                return;

            case InputState::UNMAKEMOVE:
                std::cout << "Unmaking previous move" << std::endl;
                board.unmakeMove();
                printBoard(board);
                break;

            default: // InputState::MAKEMOVE
            {
                // Move generator can't modify board, castling rights, en passant or whose turn it is
                std::vector<Piece> gridBefore = board.getGrid();
                int epBefore = board.getEnPassantSquare();
                std::string rightsBefore = board.getCastlingRights();
                Color turnBefore = board.getWhoseTurn();

                MoveGenerator mg;
                std::vector<Move> legalMoves = mg.generateLegalMoves(board);

                // Make sure that generateLegalMoves doesn't change state
                {   
                    std::cout << "---------------------------" << std::endl;
                    for (int i = 0; i < 64; i++) {
                        if (gridBefore[i].getType() != board.getSquarePieceType(i)) {
                            std::cout << "move generator modified board!!!!" << std::endl;
                            break;
                        }
                    }
                
                    if (epBefore != board.getEnPassantSquare()) {
                        std::cout << "move generator modified en passant square!!!!" << std::endl;
                    }

                    if (rightsBefore != board.getCastlingRights()) {
                        std::cout << "castling rights differ: " << rightsBefore << " " << board.getCastlingRights() << std::endl;
                    }

                    if (turnBefore != board.getWhoseTurn()) {
                        std::cout << "move generator modified whose turn it is!!!!" << std::endl;
                    }

                    std::cout << "---------------------------" << std::endl;
                }

                Move foundMove;
                bool isValid = board.isMoveLegal(legalMoves, from, to, foundMove);
                if (!isValid) {
                    std::cout << "Made move was not valid" << std::endl;
                    break;
                }

                if (board.shouldAskForPromotionPiece()) {
                    // TODO
                    std::cout << "Need to ask for the promotion piece here" << std::endl;
                    // after we get the promotion piece from input, set the promotionPiece 
                    // attribute of the move object!
                    board.setShouldAskForPromotionPiece(false);
                }

                board.makeMove(foundMove);

                // Does next side have any moves they can make
                std::vector<Move> replies = mg.generateLegalMoves(board);
                GameState gameState = board.getGameState(replies.size());

                printBoard(board);

                if (gameState == GameState::CHECKMATE) {
                    std::string winner = board.getWhoseTurn() == Color::WHITE ? "BLACK" : "WHITE";
                    std::cout << "GAME IS OVER, THE WINNER IS: " << winner << std::endl;
                    return;
                }
                if (gameState == GameState::STALEMATE) {
                    std::cout << "GAME IS OVER, STALEMATE" << std::endl;
                    return;
                }
                break;
            }
        }
        std::string turn = board.getWhoseTurn() == Color::WHITE ? "White" : "Black";
        std::cout << "Turn: " << turn << std::endl;
    }
}


/*void playGameFromData(std::vector<std::string> &moves)
{

}*/


int main() 
{

    playGameWithInput();
    // playGameFromData(operaGameMoves);
    return 0;
}