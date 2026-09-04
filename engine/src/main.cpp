#include "utils.hpp"
#include "types.hpp"
#include "board.hpp"
#include "moveGenerator.hpp"
#include "move.hpp"


void playGameWithInput() 
{
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

            Color turn = board.getWhoseTurn();
            Color whoIsMoving = board.getPieceColor(fromIndex);
            std::cout << "Piece color about to move: " << whoIsMoving << std::endl;

            if (turn != whoIsMoving) {
                std::cout << "It's not your turn to move!" << std::endl;
                continue;
            }
            
            // Generate all possible moves in the position
            MoveGenerator mg;
            std::vector<Move> legalMoves = mg.generateLegalMoves(board);

            // Check if given move is in legalMoves
            bool isValid = false;
            for (auto &legalMove : legalMoves) {
                if (legalMove.getFrom() == fromIndex && legalMove.getTo() == toIndex) {
                    board.makeMove(legalMove);
                    board.swapTurn();
                    
                    isValid = true;
                    break;
                }
            }
            std::cout << "En passant: " << board.getEnPassantSquare() << std::endl;
            std::cout << "The move you made was " << (isValid ? "valid" : "not valid") << std::endl; 
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