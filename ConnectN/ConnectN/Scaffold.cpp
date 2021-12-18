// Scaffold.cpp

#include "provided.h"
#include <vector>
#include <iostream>
#include <stack>

using namespace std;

class ScaffoldImpl
{
public:
    ScaffoldImpl(int nColumns, int nLevels);    // Constructor
    int cols() const;                           // Returns columns int
    int levels() const;                         // Returns levels int
    int numberEmpty() const;                        // Returns number of empty spots
    int checkerAt(int column, int level) const;         // Returns the color of a checker (or vacancy) at a location
    void display() const;                       // Displays the Scaffold
    bool makeMove(int column, int color);       // Drop a checker into a column
    int undoMove();                         // Undo the most recent move
    int lowestEmpty(int column) const;                // Return the lowest empty position in a column
private:
    int m_col;                      // Columns
    int m_levels;                   // Levels
    vector<vector<char>> grid;       // 2D vector

    class Pair      // Pair class 
    {
    public:
        Pair(int level, int column) : last_level(level), last_col(column) {}
        int m_last_level() { return last_level; }
        int m_last_col() { return last_col; }
    private:
        int last_level;     // Last level used
        int last_col;       // Last column used
    };

    stack<Pair> pairStack;  // Pair stack to save history of moves (for undo function)
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if (nColumns <= 0 || nLevels <= 0)
    {
        cerr << "Negative column and / or levels number!" << endl;
        exit(1);
    }

    m_col = nColumns;                   // After validation, initialize private data members 
    m_levels = nLevels;

    grid.resize(nLevels);               // Grid has nLevels empty rows

    for (int i = 0; i < nLevels; i++)
        grid[i].resize(nColumns);       // Row i has nColumns columns

    for (int i = 0; i < nLevels; i++)
        for (int j = 0; j < nColumns; j++)
            grid[i][j] = ' ';            // Declare each spot as vacant
}

int ScaffoldImpl::cols() const
{
    return m_col;       // Returns number of columns
}

int ScaffoldImpl::levels() const
{
    return m_levels;        // Returns number of levels
}

int ScaffoldImpl::numberEmpty() const
{
    int count = 0;       // Initalize counter to zero

    for (int i = 0; i < m_levels; i++)
        for (int j = 0; j < m_col; j++)
        {
            if (grid[i][j] == ' ')
                count++;    // Increment count everytime a VACANT spot found
        }

    return count;   // Return count
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (column > m_col || level > m_levels) // If outside of possible grid, return VACANT
        return VACANT;
    
    if (grid[m_levels - level][column - 1] == 'R')     // If RED found at grid location return RED
        return RED;
    else if (grid[m_levels - level][column - 1] == 'B')  // If BLACK found at grid location return BLACK
        return BLACK;
    else
        return VACANT;  // If neither RED or BLACK found at grid location return VACANT
}

void ScaffoldImpl::display() const
{
    for (int i = 1; i <= m_levels; i++)
    {
        for (int j = 1; j <= m_col; j++)
        {
            cout << "|";                    // Print a | before each element in a row
            cout << grid[i - 1][j - 1];         // Print element

            if (j == m_col)
                cout << "|";                // Print a | after the last element
        }
        cout << endl;       // Print a newline to separate different rows
    }

    for (int i = 0; i <= m_col; i++)
    {
        cout << "+";        // Print a + before printing -
        if (i != m_col)     // Prevents printing a - after the last element
            cout << "-";    // Print a - after every + (except after last element)
    }
    cout << endl; // Separates consecutive displays (if any)
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if (lowestEmpty(column) == -1)          // If lowestEmpty returned -1, return false because there are no vacancies
    {
        //cerr << "Column is full." << endl;
        return false;
    }

    if (color == RED)
        grid[lowestEmpty(column)][column - 1] = 'R';    // If color is RED, replace vacant element with an R
    else if (color == BLACK)
        grid[lowestEmpty(column)][column - 1] = 'B';    // If color is BLACK, replace vacant element with an R
    else
        return false;   // If color is not BLACK or RED, return false

    Pair a(lowestEmpty(column), column - 1);    // Save move in stack
    pairStack.push(a);
    return true;    // Return true
}

int ScaffoldImpl::undoMove()
{
    if (numberEmpty() == (m_levels * m_col))        // If scaffold is empty, return 0
        return 0;

    Pair a = pairStack.top();

    grid[a.m_last_level() + 1][a.m_last_col()] = ' ';     // Make spot empty
    {
        int x = a.m_last_col();     // Set m_last_col equal to a temp variable
        pairStack.pop();            // Pop stack
        return x + 1;           // Return saved last col
    }
}

int ScaffoldImpl::lowestEmpty(int column) const
{
    bool atLeastOneVacancy = false;     // Bool for indicating whether there is at least one vacancy
    int count = 0;      // Counter that increments when a BLACK or RED checker is found

    for (int i = 0; i < m_levels; i++)
    {
        if (grid[i][column - 1] == ' ')       // If element is vacant, make atLeastOneVacancy true
            atLeastOneVacancy = true;

        if (atLeastOneVacancy == true)      // Only do this process if at least one vacancy found
            if (grid[i][column - 1] != ' ')   // Increment count when a spot is not vacant
                count++;
    }

    if (atLeastOneVacancy == true)          // Only return this when at least one vacancy
        return (m_levels - 1 - count);      // Returns the amount of rows (subtracted by 1 due to interface) by counter

    return (-1);    // If there was no vacancy in column, return -1
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}

Scaffold::~Scaffold()
{
    delete m_impl;
}

Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}

Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}

int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}

void Scaffold::display() const
{
    m_impl->display();
}

bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}

int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
