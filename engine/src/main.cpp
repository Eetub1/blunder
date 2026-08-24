#include "utils.hpp"
#include "types.hpp"
#include "board.hpp"


void playGameWithInput() {
    Board board;
    board.setupDefaultBoard();
    printBoard(board);

    while (true) {
        std::string from;
        std::string to;
        std::getline(std::cin, from, ' ');
        std::getline(std::cin, to, '\n');

        if (from == "un") {
            std::cout << "Unmaking previous move" << std::endl;
            board.unmakeMove();
        } else {
            int fromIndex = algebraicToIndex(from);
            int toIndex = algebraicToIndex(to);
        
            if (!(0 <= fromIndex && fromIndex < 64) || !(0 <= toIndex && toIndex < 64)) {
                std::cout << "Invalid input, exiting game" << std::endl;
                break;
            }
            
            std::cout << "Making a move!" << std::endl;
            // Should actually validate the move before making it!

            Move move;
            move.from = fromIndex;
            move.to = toIndex;

            board.makeMove(move);
        }
        printBoard(board);
    }
}


int main() 
{
    playGameWithInput();

    return 0;
}