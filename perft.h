#ifndef _PERFT_H
#define _PERFT_H

#include <iostream>
#include <fstream>
#include <chrono>
#include "representation.h"

using namespace std::chrono;

class Perft
{
public:
	U64 _totalMoveCounter = 0;
	U64 _moveCounter = 0;
	U64 _maxDepth = 3;

	U64 _invalidMoves = 0;

	string MoveToAlgebra(const string& move);

public:

	void SetMaxDepth(int maxDepth)
	{
		_maxDepth = maxDepth;
		_totalMoveCounter = 0;
		_moveCounter = 0;

		_invalidMoves = 0;
	}

	void perft(Position *poss, const DATA& dataIn, COLOR clr, int depth);
	void perftRoot(Position*poss, const DATA& dataIn, COLOR clr, int depth);

	U64 MaxDepth() { return _maxDepth; }
	U64 MoveCounter() { return _moveCounter; }
};

////////////Logger///////////////////////////
class Logger
{
	static ofstream _file;
	static time_point<steady_clock> _st;

	static int _moveCursor;
	static int _moveCountMax;	//0: do not log move

public:
	static void Start(bool isFile = false, int moveCountMax = 0)
	{
		_moveCountMax = moveCountMax;
		_moveCursor = 0;

		if (isFile)
			_file.open("c:\\temp\\file.txt");
	}

	static void ResetClock()
	{
		_st = steady_clock::now();
		_moveCursor = 0;
	}

	static void WriteTime()
	{
		duration<double> span = duration_cast<duration<double>>(steady_clock::now() - _st);
		U64 seconds = (U64)(span.count());
		Write(seconds / 60, "Minutes");
		Write(seconds % 60, "Seconds", "\n");
	}

	static void WriteMove(string move)
	{
		if (_moveCountMax == 0)
			return;
		if (_moveCursor++ == _moveCountMax)
		{
			NewLine();
			_moveCursor = 1;
		}
		for (int i = 0; i < 4; i++)
		{
			if (move[i] == '\n')
				Write('x'); //to avoid unwanted newline
			else if (move[i] == '\r')
				Write('y'); //to avoid unwanted newline
			else
				Write(move[i]);
		}
		Write(' ');
	}

	static void Write(string str)
	{
		cout << str;
		if (_file.is_open())
			_file << str;
		NewLine();
	}

	static void Write(char chr)
	{
		cout << chr;
		if (_file.is_open())
			_file << chr;
	}

	static void Write(U64 num, string title, string delimter = ", ")
	{
		title += " ";

		cout << title << to_string(num) << delimter;
		if (_file.is_open())
			_file << title << num << delimter;
	}

	static void NewLine()
	{
		cout << endl;
		if (_file.is_open())
			_file << "\n";
	}
};

#endif //_PERFT_H
