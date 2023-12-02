#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include <iostream>
#include <string>
#include <vector>

// Define U64 as an alias for unsigned 64-bit integer type
#define U64 u_int64_t

class Position {
public:
  U64 bitboards[12]; // Array to store bitboards

  U64 WP = 0;
  U64 WN = 0;
  U64 WB = 0;
  U64 WR = 0;
  U64 WQ = 0;
  U64 WK = 0;
  U64 BP = 0;
  U64 BN = 0;
  U64 BB = 0;
  U64 BR = 0;
  U64 BQ = 0;
  U64 BK = 0;
  U64 EP = 0;

  bool side_to_move; // Indicates which side is to move
  bool white_kingside = true;
  bool white_queenside = true;
  bool black_kingside = true;
  bool black_queenside = true;
  int halfmove_clock;    // Halfmove clock for 50-move rule
  int fullmove_number;   // Fullmove number, incremented after black's move
  std::string fen;       // FEN string representing the position

  // Constructor to initialize Position with a FEN string
  // Position(std::string fen);

  // Method to print the position
  // void print();

  // Static methods related to chess position
  void initiateStandardChess();
  static void arrayToBitboards(char chessBoard[8][8], U64 &WP, U64 &WN, U64 &WB,
                               U64 &WR, U64 &WQ, U64 &WK, U64 &BP, U64 &BN,
                               U64 &BB, U64 &BR, U64 &BQ, U64 &BK);
  static U64 convertStringToBitboard(std::string binary);
  static void drawArray(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK, U64 BP,
                        U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK);
};

#endif // REPRESENTATION_H
