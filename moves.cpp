#include "moves.h"

string Piece::POSSIBLE_MOVES;
U64 Piece::UNSAFE;
U64 Piece::CANT_CAPTURE;
U64 Piece::OCCUPIED;

const U64 Piece::DiagonalMasks8[15] = {
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
	0x8000000000000000L
};

const U64 Piece::AntiDiagonalMasks8[15] = {
	0x80L,
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
	0x100000000000000L
};

const U64 Piece::FILE_AB = FileMasks8[FA] | FileMasks8[FB];
const U64 Piece::FILE_GH = FileMasks8[FG] | FileMasks8[FH];

//Bishop/Knight/Rook/Queen
void Piece::Possible(U64 board, COLOR color)
{
	while (true)
	{
		U64 i = (board & ~(board - 1));
		if (i == 0)
			break;

		ToString(i);

		board &= ~i;
	}
}

void Piece::ToString(U64 board)
{
	int location = __builtin_ctzll(board);

	U64 possibility = Possibility(location) & CANT_CAPTURE;
	
	while (true)
	{
		U64 j = (possibility & ~(possibility - 1));
		if (j == 0)
			break;
		
		int index = __builtin_ctzll(j);

		POSSIBLE_MOVES +=
			to_string(location / Eight) + //Start
			to_string(location % Eight) +
			to_string(index / Eight) +	  //End
			to_string(index % Eight);

		possibility &= ~j;
	}
}

//Bishop/Knight/Rook
void Piece::Attack(U64 board, U64 boaredQueen, COLOR color)
{
	if (_attackWithQueen)
		board |= boaredQueen;

	while (true)
	{
		U64 i = (board & ~(board - 1));
		if (i == 0)
			break;
		
		UNSAFE |= Possibility(__builtin_ctzll(i));

		board &= ~i;
	}
}

U64 Piece::HAndVMoves(int s)
{
	int col = s % Eight;
	U64 binaryS = 1ULL << s;
	U64 possibilitiesHorizontal =
		(OCCUPIED - 2 * binaryS) ^
		reverse(reverse(OCCUPIED) - 2 * reverse(binaryS));
	U64 possibilitiesVertical =
		((OCCUPIED & FileMasks8[col]) - (2 * binaryS)) ^
		reverse(reverse(OCCUPIED & FileMasks8[col]) -
			(2 * reverse(binaryS)));
	return (possibilitiesHorizontal & RankMasks8[s / Eight]) |
		(possibilitiesVertical & FileMasks8[col]);
}

U64 Piece::DAndAntiDMoves(int s)
{
	int row = s / Eight;
	int col = s % Eight;
	
	U64 binaryS = 1ULL << s;
	U64 possibilitiesDiagonal =
		((OCCUPIED & DiagonalMasks8[row + col]) - (2 * binaryS)) ^
		reverse(reverse(OCCUPIED & DiagonalMasks8[row + col]) -
			(2 * reverse(binaryS)));
	U64 possibilitiesAntiDiagonal =
		((OCCUPIED & AntiDiagonalMasks8[row + 7 - col]) -
			(2 * binaryS)) ^
		reverse(reverse(OCCUPIED & AntiDiagonalMasks8[row + 7 - col]) -
			(2 * reverse(binaryS)));
	return (possibilitiesDiagonal & DiagonalMasks8[row + col]) |
		(possibilitiesAntiDiagonal &
			AntiDiagonalMasks8[row + 7 - col]);
}

U64 Piece::reverse(U64 n)
{
	// Reverse the bits in a 64-bit integer
	n = ((n >> 1) & 0x5555555555555555ULL) | ((n & 0x5555555555555555ULL) << 1);
	n = ((n >> 2) & 0x3333333333333333ULL) | ((n & 0x3333333333333333ULL) << 2);
	n = ((n >> 4) & 0x0F0F0F0F0F0F0F0FULL) | ((n & 0x0F0F0F0F0F0F0F0FULL) << 4);
	n = ((n >> 8) & 0x00FF00FF00FF00FFULL) | ((n & 0x00FF00FF00FF00FFULL) << Eight);
	n = ((n >> 16) & 0x0000FFFF0000FFFFULL) |
		((n & 0x0000FFFF0000FFFFULL) << 16);
	return (n >> 32) | (n << 32);
}

//////////////////////////Pawn//////////////////////////////////
U64 Pawn::MY_PIECES;
U64 Pawn::RivalP_EP;

void Pawn::Possible(U64 board, COLOR color)
{
	U64 EMPTY = ~OCCUPIED;
	char sign = (color == WC) ? 1 : -1;
	bool isR = (color == WC);
	const U64* rf = RankFileMask[color];
	const string* pp = PP[color];
	
	//Capture right 
	Regular(Shift(board, isR, 7) & rf[0] & MY_PIECES, sign, -1 * sign);

	//Capture left
	Regular(Shift(board, isR, 9) & rf[1] & MY_PIECES, sign, sign);

	//Move 1 forward
	Regular(Shift(board, isR, 8) & rf[2] & EMPTY, sign);

	//Move 2 forward
	Regular(Shift(board, isR, 16) & rf[3] & EMPTY & Shift(EMPTY, isR, 8), 2 * sign);

	//Promotion by capture right
	Promotion(Shift(board, isR, 7) & rf[4] & MY_PIECES, pp, -1 * sign);

	//Promotion by capture left
	Promotion(Shift(board, isR, 9) & rf[5] & MY_PIECES, pp, sign);

	//Promotion by move 1 forward
	Promotion(Shift(board, isR, 8) & rf[6] & EMPTY, pp);

	//En passant right. Shows piece to remove, not the dest
	Enpassant(Shift(board, 1 - isR, 1) & rf[7] & RivalP_EP, EP[color], -1 * sign);

	//En passant left. Shows piece to remove, not the dest
	Enpassant(Shift(board, isR, 1) & rf[8] & RivalP_EP, EP[color], sign);
}

void Pawn::Regular(U64 PAWN_MOVES, int add1, int add2)
{
	while (true)
	{
		U64 possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
		if (possibility == 0)
			break;
		
		int index = __builtin_ctzll(possibility);
		int row = (index / Eight);
		int col = (index % Eight);
		
		POSSIBLE_MOVES +=
			to_string(row + add1) +
			to_string(col + add2) +
			to_string(row) +
			to_string(col);

		PAWN_MOVES &= ~possibility;
	}
}

void Pawn::Promotion(U64 PAWN_MOVES, const string* pp, int add)
{
	while (true)
	{
		U64 possibility = PAWN_MOVES & ~(PAWN_MOVES - 1);
		if (possibility == 0)
			break;
		
		int col = __builtin_ctzll(possibility) % Eight;

		string baseMove = 
			to_string(col + add) + 
			to_string(col);
		
		POSSIBLE_MOVES +=
			baseMove + pp[0] +
			baseMove + pp[1] +
			baseMove + pp[2] +
			baseMove + pp[3];

		PAWN_MOVES &= ~possibility;
	}
}

void Pawn::Enpassant(U64 PAWN_MOVES, const string enpassant, int add)
{
	if (PAWN_MOVES == 0)
		return;

	int col = __builtin_ctzll(PAWN_MOVES) % Eight;
	
	POSSIBLE_MOVES += 
		to_string(col + add) + 
		to_string(col) + enpassant;
}

/////////////////////////END////////////////////////////////////