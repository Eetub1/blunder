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

        state = InputState::MOVE;
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

        if (state == InputState::INVALID) {
            std::cout << "INPUT WAS INVALID" << std::endl;
            continue;
        } else if (state == InputState::QUIT) {
            std::cout << "Exiting game" << std::endl;
            break;
        } else if (state == InputState::UNMAKEMOVE) {
            std::cout << "Unmaking previous move" << std::endl;
            board.unmakeMove();
        } else {
            MoveGenerator mg;
            std::vector<Move> legalMoves = mg.generateLegalMoves(board);

            Move foundMove;
            bool isValid = board.isMoveLegal(legalMoves, from, to, foundMove);
            if (isValid) {
                board.makeMove(foundMove);
                board.swapTurn();
            }

            // std::cout << "En passant: " << board.getEnPassantSquare() << std::endl;
            std::cout << "Made move was " << (isValid ? "valid" : "not valid") << std::endl; 
        }

        printBoard(board);
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