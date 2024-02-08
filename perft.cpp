#include "perft.h"

void Perft::perft(Position *poss, const DATA& in, COLOR color, int depth)
{
	if (depth > _maxDepth)
		return;
	
	string moves = poss->PossibleMoves(in, color);

	for (size_t i = 0; i < moves.length(); i += 4)
	{
		DATA out;
		if (!poss->MakeMoves(moves.substr(i, 4), in, out))
			_invalidMoves++;
		else if (poss->UnsafeFor(color, out))
		{
			if (depth + 1 == _maxDepth)
				_moveCounter++;
			perft(poss, out, (COLOR)(1 - color), depth + 1);
		}
	}
}

void Perft::perftRoot(Position *poss, const DATA& in, COLOR color, int depth)
{
	string moves = poss->PossibleMoves(in, color);

	for (size_t i = 0; i < moves.length(); i += 4)
	{
		DATA out;
		if (!poss->MakeMoves(moves.substr(i, 4), in, out))
			_invalidMoves++;
		else if (poss->UnsafeFor(color, out))
		{
			perft(poss, out, (COLOR)(1 - color), depth + 1);

			Logger::Write(MoveToAlgebra(moves.substr(i, 4)));
			Logger::Write(_moveCounter, " ", "\n");
			_totalMoveCounter += _moveCounter;
			_moveCounter = 0;
		}
	}
}

string Perft::MoveToAlgebra(const string& move)
{
	return EmptyString +
		static_cast<char>(move[1] + 49) +
		static_cast<char>(104 - move[0]) +
		static_cast<char>(move[3] + 49) +
		static_cast<char>(104 - move[2]);
}

////////////////////Ligger/////////////////////////////////////////
ofstream Logger::_file;
time_point<steady_clock> Logger::_st;
int Logger::_moveCursor = 0;;
int Logger::_moveCountMax = 0;