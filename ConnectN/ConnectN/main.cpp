#include "provided.h"
#include <iostream>
#include <cassert>
using namespace std;



int main()
{
	int cols, rows, toWin;

	HumanPlayer hp("Ryan");
	BadPlayer sp("Smart");

	cout << "Enter amount of columns: ";
	cin >> cols;

	cout << "Enter amount of rows: ";
	cin >> rows;

	cout << "Enter amount needed to win the game: ";
	cin >> toWin;

	cout << endl;

	Game g(cols, rows, toWin, &hp, &sp);
	g.play();
}