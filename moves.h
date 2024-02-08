#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <cstdint>
// #include <intrin.h>

using namespace std;

#define U64 uint64_t

#define Eight 8
#define SixtyFour 64
#define B1 '1'
#define B0 '0'
#define SPACE ' '
#define EmptyString ""
#define MovePromotion 'P'
#define MoveEnpassant 'E'
#define MoveEnpassant_W 'W'
#define MoveEnpassant_B 'B'

enum COLOR 
{ 
	WC, BC, MAX_COLORS
};

const char Color[MAX_COLORS]
{
	'W', 'B'
};

enum BOARD_INDEX	//same order as Key & used to access DATA:Brd[color][] 
{ 
	P, B, R, Q, K, N, MAX_KEYS 
};

enum FLAG_INDEX		//used to access DATA : Flg[color][]
{
	CK, CQ, MAX_FLAGS
};

const char Key[MAX_COLORS][MAX_KEYS]
{ 
	{ 'P', 'B', 'R', 'Q', 'K', 'N' }, //WC
	{ 'p', 'b', 'r', 'q', 'k', 'n' }  //BC
};

struct DATA
{ //Use COLOR/PIECE_INDEX/FLAG_INDEX to access
	U64 EP_TargetSquare;
	bool Flg[MAX_COLORS][MAX_FLAGS];
	U64 Brd[MAX_COLORS][MAX_KEYS];
};

enum RANK_MASK_INDEX 
{ 
	R8, R7, R6, R5, R4, R3, R2, R1
};

const U64 RankMasks8[Eight] = 
{
	0xFFL,
	0xFF00L,
	0xFF0000L,
	0xFF000000L,
	0xFF00000000L,
	0xFF0000000000L,
	0xFF000000000000L,
	0xFF00000000000000L
};

enum FILE_MASK_INDEX
{ 
	FA, FB, FC, FD, FE, FF, FG, FH
};

const U64 FileMasks8[Eight] =
{
	0x101010101010101L,
	0x202020202020202L,
	0x404040404040404L,
	0x808080808080808L,
	0x1010101010101010L,
	0x2020202020202020L,
	0x4040404040404040L,
	0x8080808080808080L
};

class Piece
{
protected:
	static const U64 FILE_AB;
	static const U64 FILE_GH;
	static const U64 DiagonalMasks8[15];
	static const U64 AntiDiagonalMasks8[15];

	bool _attackWithQueen = false;

public:	
	static string POSSIBLE_MOVES;
	static U64 UNSAFE;
	static U64 CANT_CAPTURE;
	static U64 OCCUPIED;

public:
	////////////Virtuals///////////////////////////////////
	virtual void Possible(U64 board, COLOR color);
	virtual void Attack(U64 board, U64 boaredQueen, COLOR color);
	virtual U64 Possibility(int location) { return 0; }

	inline U64 Possibility_Span(int location, U64 span, int subtract)
	{
		U64 possibility = (location > subtract)
			? span << (location - subtract)
			: span >> (subtract - location);

		possibility &= ((location % Eight) < 4)
			? ~FILE_GH
			: ~FILE_AB;

		return 	possibility;
	}

	void ToString(U64 board);

	static U64 HAndVMoves(int s);
	static U64 DAndAntiDMoves(int s);
	static U64 reverse(U64 n);

	//Finds the position of the least significant bit set to 1
	// static inline int __builtin_ctzll(U64 pos) //Comment out on Linux
	// {
	// 	//return __builtin_ctzll(pos);
	// 	unsigned long trailing_zero = 0;
	// 	return (_BitScanForward64(&trailing_zero, pos))
	// 		? trailing_zero
	// 		: 63;
	// }
};

class Knight : public Piece
{
	const U64 KNIGHT_SPAN = 0x0000000a1100110a;

public:	
	virtual U64 Possibility(int location) override
	{
		return Possibility_Span(location, KNIGHT_SPAN, 18);
	}
};

class Queen : public Piece
{
public:
	virtual U64 Possibility(int location) override
	{
		return
			HAndVMoves(location) |
			DAndAntiDMoves(location);
	}

	virtual void Attack(U64 board, U64 boaredQueen, COLOR color) override {}
};

class Bishop : public Piece
{
public:
	Bishop() { _attackWithQueen = true; }

	virtual U64 Possibility(int location) override
	{
		return DAndAntiDMoves(location);
	}
};

class Rook : public Piece
{
public:
	Rook() { _attackWithQueen = true; }

	virtual U64 Possibility(int location) override
	{
		return HAndVMoves(location);
	}
};

class King : public Piece
{
	const U64 KING_SPAN = 0x0000000000070507;

public:
	virtual void Possible(U64 board, COLOR color) override
	{
		ToString(board);
	}

	virtual void Attack(U64 board, U64 boaredQueen, COLOR color) override
	{
		UNSAFE |= Possibility(__builtin_ctzll(board));
	}

	virtual U64 Possibility(int location) override
	{
		return Possibility_Span(location, KING_SPAN, 9);
	}
};

class Pawn : public Piece
{
	const U64 RankFileMask[MAX_COLORS][9]
	{
		{  //White
			~RankMasks8[R8] & ~FileMasks8[FA],	//Capture right 
			~RankMasks8[R8] & ~FileMasks8[FH],	//Capture left
			~RankMasks8[R8],					//Move 1 forward
			 RankMasks8[R4],					//Move 2 forward
			 RankMasks8[R8] & ~FileMasks8[FA],  //Promotion by capture right
			 RankMasks8[R8] & ~FileMasks8[FH],  //Promotion by capture left
			 RankMasks8[R8],					//Promotion by move 1 forward
			 RankMasks8[R5] & ~FileMasks8[FA],	//En passant right
			 RankMasks8[R5] & ~FileMasks8[FH]	//En passant left
		},
		{  //Black = 8 - 1 - White 
			~RankMasks8[R1] & ~FileMasks8[FH],	//Capture right
			~RankMasks8[R1] & ~FileMasks8[FA],	//Capture left
			~RankMasks8[R1],					//Move 1 forward
			 RankMasks8[R5],					//Move 2 forward
			 RankMasks8[R1] & ~FileMasks8[FH],	//Promotion by capture right
			 RankMasks8[R1] & ~FileMasks8[FA],	//Promotion by capture left
			 RankMasks8[R1],					//Promotion by move 1 forward
			 RankMasks8[R4] & ~FileMasks8[FH],	//En passant right
			 RankMasks8[R4] & ~FileMasks8[FA]	//En passant left
		}
	};

	const string PP[MAX_COLORS][4] =
	{
		{ "QP", "RP", "BP", "NP" },
		{ "qP", "rP", "bP", "nP" }
	};

	const string EP[MAX_COLORS] = 
	{
		"WE",
		"BE"
	};

public:
	static U64 MY_PIECES;
	static U64 RivalP_EP;

	virtual void Possible(U64 board, COLOR color) override;

	virtual void Attack(U64 board, U64 boaredQueen, COLOR color) override
	{
		UNSAFE |= (color == WC)
			? ((board >> 7) & ~FileMasks8[FA]) |	// Pawn capture right
			  ((board >> 9) & ~FileMasks8[FH])		// Pawn capture left
			: ((board << 7) & ~FileMasks8[FH]) |	// Pawn capture right
			  ((board << 9) & ~FileMasks8[FA]);		// Pawn capture left
	}

private:
	void Regular(U64 PAWN_MOVES, int add1, int add2 = 0);
	void Promotion(U64 PAWN_MOVES, const string* pp, int add = 0);
	void Enpassant(U64 PAWN_MOVES, const string enpassant, int add);

	inline U64 Shift(U64 board, bool isRight, int shiftBy)
	{
		return (isRight)
			? board >> shiftBy
			: board << shiftBy;
	}
};

#endif // PIECE_H

