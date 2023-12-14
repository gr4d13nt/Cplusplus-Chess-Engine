#include "moves.h"
#include "perft.h"
#include "representation.h"

int main() {
    Position p;
    p.initiateStandardChess();
    perft(p.WP, p.WN, p.WB, p.WR, p.WQ, p.WK, p.BP, p.BN, p.BB, p.BR, p.BQ,
              p.BK, p.EP, p.white_kingside, p.white_queenside, p.black_kingside,
              p.black_queenside, true, 0);
    cout << perftMoveCounter << endl;
    // U64 baord = Moves::makeMoveCastle(p.WR, p.WK, "7472", 'R');
    // U64 king = Moves::makeMove(p.WK, "7475", 'K');
    // Position::drawArray(p.WP, p.WN, p.WB, p.WR, p.WQ, p.WK, p.BP, p.BN, p.BB,
                        // p.BR, p.BQ, p.BK);
}