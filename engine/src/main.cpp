#include "utils.hpp"
#include "board.hpp"

int main() 
{
    Board board;
    board.setupDefaultBoard();
    printBoard(board);

    return 0;
}