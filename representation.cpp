#include "representation.h"
#include "perft.h"

Position::Position(COLOR initColor, bool print)
{
	// char chessBoard[Eight][Eight]
	// {
	// 	{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
	// 	{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
	// 	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	// 	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	// 	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	// 	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	// 	{'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
	// 	{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
	// };

	 char chessBoard[Eight][Eight] =
	{
	  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	  {' ', ' ', 'p', ' ', ' ', ' ', ' ', ' '},
	  {' ', ' ', ' ', 'p', ' ', ' ', ' ', ' '},
	  {'K', 'P', ' ', ' ', ' ', ' ', ' ', 'r'},
	  {' ', 'R', ' ', ' ', ' ', 'p', ' ', 'k'},
	  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	  {' ', ' ', ' ', ' ', 'P', ' ', 'P', ' '},
	  {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
	};

	ArrayToBitborards(chessBoard);
	if (print)
		PrintByKeys();

	UnsafeFor(initColor, _dataInit);
}

Position::~Position() 
{ 
	for (int k = 0; k < MAX_KEYS; k++)
		delete _piece[k];
}

string Position::PossibleMoves(const DATA& in, COLOR color)
{	
	Piece::POSSIBLE_MOVES = EmptyString;

	//Added RivalK to avoid illegal capture
	Piece::CANT_CAPTURE = ~(_myPieces[color] | _bothKing);

	COLOR rival = (COLOR)(1 - color);

	Pawn::MY_PIECES = _myPieces[rival];
	Pawn::RivalP_EP = in.Brd[rival][P] & in.EP_TargetSquare;
	
	for (int k = 0; k < MAX_KEYS; k++)
		_piece[k]->Possible(in.Brd[color][k], color);
	
	for (int f = 0; f < MAX_FLAGS; f++)
	{
		if (!in.Flg[color][f])
			continue;
		
		const CastleRook& castle = Castle[color][f];
		if ((in.Brd[color][R] & castle.StartL) == 0)
			continue;
		
		if (((Piece::OCCUPIED | (Piece::UNSAFE & castle.NotLeft1)) &
			castle.LeftAll) == 0)
			Piece::POSSIBLE_MOVES += castle.moveC;
	}

	return Piece::POSSIBLE_MOVES;
}

bool Position::UnsafeFor(COLOR color, const DATA& in)
{
	U64 boardKing = in.Brd[color][K];
	
	color = (COLOR)(1 - color);
	
	for (int c = 0; c < MAX_COLORS; c++)
		_myPieces[c] = in.Brd[c][P] | in.Brd[c][B] | in.Brd[c][N] |
					   in.Brd[c][R] | in.Brd[c][Q];
	_bothKing = in.Brd[WC][K] | in.Brd[BC][K];
	Piece::OCCUPIED = _myPieces[WC] | _myPieces[BC] | _bothKing;
	
	Piece::UNSAFE = 0;
	for (int k = 0; k < MAX_KEYS; k++)
		_piece[k]->Attack(in.Brd[color][k], in.Brd[color][Q], color);

	return (Piece::UNSAFE & boardKing) == 0;
}

bool Position::MakeMoves(const string& move, const DATA& in, DATA& out)
{
	out.EP_TargetSquare = 0;
	::memcpy(&out.Flg, &in.Flg, sizeof(out.Flg));

	switch (move[3])
	{
		case MovePromotion:
		{
			if (iswupper(move[2]))
				_se.Set((move[0] - B0) * Eight + 6,
					    (move[1] - B0) * Eight + 7);
			else
				_se.Set((move[0] - B0) * Eight + 1,
					    (move[1] - B0) * Eight);

			for (int c = 0; c < MAX_COLORS; c++)
				for (int k = 0; k < MAX_KEYS; k++)
				{
					U64 board = in.Brd[c][k];
					if (move[2] == Key[c][k])
						board |= _se.endL;
					else
					{
						board &= _se.notStartL;
						board &= _se.notEndL;
					}
					out.Brd[c][k] = board;
				}
		} return true;
		case MoveEnpassant:
		{
			if (move[2] == MoveEnpassant_W)
				_se.Set(
					__builtin_ctzll(FileMasks8[move[0] - B0] & RankMasks8[R5]),
					__builtin_ctzll(FileMasks8[move[1] - B0] & RankMasks8[R6]),
					~(FileMasks8[move[1] - B0] & RankMasks8[R5]));
			else
				_se.Set(
					__builtin_ctzll(FileMasks8[move[0] - B0] & RankMasks8[R4]),
					__builtin_ctzll(FileMasks8[move[1] - B0] & RankMasks8[R3]),
					~(FileMasks8[move[1] - B0] & RankMasks8[R4]));

			for (int c = 0; c < MAX_COLORS; c++)
				for (int k = 0; k < MAX_KEYS; k++)
				{
					U64 board = in.Brd[c][k] & _se.mask;
					if (((board >> _se.start) & 1) == 1)
					{
						board &= _se.notStartL;
						board |= _se.endL;
					}
					out.Brd[c][k] = board;
				}
		} return true;
		default:
		{
			if (!isdigit(move[3]))
				return false;

			_se.Set((move[0] - B0) * Eight + (move[1] - B0),
				    (move[2] - B0) * Eight + (move[3] - B0));

			for (int c = 0; c < MAX_COLORS; c++)
				for (int k = 0; k < MAX_KEYS; k++)
				{
					U64 board = in.Brd[c][k];
					if (((board >> _se.start) & 1) == 1)
					{
						board &= _se.notStartL;
						board |= _se.endL;
					}
					else
						board &= _se.notEndL;
					out.Brd[c][k] = board;
				}
			if (((_bothKing >> _se.start) & 1) == 1)
				for (int c = 0; c < MAX_COLORS; c++)
					for (int f = 0; f < MAX_FLAGS; f++)
						if (Castle[c][f].moveC == move)
						{
							out.Brd[c][R] &= Castle[c][f].NotStartL;
							out.Brd[c][R] |= Castle[c][f].EndL;
							break;
						}
			
			U64 bothPawn = in.Brd[WC][P] | in.Brd[BC][P];
			bool isUpdateFlags = (bothPawn & _se.startL) == 0;
			if (!isUpdateFlags || ((bothPawn >> _se.start) & 1) == 1)
			{
				if (abs(move[0] - move[2]) == 2) //Pawn double push detected
					out.EP_TargetSquare = FileMasks8[move[1] - B0]; //Calc en passant target square
			}
			if (isUpdateFlags)
			{
				if ((_se.startL & in.Brd[WC][K]) != 0)
					out.Flg[WC][CK] = out.Flg[WC][CQ] = false;
				else if ((_se.startL & in.Brd[BC][K]) != 0)
					out.Flg[BC][CK] = out.Flg[BC][CQ] = false;
				else
					for (int c = 0; c < MAX_COLORS; c++)
						for (int f = 0; f < MAX_FLAGS; f++)
							if ((in.Brd[c][R] & _se.startL & Castle[c][f].StartL) != 0)
							{
								out.Flg[c][f] = false;
								break;
							}
			}
		} return true;
	}
}

////////////Initialize Board/////////////////////////////////////
void Position::ArrayToBitborards(char chessBoard[Eight][Eight])
{
	memset(&_dataInit, 0, sizeof(DATA));
	memset(&_dataInit.Flg, true, sizeof(_dataInit.Flg));

	char binary[SixtyFour];
	for (size_t i = 0; i < SixtyFour; i++)
	{
		::memset(&binary, B0, sizeof(binary));
		binary[SixtyFour - 1 - i] = B1;

		char key = chessBoard[i / Eight][i % Eight];
		if (key == SPACE)
			continue;

		for (int c = 0; c < MAX_COLORS; c++)
			for (int k = 0; k < MAX_KEYS; k++)
				if (Key[c][k] == key)
				{
					for (size_t b = 0; b < SixtyFour; b++)
						if (binary[b] == B1)
						{
							_dataInit.Brd[c][k] |= (1ULL << (SixtyFour - 1 - b));
							break;
						}
					break;
				}
	}
}

void Position::PrintByKeys()
{
	for (int i = 0; i < SixtyFour; i++)
	{
		if ((i % Eight) == 0 && i != 0)
			Logger::NewLine(); // New line at the end of each row

		bool isEmpty = true;
		for (int c = 0; c < MAX_COLORS; c++)
			for (int k = 0; k < MAX_KEYS; k++)
				if (((_dataInit.Brd[c][k] >> i) & 1) == 1)
				{
					isEmpty = false;
					Logger::Write(Key[c][k]);
				}
		if (isEmpty)	//I only need start value here
			Logger::Write('.');
	}
	Logger::NewLine();
}

void Position::PrintBitBoard(U64 bitBoard)
{
	char chessBoard[Eight][Eight];

	// Mark the squares with X where bits are set in bitBoard
	const char printChars[] = { SPACE, 'X' };

	for (int i = 0; i < SixtyFour; ++i)
		chessBoard[i / Eight][i % Eight] = printChars[(bitBoard >> i) & 1];

	for (int i = 0; i < Eight; ++i)
	{
		for (int j = 0; j < Eight; ++j)
			Logger::Write(chessBoard[i][j]);
		Logger::NewLine();
	}
}
/////////////////END/////////////////////////////////////////////