#include "moves.h"
#include "representation.h"
#include <cctype>
#include <iostream>
#include <string>

std::string moveToAlgebra(const std::string& move);

extern int perftTotalMoveCounter;
extern int perftMoveCounter;
extern int perftMaxDepth;

void perft(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK, U64 BP, U64 BN,
           U64 BB, U64 BR, U64 BQ, U64 BK, U64 EP, bool CWK, bool CWQ, bool CBK,
           bool CBQ, bool WhiteToMove, int depth);

void perftRoot(uint64_t WP, uint64_t WN, uint64_t WB, uint64_t WR,
                      uint64_t WQ, uint64_t WK, uint64_t BP, uint64_t BN,
                      uint64_t BB, uint64_t BR, uint64_t BQ, uint64_t BK,
                      uint64_t EP, bool CWK, bool CWQ, bool CBK, bool CBQ,
                      bool WhiteToMove, int depth);