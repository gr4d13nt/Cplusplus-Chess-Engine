#ifndef MOVES_H
#define MOVES_H

#include "representation.h"
#include <bitset>
#include <iostream>
#include <string>

#define U64 u_int64_t

using namespace std;

U64 reverse(U64 n);

class Moves {
  public:
    static const U64 FILE_A;
    static const U64 FILE_B;
    static const U64 FILE_C;
    static const U64 FILE_D;
    static const U64 FILE_E;
    static const U64 FILE_F;
    static const U64 FILE_G;
    static const U64 FILE_H;
    static const U64 FILE_AB;
    static const U64 FILE_GH;
    static const U64 RANK_8;
    static const U64 RANK_7;
    static const U64 RANK_6;
    static const U64 RANK_5;
    static const U64 RANK_4;
    static const U64 RANK_3;
    static const U64 RANK_2;
    static const U64 RANK_1;

    static U64 CANT_CAPTURE;
    static U64 MY_PIECES;
    static U64 EMPTY;
    static U64 OCCUPIED;
    static U64 CASTLE_ROOKS[4];

    static U64 KING_SPAN;
    static U64 KNIGHT_SPAN;

    static const U64 RankMasks8[8];
    static const U64 FileMasks8[8];
    static const U64 DiagonalMasks8[15];
    static const U64 AntiDiagonalMasks8[15];

    static U64 makeMove(U64 board, const string& move, char type);

    static uint64_t makeMoveCastle(uint64_t rookBoard, uint64_t kingBoard,
                                   const string& move, char type);

    static uint64_t makeMoveEP(uint64_t board, const std::string& move);

    static U64 HAndVMoves(int s, U64 OCCUPIED);

    static U64 DAndAntiDMoves(int s, U64 OCCUPIED);

    static string possibleMovesW(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                                 U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                                 U64 EP, bool white_kingside,
                                 bool white_queenside, bool black_kingside,
                                 bool black_queenside);

    static string possibleMovesB(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                                 U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                                 U64 EP, bool white_kingside,
                                 bool white_queenside, bool black_kingside,
                                 bool black_queenside);

    static string possiblePW(U64 WP, U64 BP, U64 EP);
 
    static string possibleBP(U64 BP, U64 WP, U64 EP);

    static string possibleB(U64 OCCUPIED, U64 B);

    static string possibleR(U64 OCCUPIED, U64 R);

    static string possibleQ(U64 OCCUPIED, U64 Q);

    static string possibleK(U64 OCCUPIED, U64 K);

    static string possibleN(U64 OCCUPIED, U64 N);

    static string possibleCW(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                       U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                       bool CWK, bool CWQ);

    static string possibleCB(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                       U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                       bool CWK, bool CWQ);

    static uint64_t unsafeForBlack(uint64_t WP, uint64_t WN, uint64_t WB,
                                   uint64_t WR, uint64_t WQ, uint64_t WK,
                                   uint64_t BP, uint64_t BN, uint64_t BB,
                                   uint64_t BR, uint64_t BQ, uint64_t BK);

    static void drawBitboard(uint64_t bitBoard);

    static U64 unsafeForWhite(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                              U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK);
};

#endif // MOVES_H