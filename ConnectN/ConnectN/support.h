#include "provided.h"
#ifndef support_h
#define support_h

int lastLevelUsed(const Scaffold& s, int lastMove);		// Returns the last level used based on last move
int rate(Scaffold s, int N, int color, int lastMove, int depth, bool turn);	// Rates a move
bool possibleMove(Scaffold s, int column);	// Determines true or false whether it is possible to make a move in a column
std::pair<int, int> determineBestMove(Scaffold& s, int N, int color, int& depth, bool turn, AlarmClock& ac);	// Returns a pair that holds a column and a rating

#endif 