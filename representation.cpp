#include <iostream>
#include <string>
#include <vector>
#include "representation.h"

using namespace std;

// string fen;
// Position(string fen);
// void print();

void Position::initiateStandardChess() {
    char chessBoard[8][8] = {{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
                             {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
                             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                             {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
                             {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

    // char chessBoard[8][8] = {{' ', ' ', ' ', ' ', 'k', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //                          {'R', ' ', ' ', ' ', 'K', ' ', ' ', ' '}};
    arrayToBitboards(chessBoard, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ,
                     BK);
}

void Position::arrayToBitboards(char chessBoard[8][8], U64& WP, U64& WN, U64& WB,
                             U64& WR, U64& WQ, U64& WK, U64& BP, U64& BN,
                             U64& BB, U64& BR, U64& BQ, U64& BK) {
    string Binary;
    for (int i = 0; i < 64; i++) {
        Binary =
            "0000000000000000000000000000000000000000000000000000000000000000";
        Binary = Binary.substr(i + 1) + "1" + Binary.substr(0, i);
        switch (chessBoard[i / 8][i % 8]) {
        case 'P':
            WP += convertStringToBitboard(Binary);
            break;
        case 'N':
            WN += convertStringToBitboard(Binary);
            break;
        case 'B':
            WB += convertStringToBitboard(Binary);
            break;
        case 'R':
            WR += convertStringToBitboard(Binary);
            break;
        case 'Q':
            WQ += convertStringToBitboard(Binary);
            break;
        case 'K':
            WK += convertStringToBitboard(Binary);
            break;
        case 'p':
            BP += convertStringToBitboard(Binary);
            break;
        case 'n':
            BN += convertStringToBitboard(Binary);
            break;
        case 'b':
            BB += convertStringToBitboard(Binary);
            break;
        case 'r':
            BR += convertStringToBitboard(Binary);
            break;
        case 'q':
            BQ += convertStringToBitboard(Binary);
            break;
        case 'k':
            BK += convertStringToBitboard(Binary);
            break;
        }
    }
    drawArray(WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK);
}

U64 Position::convertStringToBitboard(string binary) {
    U64 bitboard = 0;
    for (int i = 0; i < binary.length(); i++) {
        if (binary[i] == '1') {
            bitboard |= (1ULL << (binary.length() - 1 - i));
        }
    }
    return bitboard;
}

void Position::drawArray(U64 WP, U64 WN, U64 WB, U64 WR, U64 WQ, U64 WK, U64 BP,
                      U64 BN, U64 BB, U64 BR, U64 BQ, U64 BK) {
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            cout << endl; // New line at the end of each row
        }

        if (((WP >> i) & 1) == 1) {
            cout << 'P';
        }
        else if (((WN >> i) & 1) == 1) {
            cout << 'N';
        }
        else if (((WB >> i) & 1) == 1) {
            cout << 'B';
        }
        else if (((WR >> i) & 1) == 1) {
            cout << 'R';
        }
        else if (((WQ >> i) & 1) == 1) {
            cout << 'Q';
        }
        else if (((WK >> i) & 1) == 1) {
            cout << 'K';
        }
        else if (((BP >> i) & 1) == 1) {
            cout << 'p';
        }
        else if (((BN >> i) & 1) == 1) {
            cout << 'n';
        }
        else if (((BB >> i) & 1) == 1) {
            cout << 'b';
        }
        else if (((BR >> i) & 1) == 1) {
            cout << 'r';
        }
        else if (((BQ >> i) & 1) == 1) {
            cout << 'q';
        }
        else if (((BK >> i) & 1) == 1) {
            cout << 'k';
        }
        else {
            cout << '.'; // Empty square
        }
    }
    cout << endl;
}