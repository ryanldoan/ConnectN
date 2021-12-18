// Game.cpp

#include "provided.h"
#include <iostream>
#include <string>

using namespace std;

class GameImpl
{
public:
	GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
	bool completed(int& winner) const;
	bool takeTurn();
	void play();
	int checkerAt(int c, int r) const;
	int determineTurn();
	bool checkHorizontal(int& winner) const;
	bool checkVertical(int& winner) const;
	bool checkAscendingDiag(int& winner) const;
	bool checkDescendingDiag(int& winner) const;
	int lastLevelUsed() const;
	~GameImpl() { delete s; };
private:
	Scaffold* s;
	Player* redPlayer;
	Player* blackPlayer;
	int m_N;
	int m_current_player;
	bool oneByOneWin;
	int recent_column;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
	: m_current_player(0), oneByOneWin(false), recent_column(-1000)		// Initialize member variables (recent_column is initialized to -1000 to prevent runtime error)
{
	s = new Scaffold(nColumns, nLevels);		// Dynamically allocate a scaffold
	redPlayer = red;							// Make member players point to passed player
	blackPlayer = black;

	if (N < 1 || (N > nColumns && N > nLevels))		// Check for valid N
	{
		cerr << "Impossible to Connect " << N << " checkers!" << endl;
		exit(1);
	}
	m_N = N;	// Initialize N if valid
}

bool GameImpl::completed(int& winner) const
{
	// If the Scaffold is 1x1, RED will always win
	if (oneByOneWin == true)
	{
		winner = RED;
		return true;
	}

	//	Check for horizontal connections
	if (checkHorizontal(winner) == true)
		return true;

	//	Check for vertical connections
	if (checkVertical(winner) == true)
		return true;

	if (!(m_N > s->cols() || m_N > s->levels()))	// Check if it's possible to have a diagonal win
	{
		//	Check for left to right diagonal connections
		if (checkAscendingDiag(winner) == true)
			return true;

		// Check for right to left diagonal connections
		if (checkDescendingDiag(winner) == true)
			return true;
	}

	// If no more squares and no winner has been found yet, it is a tie
	if (s->numberEmpty() == 0 && (winner != RED && winner != BLACK))
	{
		winner = TIE_GAME;
		return true;		// Return true
	}

	return false;	// If no  conditions found, return false
}

bool GameImpl::takeTurn()
{
	int placeholder;
	if (completed(placeholder) == false)	// If game is not completed, continue through the function
	{
		if (s->cols() == 1 && s->levels() == 1)		// If 1x1 Scaffold, always give the win to RED Player
		{
			if (redPlayer->isInteractive())		// If interactive player or not, print out correct prompt
				cout << "RED Player enter a column number: ";
			else
				cout << "COMPUTER Turn: " << endl;

			s->makeMove(redPlayer->chooseMove(*s, m_N, RED), RED); // Make a move on the chosen column for RED Player
			oneByOneWin = true;		// Indicate that a 1x1 Win is true
			return true;	// Return true
		}

		if (determineTurn() == RED)		// If RED's turn
		{
			if (redPlayer->isInteractive())
				cout << "RED Player enter a column number: ";
			else
				cout << "COMPUTER Move: " << endl;

			recent_column = redPlayer->chooseMove(*s, m_N, RED);	// Assign recent_column to the chosen move
			s->makeMove(recent_column, RED);	// Make a move on the chosen column
			m_current_player++;		// Switch player turns
			return true;
		}
		else		// If BLACK's turn
		{
			if (blackPlayer->isInteractive())
				cout << "BLACK Player enter a column number: ";
			else
				cout << "COMPUTER Move: " << endl;

			recent_column = blackPlayer->chooseMove(*s, m_N, BLACK);	// Assign recent_column to the chosen move
			s->makeMove(recent_column, BLACK);		// Make a move on the chosen column
			m_current_player++;		// Switch player turns
			return true;
		}
	}
	return false;		// Return false if game is already completed
}

void GameImpl::play()
{
	int placeholder;
	s->display();

	while (completed(placeholder) == false)		// Run loop while the game is not finished
	{
		if (!redPlayer->isInteractive() && !blackPlayer->isInteractive())	// If both players are not interactive
		{
			cout << "Press enter to view next move" << endl;	// Prompt to press enter
			cin.ignore(1000, '\n');		// Ignore cin buffer
			takeTurn();		// Take turn
			s->display();	// Display board
		}
		else	// If one or more players are interactive
		{
			takeTurn();		// Take turn (functions inside will prompt user)
			s->display();	// Display board
		}
	}

	if (placeholder == RED)		// If winner is RED, print RED Player wins
		cout << "RED Player wins!" << endl;
	else if (placeholder == BLACK)		// If winner is BLACK, Print BLACK Player wins
		cout << "BLACK Player wins!" << endl;
	else
		cout << "TIE Game!" << endl;	// Print out it's a tie
}

int GameImpl::checkerAt(int c, int r) const
{
	return s->checkerAt(c, r);		// Returns checker at a square
}

int GameImpl::determineTurn()
{
	if (m_current_player == 0 || m_current_player % 2 == 0)		// If m_current_player is 0 or even it is RED's turn
		return RED;
	else
		return BLACK;	// If odd, it is BLACK's turn
}

int GameImpl::lastLevelUsed() const
{
	int tracker = 1;	// Tracker int

	for (int i = 1; i <= s->levels(); i++)
	{
		if (s->checkerAt(recent_column, i) == VACANT)	// Find the last vacant spot in a column
		{
			tracker = i;			// Set tracker equal to i
			return (tracker - 1);	// Return tracker - 1 (this will be the last non-vacant entry)
		}
	}

	return (s->levels());	// If all are not VACANT, return the max level in Scaffold
}

bool GameImpl::checkAscendingDiag(int& winner) const
{
	int LRdiagonalCount = 1;	// Similar left to right diagonal count

	if (recent_column != -1000)		// If recent_column is not -1000 (to prevent runtime error)
	{
		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column + i > s->cols() || lastLevelUsed() + i > s->levels())		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column + i, lastLevelUsed() + i))	// If in bounds and equal squares, increment count
			{
				LRdiagonalCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column - i < 1 || lastLevelUsed() - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column - i, lastLevelUsed() - i))	// If in bounds and equal squares, increment count
			{
				LRdiagonalCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		if (LRdiagonalCount >= m_N)		// If diagonal count is greater than or equal to m_N
		{
			winner = s->checkerAt(recent_column, lastLevelUsed());	// Set the winner
			return true;	// Return true
		}

		LRdiagonalCount = 1;	// If no winner found or winning condition not met, reset counter
	}

	return false;	// Return false if no winning condition found
}

bool GameImpl::checkDescendingDiag(int& winner) const
{
	int RLdiagonalCount = 1;		// Similar right to left diagonal count
	if (recent_column != -1000)			// If recent_column is not -1000 (to prevent runtime error)
	{
		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column - i < 1 || lastLevelUsed() + i > s->levels())		// If out of bounds, break out of loop
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column - i, lastLevelUsed() + i))	// If in bounds and similarity found, increment counter
			{
				RLdiagonalCount++;
			}
			else
				break;	// If no similarities, break out of for loop
		}

		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column + i > s->cols() || lastLevelUsed() - i < 1)	// If out of bounds, break out of loop
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column + i, lastLevelUsed() - i))	// If in bounds and similarity found, increment counter
			{
				RLdiagonalCount++;
			}
			else
				break;	// If no similarities, break out of for loop
		}

		if (RLdiagonalCount >= m_N)		// If counter is greater than or equal to m_N 
		{
			winner = s->checkerAt(recent_column, lastLevelUsed());		// Set winner
			return true;	// Return true
		}

		RLdiagonalCount = 1;	// Reset counter if not enough to satisfy winning conditions
	}

	return false;	// Return false
}

bool GameImpl::checkHorizontal(int& winner) const
{
	int horizCount = 1;	// Similar horizontal count count

	if (recent_column != -1000)		// If recent_column is not -1000 (to prevent runtime error)
	{
		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column + i > s->cols())		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column + i, lastLevelUsed()))	// If in bounds and equal squares, increment count
			{
				horizCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		for (int i = 1; i <= s->cols(); i++)
		{
			if (recent_column - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column - i, lastLevelUsed()))	// If in bounds and equal squares, increment count
			{
				horizCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		if (horizCount >= m_N)		// If horizontal count is greater than or equal to m_N
		{
			winner = s->checkerAt(recent_column, lastLevelUsed());	// Set the winner
			return true;	// Return true
		}

		horizCount = 1;	// If no winner found or winning condition not met, reset counter
	}

	return false;	// Return false if no winning condition found
}

bool GameImpl::checkVertical(int& winner) const
{
	int vertCount = 1;	// Similar vertical count

	if (recent_column != -1000)		// If recent_column is not -1000 (to prevent runtime error)
	{
		for (int i = 1; i <= s->cols(); i++)
		{
			if (lastLevelUsed() + i > s->levels())		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column, lastLevelUsed() + i))	// If in bounds and equal squares, increment count
			{
				vertCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		for (int i = 1; i <= s->cols(); i++)
		{
			if (lastLevelUsed() - i < 1)		// If the checked square is out of bounds, don't allow function to continue for current i
				break;

			if (s->checkerAt(recent_column, lastLevelUsed()) == s->checkerAt(recent_column, lastLevelUsed() - i))	// If in bounds and equal squares, increment count
			{
				vertCount++;
			}
			else
				break;	// If no other similarity found, break out of for loop
		}

		if (vertCount >= m_N)		// If vertical count is greater than or equal to m_N
		{
			winner = s->checkerAt(recent_column, lastLevelUsed());	// Set the winner
			return true;	// Return true
		}

		vertCount = 1;	// If no winner found or winning condition not met, reset counter
	}

	return false;	// Return false if no winning condition found
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
	m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}

Game::~Game()
{
	delete m_impl;
}

bool Game::completed(int& winner) const
{
	return m_impl->completed(winner);
}

bool Game::takeTurn()
{
	return m_impl->takeTurn();
}

void Game::play()
{
	m_impl->play();
}

int Game::checkerAt(int c, int r) const
{
	return m_impl->checkerAt(c, r);
}