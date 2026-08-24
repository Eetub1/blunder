#include "utils.hpp"
#include "board.hpp"

int main() 
{
    Board board;
    board.setupDefaultBoard();
    std::cout << "Moi" << std::endl;
    printBoard(board);

    return 0;
}