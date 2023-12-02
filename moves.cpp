#include "moves.h"
#include <bitset>
#include <iostream>
#include <string>

using namespace std;

U64 reverse(U64 n) {
    // Reverse the bits in a 64-bit integer
    n = ((n >> 1) & 0x5555555555555555ULL) | ((n & 0x5555555555555555ULL) << 1);
    n = ((n >> 2) & 0x3333333333333333ULL) | ((n & 0x3333333333333333ULL) << 2);
    n = ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((n & 0x0F0F0F0F0F0F0F0FULL) << 4);
    n = ((n >> 8) & 0x00FF00FF00FF00FFULL) | ((n & 0x00FF00FF00FF00FFULL) << 8);
    n = ((n >> 16) & 0x0000FFFF0000FFFFULL) |
        ((n & 0x0000FFFF0000FFFFULL) << 16);
    return (n >> 32) | (n << 32);
}

const U64 Moves::FILE_A = 72340172838076673;
const U64 Moves::FILE_B = 144680345676153346;
const U64 Moves::FILE_C = 289360691352306692;
const U64 Moves::FILE_D = 578721382704613384;
const U64 Moves::FILE_E = 1157442765409226768;
const U64 Moves::FILE_F = 2314885530818453536;
const U64 Moves::FILE_G = 4629771061636907072;
const U64 Moves::FILE_H = 9259542123273814144ULL;
const U64 Moves::FILE_AB = 217020518514230019ULL;
const U64 Moves::FILE_GH = 13889313184910721216ULL;
const U64 Moves::RANK_8 = 255;
const U64 Moves::RANK_7 = 65280;
const U64 Moves::RANK_6 = 16711680;
const U64 Moves::RANK_5 = 4278190080;
const U64 Moves::RANK_4 = 1095216660480;
const U64 Moves::RANK_3 = 280375465082880;
const U64 Moves::RANK_2 = 71776119061217280;
const U64 Moves::RANK_1 = 18374686479671623680ULL;

U64 Moves::CANT_CAPTURE = 0;
U64 Moves::MY_PIECES = 0;
U64 Moves::EMPTY = 0;
U64 Moves::CASTLE_ROOKS[4] = {63, 56, 7, 0};

U64 Moves::OCCUPIED = 0;

U64 Moves::KING_SPAN = 460039L;
U64 Moves::KNIGHT_SPAN = 43234889994L;

const U64 Moves::RankMasks8[8] = {
    0xFFL,         0xFF00L,         0xFF0000L,         0xFF000000L,
    0xFF00000000L, 0xFF0000000000L, 0xFF000000000000L, 0xFF00000000000000L};

const U64 Moves::FileMasks8[8] = {0x101010101010101L,  0x202020202020202L,
                                  0x404040404040404L,  0x808080808080808L,
                                  0x1010101010101010L, 0x2020202020202020L,
                                  0x4040404040404040L, 0x8080808080808080L};

const U64 Moves::DiagonalMasks8[15] = {
    0x1L, // diagonals go from bottom left to top right
    0x102L,
    0x10204L,
    0x1020408L,
    0x102040810L,
    0x10204081020L,
    0x1020408102040L,
    0x102040810204080L,
    0x204081020408000L,
    0x408102040800000L,
    0x810204080000000L,
    0x1020408000000000L,
    0x2040800000000000L,
    0x4080000000000000L,
    0x8000000000000000L};

const U64 Moves::AntiDiagonalMasks8[15] = {0x80L,
                                           0x8040L,
                                           0x804020L,
                                           0x80402010L,
                                           0x8040201008L,
                                           0x804020100804L,
                                           0x80402010080402L,
                                           0x8040201008040201L,
                                           0x4020100804020100L,
                                           0x2010080402010000L,
                                           0x1008040201000000L,
                                           0x804020100000000L,
                                           0x402010000000000L,
                                           0x201000000000000L,
                                           0x100000000000000L};

U64 Moves::makeMove(U64 board, const string& move, char type) {
    if (isdigit(move[3])) { // 'regular' move
        int start = (move[0] - '0') * 8 + (move[1] - '0');
        int end = (move[2] - '0') * 8 + (move[3] - '0');
        if ((board >> start) & 1) {
            board &= ~(1ULL << start);
            board |= (1ULL << end);
        }
        else {
            board &= ~(1ULL << end);
        }
    }
    else if (move[3] == 'P') { // pawn promotion
        int start, end;
        if (isupper(move[2])) {
            start = (move[0] - '0') * 8 + 6;
            end = (move[1] - '0') * 8 + 7;
        }
        else {
            start = (move[0] - '0') * 8 + 1;
            end = (move[1] - '0') * 8 + 0;
        }
        if (type == move[2]) {
            board |= (1ULL << end);
        }
        else {
            board &= ~(1ULL << start);
            board &= ~(1ULL << end);
        }
    }
    else if (move[3] == 'E') { // en passant
        int start, end;
        if (move[2] == 'W') {
            start = (move[0] - '0') * 8 + 3;
            end = (move[1] - '0') * 8 + 2;
            board &= ~(1ULL << ((move[1] - '0') * 8 + 3));
        }
        else {
            start = (move[0] - '0') * 8 + 4;
            end = (move[1] - '0') * 8 + 5;
            board &= ~(1ULL << ((move[1] - '0') * 8 + 4));
        }
        if ((board >> start) & 1) {
            board &= ~(1ULL << start);
            board |= (1ULL << end);
        }
    }
    else {
        std::cerr << "ERROR: Invalid move type" << std::endl;
    }
    return board;
}

uint64_t Moves::makeMoveCastle(uint64_t rookBoard, uint64_t kingBoard,
                               const string& move, char type) {
    int start = (move[0] - '0') * 8 + (move[1] - '0');
    if (((kingBoard >> start) & 1) == 1 && (move == "0402" || move == "0406" ||
                                            move == "7472" || move == "7476")) {
        if (type == 'R') {
            if (move == "7472") {
                rookBoard &= ~(1ULL << CASTLE_ROOKS[1]);
                rookBoard |= (1ULL << (CASTLE_ROOKS[1] + 3));
            }
            else if (move == "7476") {
                rookBoard &= ~(1ULL << CASTLE_ROOKS[0]);
                rookBoard |= (1ULL << (CASTLE_ROOKS[0] - 2));
            }
        }
        else {
            if (move == "0402") {
                rookBoard &= ~(1ULL << CASTLE_ROOKS[3]);
                rookBoard |= (1ULL << (CASTLE_ROOKS[3] + 3));
            }
            else if (move == "0406") {
                rookBoard &= ~(1ULL << CASTLE_ROOKS[2]);
                rookBoard |= (1ULL << (CASTLE_ROOKS[2] - 2));
            }
        }
    }
    return rookBoard;
}

uint64_t Moves::makeMoveEP(uint64_t board, const std::string& move) {
    if (isdigit(move[3])) {
        int start = (move[0] - '0') * 8 + (move[1] - '0');
        if (abs(move[0] - move[2]) == 2 && ((board >> start) & 1)) {
            // Pawn double push detected, calculate en passant target square
            return FileMasks8[move[1] - '0'];
        }
    }
    return 0;
}

U64 Moves::HAndVMoves(int s, U64 OCCUPIED) {
    U64 binaryS = 1ULL << s;
    U64 possibilitiesHorizontal =
        (OCCUPIED - 2 * binaryS) ^
        reverse(reverse(OCCUPIED) - 2 * reverse(binaryS));
    U64 possibilitiesVertical =
        ((OCCUPIED & Moves::FileMasks8[s % 8]) - (2 * binaryS)) ^
        reverse(reverse(OCCUPIED & Moves::FileMasks8[s % 8]) - (2 * reverse(binaryS)));
    return (possibilitiesHorizontal & Moves::RankMasks8[s / 8]) |
           (possibilitiesVertical & Moves::FileMasks8[s % 8]);
}

U64 Moves::DAndAntiDMoves(int s, U64 OCCUPIED) {
    U64 binaryS = 1ULL << s;
    U64 possibilitiesDiagonal =
        ((OCCUPIED & DiagonalMasks8[(s / 8) + (s % 8)]) - (2 * binaryS)) ^
        reverse(reverse(OCCUPIED & DiagonalMasks8[(s / 8) + (s % 8)]) -
                (2 * reverse(binaryS)));
    U64 possibilitiesAntiDiagonal =
        ((OCCUPIED & AntiDiagonalMasks8[(s / 8) + 7 - (s % 8)]) -
         (2 * binaryS)) ^
        reverse(reverse(OCCUPIED & AntiDiagonalMasks8[(s / 8) + 7 - (s % 8)]) -
                (2 * reverse(binaryS)));
    return (possibilitiesDiagonal & DiagonalMasks8[(s / 8) + (s % 8)]) |
           (possibilitiesAntiDiagonal &
            AntiDiagonalMasks8[(s / 8) + 7 - (s % 8)]);
}

string Moves::possibleMovesW(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                             U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                             U64 EP, bool white_kingside, bool white_queenside,
                             bool black_kingside, bool black_queenside) {
    CANT_CAPTURE = ~(WP | WN | WB | WR | WQ | WK |
                     BK);               // Added BK to avoid illegal capture
    MY_PIECES = BP | BN | BB | BR | BQ; // Omitted BK to avoid illegal capture
    OCCUPIED = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;
    EMPTY = ~OCCUPIED;

    // Assuming timeExperiment and other move functions are defined
    // elsewhere
    string list = possiblePW(WP, BP, EP) + possibleB(OCCUPIED, WB) +
                  possibleR(OCCUPIED, WR) + possibleQ(OCCUPIED, WQ) +
                  possibleK(OCCUPIED, WK) + possibleN(OCCUPIED, WN) +
                  possibleCW(WR, white_kingside, white_queenside);
    // unsafeForBlack(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK);
    return list;
}

string Moves::possibleMovesB(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                             U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK,
                             U64 EP, bool white_kingside, bool white_queenside,
                             bool black_kingside, bool black_queenside) {
    CANT_CAPTURE = ~(BP | BN | BB | BR | BQ | BK |
                     WK);               // Added WK to avoid illegal capture
    MY_PIECES = WP | WN | WB | WR | WQ; // Omitted WK to avoid illegal capture
    OCCUPIED = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;
    EMPTY = ~OCCUPIED;

    string list = possibleBP(BP, WP, EP) + possibleB(OCCUPIED, BB) +
                  possibleR(OCCUPIED, BR) + possibleQ(OCCUPIED, BQ) +
                  possibleK(OCCUPIED, BK) + possibleN(OCCUPIED, BN) +
                  possibleCB(BR, black_kingside, black_queenside);
    // unsafeForWhite(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK);
    return list;
}

string Moves::possiblePW(U64 WP, U64 BP, U64 EP) {
    string list = "";
    U64 PAWN_MOVES;
    U64 possibility;

    PAWN_MOVES = (WP >> 7) & MY_PIECES & ~RANK_8 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 + 1) + to_string(index % 8 - 1) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    PAWN_MOVES = (WP >> 9) & MY_PIECES & ~RANK_8 & ~FILE_H;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 + 1) + to_string(index % 8 + 1) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Move 1 forward
    PAWN_MOVES = (WP >> 8) & EMPTY & ~RANK_8;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 + 1) + to_string(index % 8) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Move 2 forward
    PAWN_MOVES = (WP >> 16) & EMPTY & (EMPTY >> 8) & RANK_4;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 + 2) + to_string(index % 8) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by capture right
    PAWN_MOVES = (WP >> 7) & MY_PIECES & RANK_8 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        string baseMove = to_string(index % 8 - 1) + to_string(index % 8);
        list += baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove +
                "NP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by capture left
    PAWN_MOVES = (WP >> 9) & MY_PIECES & RANK_8 & ~FILE_H;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        string baseMove = to_string(index % 8 + 1) + to_string(index % 8);
        list += baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove +
                "NP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by move 1 forward
    PAWN_MOVES = (WP >> 8) & EMPTY & RANK_8;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        string baseMove = to_string(index % 8) + to_string(index % 8);
        list += baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove +
                "NP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    possibility = (WP << 1) & BP & RANK_5 & ~FILE_A &
                  EP; // Shows piece to remove, not the destination
    if (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index % 8 - 1) + to_string(index % 8) + " E";
    }

    // En passant left
    possibility = (WP >> 1) & BP & RANK_5 & ~FILE_H &
                  EP; // Shows piece to remove, not the destination
    if (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index % 8 + 1) + to_string(index % 8) + " E";
    }

    return list;
}

string Moves::possibleBP(U64 BP, U64 WP, U64 EP) {
    string list = "";
    U64 PAWN_MOVES;
    U64 possibility;

    // Capture right
    PAWN_MOVES = (BP << 7) & MY_PIECES & ~RANK_1 & ~FILE_H;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 - 1) + to_string(index % 8 + 1) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Capture left
    PAWN_MOVES = (BP << 9) & MY_PIECES & ~RANK_1 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 - 1) + to_string(index % 8 - 1) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Move 1 forward
    PAWN_MOVES = (BP << 8) & EMPTY & ~RANK_1;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 - 1) + to_string(index % 8) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Move 2 forward
    PAWN_MOVES = (BP << 16) & EMPTY & (EMPTY << 8) & RANK_5;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index / 8 - 2) + to_string(index % 8) +
                to_string(index / 8) + to_string(index % 8);
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by capture right
    PAWN_MOVES = (BP << 7) & MY_PIECES & RANK_1 & ~FILE_H;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += "" + to_string(index % 8 + 1) + to_string(index % 8) + "qP" +
                to_string(index % 8 + 1) + to_string(index % 8) + "rP" +
                to_string(index % 8 + 1) + to_string(index % 8) + "bP" +
                to_string(index % 8 + 1) + to_string(index % 8) + "nP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by capture left
    PAWN_MOVES = (BP << 9) & MY_PIECES & RANK_1 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += "" + to_string(index % 8 - 1) + to_string(index % 8) + "qP" +
                to_string(index % 8 - 1) + to_string(index % 8) + "rP" +
                to_string(index % 8 - 1) + to_string(index % 8) + "bP" +
                to_string(index % 8 - 1) + to_string(index % 8) + "nP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by move 1 forward
    PAWN_MOVES = (BP << 8) & EMPTY & RANK_1;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += "" + to_string(index % 8) + to_string(index % 8) + "qP" +
                to_string(index % 8) + to_string(index % 8) + "rP" +
                to_string(index % 8) + to_string(index % 8) + "bP" +
                to_string(index % 8) + to_string(index % 8) + "nP";
        PAWN_MOVES &= ~possibility;
        possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // En passant right
    possibility = (BP >> 1) & WP & RANK_4 & ~FILE_H &
                  EP; // Shows piece to remove, not the destination
    if (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index % 8 + 1) + to_string(index % 8) + "bE";
    }

    // En passant left
    possibility = (BP << 1) & WP & RANK_4 & ~FILE_A &
                  EP; // Shows piece to remove, not the destination
    if (possibility != 0) {
        int index = __builtin_ctzll(possibility);
        list += to_string(index % 8 - 1) + to_string(index % 8) + "bE";
    }

    return list;
}

string Moves::possibleB(U64 OCCUPIED, U64 B) {
    string list = "";
    U64 i = B & ~(B - 1);
    U64 possibility;

    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        possibility = DAndAntiDMoves(iLocation, OCCUPIED) & CANT_CAPTURE;
        U64 j = possibility & ~(possibility - 1);

        while (j != 0) {
            int index = __builtin_ctzll(j);
            list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                    to_string(index / 8) + to_string(index % 8);
            possibility &= ~j;
            j = possibility & ~(possibility - 1);
        }

        B &= ~i;
        i = B & ~(B - 1);
    }

    return list;
}

string Moves::possibleR(U64 OCCUPIED, U64 R) {
    string list = "";
    U64 i = R & ~(R - 1);
    U64 possibility;

    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        possibility = HAndVMoves(iLocation, OCCUPIED) & CANT_CAPTURE;
        U64 j = possibility & ~(possibility - 1);

        while (j != 0) {
            int index = __builtin_ctzll(j);
            list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                    to_string(index / 8) + to_string(index % 8);
            possibility &= ~j;
            j = possibility & ~(possibility - 1);
        }

        R &= ~i;
        i = R & ~(R - 1);
    }

    return list;
}

string Moves::possibleQ(U64 OCCUPIED, U64 Q) {
    string list = "";
    U64 i = Q & ~(Q - 1);
    U64 possibility;

    while (i != 0) {
        int iLocation = __builtin_ctzll(
            i); // Finds the position of the least significant bit set to 1.
        possibility = (HAndVMoves(iLocation, OCCUPIED) |
                       DAndAntiDMoves(iLocation, OCCUPIED)) &
                      CANT_CAPTURE;

        U64 j = possibility & ~(possibility - 1);
        while (j != 0) {
            int index = __builtin_ctzll(j);
            list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                    to_string(index / 8) + to_string(index % 8);

            possibility &= ~j;
            j = possibility & ~(possibility - 1);
        }

        Q &= ~i;
        i = Q & ~(Q - 1);
    }

    return list;
}

string Moves::possibleK(U64 OCCUPIED, U64 K) {
    string list = "";
    U64 possibility;
    int iLocation = __builtin_ctzll(K);

    if (iLocation > 9) {
        possibility = KING_SPAN << (iLocation - 9);
    }
    else {
        possibility = KING_SPAN >> (9 - iLocation);
    }

    if (iLocation % 8 < 4) {
        possibility &= ~FILE_GH & CANT_CAPTURE;
    }
    else {
        possibility &= ~FILE_AB & CANT_CAPTURE;
    }

    U64 j = possibility & ~(possibility - 1);
    while (j != 0) {
        int index = __builtin_ctzll(j);
        list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                to_string(index / 8) + to_string(index % 8);
        possibility &= ~j;
        j = possibility & ~(possibility - 1);
    }

    return list;
}

string Moves::possibleN(U64 OCCUPIED, U64 N) {
    string list = "";
    U64 i = N & ~(N - 1);
    U64 possibility;

    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        if (iLocation > 18) {
            possibility = KNIGHT_SPAN << (iLocation - 18);
        }
        else {
            possibility = KNIGHT_SPAN >> (18 - iLocation);
        }

        if (iLocation % 8 < 4) {
            possibility &= ~FILE_GH & CANT_CAPTURE;
        }
        else {
            possibility &= ~FILE_AB & CANT_CAPTURE;
        }

        U64 j = possibility & ~(possibility - 1);
        while (j != 0) {
            int index = __builtin_ctzll(j);
            list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                    to_string(index / 8) + to_string(index % 8);
            possibility &= ~j;
            j = possibility & ~(possibility - 1);
        }

        N &= ~i;
        i = N & ~(N - 1);
    }

    return list;
}

string Moves::possibleCW(U64 WR, bool CWK, bool CWQ) {
    string list = "";
    if (CWK && ((1ULL << CASTLE_ROOKS[0]) & WR) != 0) {
        list += "7476";
    }
    if (CWQ && ((1ULL << CASTLE_ROOKS[1]) & WR) != 0) {
        list += "7472";
    }
    return list;
}

string Moves::possibleCB(U64 BR, bool CBK, bool CBQ) {
    string list = "";
    if (CBK && ((1ULL << CASTLE_ROOKS[2]) & BR) != 0) {
        list += "0406";
    }
    if (CBQ && ((1ULL << CASTLE_ROOKS[3]) & BR) != 0) {
        list += "0402";
    }
    return list;
}

uint64_t Moves::unsafeForBlack(uint64_t WP, uint64_t WN, uint64_t WB,
                               uint64_t WR, uint64_t WQ, uint64_t WK,
                               uint64_t BP, uint64_t BN, uint64_t BB,
                               uint64_t BR, uint64_t BQ, uint64_t BK) {
    uint64_t unsafe;
    uint64_t OCCUPIED =
        WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;

    // Pawn attacks
    unsafe = ((WP >> 7) & ~FILE_A);  // Pawn capture right
    unsafe |= ((WP >> 9) & ~FILE_H); // Pawn capture left

    uint64_t possibility;

    // Knight attacks
    uint64_t i = WN & ~(WN - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        if (iLocation > 18) {
            possibility = KNIGHT_SPAN << (iLocation - 18);
        }
        else {
            possibility = KNIGHT_SPAN >> (18 - iLocation);
        }
        if (iLocation % 8 < 4) {
            possibility &= ~FILE_GH;
        }
        else {
            possibility &= ~FILE_AB;
        }
        unsafe |= possibility;
        WN &= ~i;
        i = WN & ~(WN - 1);
    }

    // Bishop/Queen attacks
    uint64_t QB = WQ | WB;
    i = QB & ~(QB - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        possibility = DAndAntiDMoves(iLocation, OCCUPIED);
        unsafe |= possibility;
        QB &= ~i;
        i = QB & ~(QB - 1);
    }

    // Rook/Queen attacks
    uint64_t QR = WQ | WR;
    i = QR & ~(QR - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        possibility = HAndVMoves(iLocation, OCCUPIED);
        unsafe |= possibility;
        QR &= ~i;
        i = QR & ~(QR - 1);
    }

    // King attacks
    int kingLocation = __builtin_ctzll(WK);
    if (kingLocation > 9) {
        possibility = KING_SPAN << (kingLocation - 9);
    }
    else {
        possibility = KING_SPAN >> (9 - kingLocation);
    }
    if (kingLocation % 8 < 4) {
        possibility &= ~FILE_GH;
    }
    else {
        possibility &= ~FILE_AB;
    }
    unsafe |= possibility;

    // Optionally print the unsafe squares
    // drawBitboard(unsafe);

    return unsafe;
}

void Moves::drawBitboard(uint64_t bitBoard) {
    string chessBoard[8][8];

    // Initialize the chessboard array
    for (int i = 0; i < 64; ++i) {
        chessBoard[i / 8][i % 8] = " ";
    }

    // Mark the squares with 'P' where bits are set in bitBoard
    for (int i = 0; i < 64; ++i) {
        if ((bitBoard >> i) & 1) {
            chessBoard[i / 8][i % 8] = "P";
        }
    }

    // Print the chessboard
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            cout << chessBoard[i][j];
        }
        cout << endl;
    }
}

U64 Moves::unsafeForWhite(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK,
                          U64 BP, U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK) {
    U64 unsafe = 0;
    U64 OCCUPIED = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;

    // Pawn attacks
    unsafe |= ((BP << 7) & ~FILE_H); // Pawn capture right
    unsafe |= ((BP << 9) & ~FILE_A); // Pawn capture left

    // Knight attacks
    U64 i = BN & ~(BN - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        U64 possibility;
        if (iLocation > 18) {
            possibility = KNIGHT_SPAN << (iLocation - 18);
        }
        else {
            possibility = KNIGHT_SPAN >> (18 - iLocation);
        }
        if (iLocation % 8 < 4) {
            possibility &= ~FILE_GH;
        }
        else {
            possibility &= ~FILE_AB;
        }
        unsafe |= possibility;
        BN &= ~i;
        i = BN & ~(BN - 1);
    }

    // Bishop/Queen attacks
    U64 QB = BQ | BB;
    i = QB & ~(QB - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        U64 possibility = DAndAntiDMoves(iLocation, OCCUPIED);
        unsafe |= possibility;
        QB &= ~i;
        i = QB & ~(QB - 1);
    }

    // Rook/Queen attacks
    U64 QR = BQ | BR;
    i = QR & ~(QR - 1);
    while (i != 0) {
        int iLocation = __builtin_ctzll(i);
        U64 possibility = HAndVMoves(iLocation, OCCUPIED);
        unsafe |= possibility;
        QR &= ~i;
        i = QR & ~(QR - 1);
    }

    // King attacks
    int kingLocation = __builtin_ctzll(BK);
    U64 possibility;
    if (kingLocation > 9) {
        possibility = KING_SPAN << (kingLocation - 9);
    }
    else {
        possibility = KING_SPAN >> (9 - kingLocation);
    }
    if (kingLocation % 8 < 4) {
        possibility &= ~FILE_GH;
    }
    else {
        possibility &= ~FILE_AB;
    }
    unsafe |= possibility;

    return unsafe;
}

// long long int perft(int depth, Position p) {
//     if (depth == 0) {
//         return 1;
//     }
//     long long int nodes = 0;
//     string moves =
//         possibleMovesW(p.WP, p.WN, p.WB, p.WR, p.WQ, p.WK, p.BP, p.BN, p.BB,
//                        p.BR, p.BQ, p.BK, p.EP, p.white_kingside,
//                        p.white_queenside, p.black_kingside, p.black_queenside);
//     for (int i = 0; i < moves.size(); i += 4) {
//         Position p2 = makeMove(p, moves.substr(i, 4), moves[i + 4]);
//         nodes += perft(depth - 1, p2);
//     }
//     return nodes;
// }
