#include <stdio.h>
#include <iostream>
#include <utility>
#include "support.h"
#include "provided.h"
#include <vector>

using namespace std;

bool possibleMove(Scaffold s, int column)
{
	for (int i = 1; i <= s.levels(); i++)		// Loop through each level
	{
		if (s.checkerAt(column, i) == VACANT)	// If a vacancy is found, return true
			return true;
	}
	return false;		// Return false if no vacancy
}

int lastLevelUsed(const Scaffold& s, int lastMove)
{
	int tracker = 1;	// Tracker int

	for (int i = 1; i <= s.levels(); i++)
	{
		if (s.checkerAt(lastMove, i) == VACANT)	// Find the last vacant spot in a column
		{
			tracker = i;			// Set tracker equal to i
			return (tracker - 1);	// Return tracker - 1 (this will be the last non-vacant entry)
		}
	}

	return (s.levels());	// If all are not VACANT, return the max level in Scaffold
}

int rate(Scaffold s, int N, int color, int lastMove, int depth, bool turn)
{
	int rating = -9999;		// Initialize rating to junk number

	// Check horizontal
	int horizCount = 1;	// Similar horizontal count count

	for (int i = 1; i <= s.cols(); i++)
	{
		if (lastMove + i > s.cols())		// If the checked square is out of bounds, don't allow function to continue for current i
			break;

		if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove + i, lastLevelUsed(s, lastMove)))	// If in bounds and equal squares, increment count
		{
			horizCount++;
		}
		else
			break;	// If no other similarity found, break out of for loop
	}

	for (int i = 1; i <= s.cols(); i++)
	{
		if (lastMove - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
			break;

		if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove - i, lastLevelUsed(s, lastMove)))	// If in bounds and equal squares, increment count
		{
			horizCount++;
		}
		else
			break;	// If no other similarity found, break out of for loop
	}

	if (horizCount >= N)		// If horizontal count is greater than or equal to m_N
		if (turn)
			rating = 1000 - depth;		// Subtract depth from a big number
		else if (!turn)
			rating = depth - 1000;		// Subtract big number from depth

	horizCount = 1;	// If no winner found or winning condition not met, reset counter

	if (rating == -9999)
	{
		// Check vertical
		int vertCount = 1;	// Similar vertical count

		for (int i = 1; i <= s.cols(); i++)
		{
			if (lastLevelUsed(s, lastMove) + i > s.levels())		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove, lastLevelUsed(s, lastMove) + i))	// If in bounds and equal squares, increment count
			{
				vertCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		for (int i = 1; i <= s.cols(); i++)
		{
			if (lastLevelUsed(s, lastMove) - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove, lastLevelUsed(s, lastMove) - i))	// If in bounds and equal squares, increment count
			{
				vertCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		if (vertCount >= N)		// If vertical count is greater than or equal to m_N
			if (turn)
				rating = 1000 - depth;		// Subtract depth from a big number
			else if (!turn)
				rating = depth - 1000;		// Subtract big number from depth

		vertCount = 1;	// If no winner found or winning condition not met, reset counter
	}

	if (rating == -9999)
	{
		// Check diagonals
		if (!(N > s.cols() || N > s.levels()))	// Check if it's possible to have a diagonal win
		{
			// Ascending diagonal check
			int LRdiagonalCount = 1;
			for (int i = 1; i <= s.cols(); i++)
			{
				if (lastMove + i > s.cols() || lastLevelUsed(s, lastMove) + i > s.levels())		// If the checked square is out of bounds, don't allow function to continue for current i
					break;

				if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove + i, lastLevelUsed(s, lastMove) + i))	// If in bounds and equal squares, increment count
				{
					LRdiagonalCount++;
				}
				else
					break;	// If no other similarity found, break out of for loop
			}

			for (int i = 1; i <= s.cols(); i++)
			{
				if (lastMove - i < 1 || lastLevelUsed(s, lastMove) - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
					break;

				if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove - i, lastLevelUsed(s, lastMove) - i))	// If in bounds and equal squares, increment count
				{
					LRdiagonalCount++;
				}
				else
					break;	// If no other similarity found, break out of for loop
			}

			if (LRdiagonalCount >= N)		// If diagonal count is greater than or equal to m_N
				if (turn)
					rating = 1000 - depth;		// Subtract depth from a big number
				else if (!turn)
					rating = depth - 1000;		// Subtract big number from depth


			LRdiagonalCount = 1;	// If no winner found or winning condition not met, reset counter
		}
	}

	if (rating == -9999)
	{
		// Descending diagonal check
		int RLdiagonalCount = 1;
		for (int i = 1; i <= s.cols(); i++)
		{
			if (lastMove - i < 1 || lastLevelUsed(s, lastMove) + i > s.levels())		// If out of bounds, break out of loop
				break;

			if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove - i, lastLevelUsed(s, lastMove) + i))	// If in bounds and similarity found, increment counter
			{
				RLdiagonalCount++;
			}
			else
				break;	// If no similarities, break out of for loop
		}

		for (int i = 1; i <= s.cols(); i++)
		{
			if (lastMove + i > s.cols() || lastLevelUsed(s, lastMove) - i < 1)	// If out of bounds, break out of loop
				break;

			if (s.checkerAt(lastMove, lastLevelUsed(s, lastMove)) == s.checkerAt(lastMove + i, lastLevelUsed(s, lastMove) - i))	// If in bounds and similarity found, increment counter
			{
				RLdiagonalCount++;
			}
			else
				break;	// If no similarities, break out of for loop
		}

		if (RLdiagonalCount >= N)		// If counter is greater than or equal to m_N 
			if (turn)
				rating = 1000 - depth;		// Subtract depth from a big number
			else if (!turn)
				rating = depth - 1000;		// Subtract big number from depth


		RLdiagonalCount = 1;	// Reset counter if not enough to satisfy winning conditions
	}

	// Full scaffold
	if (rating == -9999 && s.numberEmpty() == 0)
	{
		rating = 0;
	}

	return rating;
}

pair<int, int> determineBestMove(Scaffold& s, int N, int color, int& depth, bool turn, AlarmClock& ac)
{
	std::pair<int, int> colAndRating; // Pair that holds the column and rating for a move
	Scaffold test = s;
	int bestRating = 0;
	int colChosen = 0;
	vector<int> storedValues;
	storedValues.resize(test.cols());	// Stores the ratings of each move

	if (!ac.timedOut())		// If there's still time, run the algorithm
	{
		for (int i = 1; i <= test.cols(); i++)
		{
			// Check columns possible to make a move
			if (possibleMove(s, i))
			{
				s.makeMove(i, color);	// Make a move
				depth++;				// Increase depth

				if (rate(s, N, color, i, depth, turn) != -9999)		// If the rating does not equal -9999, return pair
				{
					colAndRating.first = i;		// Assign i to column of pair
					colAndRating.second = rate(s, N, color, i, depth, turn);	// Assign rating to rating of pair
					s.undoMove();	// Undo move
					depth--;	// Decrease depth
					return colAndRating;	// Return pair
				}
				else      // If not, store rating
				{
					color = 1 - color;		// Change color for next call
					turn = !turn;			// Change turn for next call
					storedValues[i - 1] = determineBestMove(s, N, color, depth, turn, ac).second;	// Store rating
				}

				s.undoMove();		// Undo move
				color = 1 - color;	// Change color for next call
				turn = !turn;		// Change turn for next call
				depth--;			// Decrease depth
			}

			if (!possibleMove(s, i))		// If no move is possible store an arbitrary number for rating
			{
				if (turn)	// Depending on turn, either store really high or low number
					storedValues[i - 1] = -1000000;
				else
					storedValues[i - 1] = 1000000;
			}
		}

		if (turn)				// Depending on turn, either return max rating or min rating
			bestRating = -9000;
		else if (!turn)
			bestRating = 9000;

		for (int i = 0; i < test.cols(); i++)		// Loop through columns
		{
			if (turn)	// Depending on turn either set the best rating equal to higher rating or lower rating
			{
				if (storedValues[i] > bestRating)
				{
					bestRating = storedValues[i];
					colChosen = i + 1;
				}
			}

			if (!turn)
			{
				if (storedValues[i] < bestRating)
				{
					bestRating = storedValues[i];
					colChosen = i + 1;
				}
			}
		}

		colAndRating.first = colChosen;		// Set column of pair equal to the column chosen
		colAndRating.second = bestRating;	// Set rating of pair equal to the best rating found (depending on whose turn it is)
		return colAndRating;	// Return pair
	}
	else      // If alarm clock timed out, return what was found so far
	{
		cerr << "Time out!" << endl;
		return colAndRating;
	}
}

	


			
	


	

