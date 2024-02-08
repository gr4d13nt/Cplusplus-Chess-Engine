#include "representation.h"
#include "perft.h"

using namespace std::chrono;

int main()
{
	const short Expected_Count = 7;
	U64 Expect[Expected_Count] =
	{
		0,
		20,
		400,
		8902,
		197281,
		4865609,
		119060324,
		//18446744072609603475
	};

	Logger::Start(true);
	for (int i = 0; i < Expected_Count - 1; i++)
	{
		COLOR initColor = WC;
		Position p(initColor, i == 0);
		Perft perft;

		try
		{
			Logger::ResetClock();
			perft.SetMaxDepth(i);
			perft.perft(&p, p.DataInit(), initColor, 0);
		}
		catch (...)
		{
			Logger::Write("Exception has been thrown performing perft");
		}
		if (perft.MoveCounter() != Expect[i])
			Logger::Write("ERROR: Total move count is not expected!");
		if (perft._invalidMoves > 0)
			Logger::Write(perft._invalidMoves, "ERROR: Invalid move type", "\n");

		Logger::Write(perft.MaxDepth(), "MaxDepth");
		Logger::Write(perft.MoveCounter(), "Count");
		Logger::Write(Expect[i], "Expected");
		Logger::WriteTime();
	}
}