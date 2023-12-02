#include "representation.h"
#include "moves.h"
#include "perft.h"

int main() {
    Position p;
    p.initiateStandardChess();

    perftRoot(p.WP, p.WN, p.WB, p.WR, p.WQ, p.WK, p.BP, p.BN, p.BB, p.BR, p.BQ,
              p.BK, p.EP, p.white_kingside, p.white_queenside, p.black_kingside,
              p.black_queenside, true, 0);
    cout << perftTotalMoveCounter << endl;
}