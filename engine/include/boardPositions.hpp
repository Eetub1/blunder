#pragma once

#include "piece.hpp"

PieceType defaultBoard[64]  = { 
    BR, BN, BB, BQ, BK, BB, BN, BR,
    BP, BP, BP, BP, BP, BP, BP, BP,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WP, WP, WP, WP, WP, WP, WP, WP,
    WR, WN, WB, WQ, WK, WB, WN, WR 
};

// The famous opera game
std::vector<std::string> operaGameMoves = {
    "e2-e4", "e7-e5", // 1. e4 e5
    "g1-f3", "d7-d6", // 2. Nf3 d6
    "d2-d4", "c8-g4", // 3. d4 Bg4
    "d4-e5", "g4-f3", // 4. dxe5 Bxf3
    "d1-f3", "d6-e5", // 5. Qxf3 dxe5
    "f1-c4", "g8-f6", // 6. Bc4 Nf6
    "f3-b3", "d8-e7", // 7. Qb3 Qe7
    "b1-c3", "c7-c6", // 8. Nc3 c6
    "c1-g5", "b7-b5", // 9. Bg5 b5
    "c3-b5", "c6-b5", // 10. Nxb5 cxb5
    "c4-b5", "b8-d7", // 11. Bxb5+ Nbd7
    "e1-c1", "a8-d8", // 12. O-O-O Rdd8
    "d1-d7", "d8-d7", // 13. Rxd7 Rxd7
    "h1-d1", "e7-e6", // 14. Rd1 Qe6
    "b5-d7", "f6-d7", // 15. Bxd7+ Nxd7
    "b3-b8", "d7-b8", // 16. Qb8+ Nxb8
    "d1-d8"           // 17. Rd8# (Checkmate)
};