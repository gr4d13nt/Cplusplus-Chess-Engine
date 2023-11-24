#include "representation.cpp"
#include <bitset>
#include <iostream>
#include <string>

#define U64 u_int64_t

using namespace std;

U64 reverse(U64 n) {
  // Reverse the bits in a 64-bit integer
  n = ((n >> 1) & 0x5555555555555555ULL) | ((n & 0x5555555555555555ULL) << 1);
  n = ((n >> 2) & 0x3333333333333333ULL) | ((n & 0x3333333333333333ULL) << 2);
  n = ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((n & 0x0F0F0F0F0F0F0F0FULL) << 4);
  n = ((n >> 8) & 0x00FF00FF00FF00FFULL) | ((n & 0x00FF00FF00FF00FFULL) << 8);
  n = ((n >> 16) & 0x0000FFFF0000FFFFULL) | ((n & 0x0000FFFF0000FFFFULL) << 16);
  return (n >> 32) | (n << 32);
}

class Moves {
public:
  static const U64 FILE_A = 72340172838076673;
  static const U64 FILE_B = 144680345676153346;
  static const U64 FILE_C = 289360691352306692;
  static const U64 FILE_D = 578721382704613384;
  static const U64 FILE_E = 1157442765409226768;
  static const U64 FILE_F = 2314885530818453536;
  static const U64 FILE_G = 4629771061636907072;
  static const U64 FILE_H = 9259542123273814144ULL;
  static const U64 FILE_AB = 217020518514230019ULL;
  static const U64 FILE_GH = 13889313184910721216ULL;
  static const U64 RANK_8 = 255;
  static const U64 RANK_7 = 65280;
  static const U64 RANK_6 = 16711680;
  static const U64 RANK_5 = 4278190080;
  static const U64 RANK_4 = 1095216660480;
  static const U64 RANK_3 = 280375465082880;
  static const U64 RANK_2 = 71776119061217280;
  static const U64 RANK_1 = 18374686479671623680ULL;

  static U64 WHITE_CANT_CAPTURE;
  static U64 BLACK_PIECES;
  static U64 EMPTY;
  static U64 OCCUPIED;

  static U64 KING_SPAN;
  static U64 KNIGHT_SPAN;

  static const U64 RankMasks8[8];
  static const U64 FileMasks8[8];
  static const U64 DiagonalMasks8[15];
  static const U64 AntiDiagonalMasks8[15];

  static U64 HAndVMoves(int s, U64 OCCUPIED) {
    U64 binaryS = 1ULL << s;
    U64 possibilitiesHorizontal =
        (OCCUPIED - 2 * binaryS) ^
        reverse(reverse(OCCUPIED) - 2 * reverse(binaryS));
    U64 possibilitiesVertical =
        ((OCCUPIED & FileMasks8[s % 8]) - (2 * binaryS)) ^
        reverse(reverse(OCCUPIED & FileMasks8[s % 8]) - (2 * reverse(binaryS)));
    return (possibilitiesHorizontal & RankMasks8[s / 8]) |
           (possibilitiesVertical & FileMasks8[s % 8]);
  }

  static U64 DAndAntiDMoves(int s, U64 OCCUPIED) {
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

  static string possibleMovesW(const string &history, U64 WP, U64 WN, U64 WB,
                               U64 WR, U64 WQ, U64 WK, U64 BP, U64 BN, U64 BB,
                               U64 BR, U64 BQ, U64 BK) {
    WHITE_CANT_CAPTURE = ~(WP | WN | WB | WR | WQ | WK |
                           BK); // Added BK to avoid illegal capture
    BLACK_PIECES =
        BP | BN | BB | BR | BQ; // Omitted BK to avoid illegal capture
    OCCUPIED = WP | WN | WB | WR | WQ | WK | BP | BN | BB | BR | BQ | BK;
    EMPTY = ~OCCUPIED;

    // Assuming timeExperiment and other move functions are defined elsewhere
    string list = possiblePW(history, WP, BP) + possibleWB(OCCUPIED, WB) +
                  possibleWR(OCCUPIED, WR) + possibleWQ(OCCUPIED, WQ) +
                  possibleWK(OCCUPIED, WK) + possibleWN(OCCUPIED, WN);
    unsafeForBlack(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK);
    cout << list << endl;
    return list;
  }

  static string possiblePW(string history, U64 WP, U64 BP) {
    string list = "";
    U64 PAWN_MOVES;
    U64 possibility;

    PAWN_MOVES = (WP >> 7) & BLACK_PIECES & ~RANK_8 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
      int index = __builtin_ctzll(possibility);
      list += to_string(index / 8 + 1) + to_string(index % 8 - 1) +
              to_string(index / 8) + to_string(index % 8);
      PAWN_MOVES &= ~possibility;
      possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    PAWN_MOVES = (WP >> 9) & BLACK_PIECES & ~RANK_8 & ~FILE_H;
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
    PAWN_MOVES = (WP >> 7) & BLACK_PIECES & RANK_8 & ~FILE_A;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
      int index = __builtin_ctzll(possibility);
      string baseMove = to_string(index % 8 - 1) + to_string(index % 8);
      list +=
          baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove + "NP";
      PAWN_MOVES &= ~possibility;
      possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by capture left
    PAWN_MOVES = (WP >> 9) & BLACK_PIECES & RANK_8 & ~FILE_H;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
      int index = __builtin_ctzll(possibility);
      string baseMove = to_string(index % 8 + 1) + to_string(index % 8);
      list +=
          baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove + "NP";
      PAWN_MOVES &= ~possibility;
      possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    // Pawn promotion by move 1 forward
    PAWN_MOVES = (WP >> 8) & EMPTY & RANK_8;
    possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    while (possibility != 0) {
      int index = __builtin_ctzll(possibility);
      string baseMove = to_string(index % 8) + to_string(index % 8);
      list +=
          baseMove + "QP" + baseMove + "RP" + baseMove + "BP" + baseMove + "NP";
      PAWN_MOVES &= ~possibility;
      possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
    }

    if (history.length() >= 4) {
      // Checking the last move for a two-square pawn move
      if ((history[history.length() - 1] == history[history.length() - 3]) &&
          abs(history[history.length() - 2] - history[history.length() - 4]) ==
              2) {

        int eFile = history[history.length() - 1] - '0';

        // En passant right
        possibility = (WP << 1) & BP & RANK_5 & ~FILE_A & FileMasks8[eFile];
        if (possibility != 0) {
          int index = __builtin_ctzll(possibility);
          list += to_string(index % 8 - 1) + to_string(index % 8) + " E";
        }

        // En passant left
        possibility = (WP >> 1) & BP & RANK_5 & ~FILE_H & FileMasks8[eFile];
        if (possibility != 0) {
          int index = __builtin_ctzll(possibility);
          list += to_string(index % 8 + 1) + to_string(index % 8) + " E";
        }
      }
    }

    return list;
  }

  static string possibleWB(U64 OCCUPIED, U64 WB) {
    string list = "";
    U64 i = WB & ~(WB - 1);
    U64 possibility;

    while (i != 0) {
      int iLocation = __builtin_ctzll(i);
      possibility = DAndAntiDMoves(iLocation, OCCUPIED) & WHITE_CANT_CAPTURE;
      U64 j = possibility & ~(possibility - 1);

      while (j != 0) {
        int index = __builtin_ctzll(j);
        list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                to_string(index / 8) + to_string(index % 8);
        possibility &= ~j;
        j = possibility & ~(possibility - 1);
      }

      WB &= ~i;
      i = WB & ~(WB - 1);
    }

    return list;
  }

  static string possibleWR(U64 OCCUPIED, U64 WR) {
    string list = "";
    U64 i = WR & ~(WR - 1);
    U64 possibility;

    while (i != 0) {
      int iLocation = __builtin_ctzll(i);
      possibility = HAndVMoves(iLocation, OCCUPIED) & WHITE_CANT_CAPTURE;
      U64 j = possibility & ~(possibility - 1);

      while (j != 0) {
        int index = __builtin_ctzll(j);
        list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                to_string(index / 8) + to_string(index % 8);
        possibility &= ~j;
        j = possibility & ~(possibility - 1);
      }

      WR &= ~i;
      i = WR & ~(WR - 1);
    }

    return list;
  }

  static string possibleWQ(U64 OCCUPIED, U64 WQ) {
    string list = "";
    U64 i = WQ & ~(WQ - 1);
    U64 possibility;

    while (i != 0) {
      int iLocation = __builtin_ctzll(
          i); // Finds the position of the least significant bit set to 1.
      possibility = (HAndVMoves(iLocation, OCCUPIED) |
                     DAndAntiDMoves(iLocation, OCCUPIED)) &
                    WHITE_CANT_CAPTURE;

      U64 j = possibility & ~(possibility - 1);
      while (j != 0) {
        int index = __builtin_ctzll(j);
        list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                to_string(index / 8) + to_string(index % 8);

        possibility &= ~j;
        j = possibility & ~(possibility - 1);
      }

      WQ &= ~i;
      i = WQ & ~(WQ - 1);
    }

    return list;
  }

  static string possibleWK(U64 OCCUPIED, U64 WK) {
    string list = "";
    U64 possibility;
    int iLocation = __builtin_ctzll(WK);

    if (iLocation > 9) {
      possibility = KING_SPAN << (iLocation - 9);
    } else {
      possibility = KING_SPAN >> (9 - iLocation);
    }

    if (iLocation % 8 < 4) {
      possibility &= ~FILE_GH & WHITE_CANT_CAPTURE;
    } else {
      possibility &= ~FILE_AB & WHITE_CANT_CAPTURE;
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

  static string possibleWN(U64 OCCUPIED, U64 WN) {
    string list = "";
    U64 i = WN & ~(WN - 1);
    U64 possibility;

    while (i != 0) {
      int iLocation = __builtin_ctzll(i);
      if (iLocation > 18) {
        possibility = KNIGHT_SPAN << (iLocation - 18);
      } else {
        possibility = KNIGHT_SPAN >> (18 - iLocation);
      }

      if (iLocation % 8 < 4) {
        possibility &= ~FILE_GH & WHITE_CANT_CAPTURE;
      } else {
        possibility &= ~FILE_AB & WHITE_CANT_CAPTURE;
      }

      U64 j = possibility & ~(possibility - 1);
      while (j != 0) {
        int index = __builtin_ctzll(j);
        list += to_string(iLocation / 8) + to_string(iLocation % 8) +
                to_string(index / 8) + to_string(index % 8);
        possibility &= ~j;
        j = possibility & ~(possibility - 1);
      }

      WN &= ~i;
      i = WN & ~(WN - 1);
    }

    return list;
  }

  static uint64_t unsafeForBlack(uint64_t WP, uint64_t WN, uint64_t WB,
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
      } else {
        possibility = KNIGHT_SPAN >> (18 - iLocation);
      }
      if (iLocation % 8 < 4) {
        possibility &= ~FILE_GH;
      } else {
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
    } else {
      possibility = KING_SPAN >> (9 - kingLocation);
    }
    if (kingLocation % 8 < 4) {
      possibility &= ~FILE_GH;
    } else {
      possibility &= ~FILE_AB;
    }
    unsafe |= possibility;

    // Optionally print the unsafe squares
    drawBitboard(unsafe);

    return unsafe;
  }

  static void drawBitboard(uint64_t bitBoard) {
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
};

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

U64 Moves::WHITE_CANT_CAPTURE = 0;
U64 Moves::BLACK_PIECES = 0;
U64 Moves::EMPTY = 0;

U64 Moves::OCCUPIED = 0;
int main() {
  Position p;
  p.initiateStandardChess();
  string history = "";
  Moves::possibleMovesW(history, p.WP, p.WN, p.WB, p.WR, p.WQ, p.WK, p.BP, p.BN,
                        p.BB, p.BR, p.BQ, p.BK);
}