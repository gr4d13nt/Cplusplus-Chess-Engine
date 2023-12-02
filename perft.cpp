#include "perft.h"

std::string moveToAlgebra(const std::string& move) {
    std::string moveString = "";
    moveString += static_cast<char>(move[1] + 49);
    moveString += static_cast<char>('8' - move[0]);
    moveString += static_cast<char>(move[3] + 49);
    moveString += static_cast<char>('8' - move[2]);
    return moveString;
}

int perftTotalMoveCounter = 0;
int perftMoveCounter = 0;
int perftMaxDepth = 3;

void perft(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK, U64 BP, U64 BN,
           U64 BB, U64 BR, U64 BQ, U64 BK, U64 EP, bool CWK, bool CWQ, bool CBK,
           bool CBQ, bool WhiteToMove, int depth) {
    if (depth < perftMaxDepth) {
        std::string moves;
        if (WhiteToMove) {
            moves = Moves::possibleMovesW(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ,
                                   BK, EP, CWK, CWQ, CBK, CBQ);
        }
        else {
            moves = Moves::possibleMovesB(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ,
                                   BK, EP, CWK, CWQ, CBK, CBQ);
        }

        for (size_t i = 0; i < moves.length(); i += 4) {
            U64 WPt = Moves::makeMove(WP, moves.substr(i, 4), 'P'),
                WNt = Moves::makeMove(WN, moves.substr(i, 4), 'N'),
                WBt = Moves::makeMove(WB, moves.substr(i, 4), 'B'),
                WRt = Moves::makeMove(WR, moves.substr(i, 4), 'R'),
                WQt = Moves::makeMove(WQ, moves.substr(i, 4), 'Q'),
                WKt = Moves::makeMove(WK, moves.substr(i, 4), 'K'),
                BPt = Moves::makeMove(BP, moves.substr(i, 4), 'p'),
                BNt = Moves::makeMove(BN, moves.substr(i, 4), 'n'),
                BBt = Moves::makeMove(BB, moves.substr(i, 4), 'b'),
                BRt = Moves::makeMove(BR, moves.substr(i, 4), 'r'),
                BQt = Moves::makeMove(BQ, moves.substr(i, 4), 'q'),
                BKt = Moves::makeMove(BK, moves.substr(i, 4), 'k'),
                EPt = Moves::makeMoveEP(WP | BP, moves.substr(i, 4));

            WRt = Moves::makeMoveCastle(WRt, WK | BK, moves.substr(i, 4), 'R');
            BRt = Moves::makeMoveCastle(BRt, WK | BK, moves.substr(i, 4), 'r');

            bool CWKt = CWK, CWQt = CWQ, CBKt = CBK, CBQt = CBQ;
            int start = (moves[i] - '0') * 8 + (moves[i + 1] - '0');
            // ... rest of the logic for updating castle rights ...

            if (((WKt & Moves::unsafeForWhite(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt,
                                       BBt, BRt, BQt, BKt)) == 0 &&
                 WhiteToMove) ||
                ((BKt & Moves::unsafeForBlack(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt,
                                       BBt, BRt, BQt, BKt)) == 0 &&
                 !WhiteToMove)) {
                if (depth + 1 == perftMaxDepth) {
                    perftMoveCounter++;
                }
                perft(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt, BBt, BRt, BQt,
                      BKt, EPt, CWKt, CWQt, CBKt, CBQt, !WhiteToMove, depth + 1);
            }
        }
    }
}

void perftRoot(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR,
                      uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN,
                      uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK,
                      uint64_t EP, bool CWK, bool CWQ, bool CBK, bool CBQ,
                      bool WhiteToMove, int depth) {
    std::string moves;
    if (WhiteToMove) {
        moves = Moves::possibleMovesW(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR,
                                      BQ, BK, EP, CWK, CWQ, CBK, CBQ);
    }
    else {
        moves = Moves::possibleMovesB(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR,
                                      BQ, BK, EP, CWK, CWQ, CBK, CBQ);
    }

    for (size_t i = 0; i < moves.length(); i += 4) {
        uint64_t WPt = Moves::makeMove(WP, moves.substr(i, 4), 'P');
        uint64_t WNt = Moves::makeMove(WN, moves.substr(i, 4), 'N');
        uint64_t WBt = Moves::makeMove(WB, moves.substr(i, 4), 'B');
        uint64_t WRt = Moves::makeMove(WR, moves.substr(i, 4), 'R');
        uint64_t WQt = Moves::makeMove(WQ, moves.substr(i, 4), 'Q');
        uint64_t WKt = Moves::makeMove(WK, moves.substr(i, 4), 'K');
        uint64_t BPt = Moves::makeMove(BP, moves.substr(i, 4), 'p');
        uint64_t BNt = Moves::makeMove(BN, moves.substr(i, 4), 'n');
        uint64_t BBt = Moves::makeMove(BB, moves.substr(i, 4), 'b');
        uint64_t BRt = Moves::makeMove(BR, moves.substr(i, 4), 'r');
        uint64_t BQt = Moves::makeMove(BQ, moves.substr(i, 4), 'q');
        uint64_t BKt = Moves::makeMove(BK, moves.substr(i, 4), 'k');

        uint64_t EPt = Moves::makeMoveEP(WP | BP, moves.substr(i, 4));
        WRt = Moves::makeMoveCastle(WR, WK | BK, moves.substr(i, 4), 'R');
        BRt = Moves::makeMoveCastle(BR, WK | BK, moves.substr(i, 4), 'r');
        bool CWKt = CWK, CWQt = CWQ, CBKt = CBK, CBQt = CBQ;

        // Update castle flags based on move
        // ...

        if ((WKt & Moves::unsafeForWhite(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt,
                                         BBt, BRt, BQt, BKt)) == 0 &&
                WhiteToMove ||
            (BKt & Moves::unsafeForBlack(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt,
                                         BBt, BRt, BQt, BKt)) == 0 &&
                !WhiteToMove) {
            perft(WPt, WNt, WBt, WRt, WQt, WKt, BPt, BNt, BBt, BRt, BQt, BKt,
                  EPt, CWKt, CWQt, CBKt, CBQt, !WhiteToMove, depth + 1);
            // std::cout << moveToAlgebra(moves.substr(i, 4)) << " "
            //           << perftMoveCounter << std::endl;
            perftTotalMoveCounter += perftMoveCounter;
            perftMoveCounter = 0;
        }
    }
}
