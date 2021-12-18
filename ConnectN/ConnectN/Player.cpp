// Player.cpp

#include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
using namespace std;

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int col;
    cin >> col;

    while (col > s.cols() || col <= 0 && s.checkerAt(col, s.levels() != VACANT))        // If col entered is invalid and is not vacant
    {
        cout << "Invalid input! Re-enter a column: ";
        cin >> col;     // Re-prompt input
    }

    if (s.numberEmpty() == 0)       // If no moves possible, return 0
        return 0;

    return col;     // Return column inputted
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (s.numberEmpty() == 0)       // If no moves possible, return 0
        return 0;

    for (int i = 1; i <= s.cols(); i++)
    {
        if (s.checkerAt(i, s.levels()) == VACANT)       // If column is not full, choose the leftmost non-empty column
            return i;
    }
    return 0; // Indicate failure
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    AlarmClock ac(9900);    // Create alarm clock
    Scaffold test = s;      // Create a test scaffold
    bool turn = true;       // Initialize turn to determine whether to return max or min
    int depth = 0;          // Set depth to 0

    return determineBestMove(test, N, color, depth,turn, ac).first; // Determine the best move
}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
