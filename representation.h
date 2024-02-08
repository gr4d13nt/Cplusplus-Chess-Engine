#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include "moves.h"

class Position
{
	Piece* _piece[MAX_KEYS] =
	{
	   new Pawn(),
	   new Bishop(),
	   new Rook(),
	   new Queen(),
	   new King(),
	   new Knight()
	};
	
	DATA _dataInit;
	
	U64 _myPieces[MAX_COLORS];
	U64 _bothKing;

	struct CastleRook
	{
		string moveC;
		U64 StartL;
		U64 EndL;
		U64 NotStartL;
		U64 NotLeft1;
		U64 LeftAll;

		CastleRook(//CASTLE_ROOKS[]={63, 56, 7, 0}
			int CASTLE_ROOK, int CASTLE_ROOK_Add,
			string moveC,
			int shift1, int shift2, int shift3) : moveC(moveC)
		{
			StartL = 1ULL << CASTLE_ROOK;
			EndL = 1ULL << (CASTLE_ROOK + CASTLE_ROOK_Add);
			NotStartL = ~StartL;

			NotLeft1 = (shift3 == 0) //King 
				? UINT64_MAX
				: ~(1ULL << shift1);
			LeftAll =
				(1ULL << shift1) |
				(1ULL << shift2) |
				(1ULL << shift3);
		}
	}
	const Castle[MAX_COLORS][MAX_FLAGS] =
	{
		{	//White
			{ 63, -2, "7476", 61, 62, 0 }, //King
			{ 56,  3, "7472", 57, 58, 59 } //Queen
		},
		{	//Black
			{ 7,  -2, "0406", 5,  6,  0 }, //King
			{ 0,   3, "0402", 1,  2,  3 }  //Queen
		}
	};

	struct StartEnd
	{
		int start = -1; //can't be U64 or char!
		U64 mask;
		U64 startL = -1;
		U64 endL = -1;
		U64 notStartL = -1;
		U64 notEndL = -1;

		void Set(int s, int e, U64 m = 0)
		{
			start = s;
			mask = m;
			startL = 1ULL << s;
			endL = 1ULL << e;
			notStartL = ~startL;
			notEndL = ~endL;
		}
	} _se;

public:
	Position(COLOR initColor = WC, bool print = true);
	~Position();

	const DATA& DataInit() { return _dataInit; }

	string PossibleMoves(const DATA& in, COLOR clr);
	bool MakeMoves(const string& move, const DATA& in, DATA& dataOut);
	bool UnsafeFor(COLOR color, const DATA& in);

private:
	void ArrayToBitborards(char chessBoard[Eight][Eight]);
	void PrintByKeys();

	void PrintBitBoard(U64 bitBoard);
};

#endif // REPRESENTATION_H
