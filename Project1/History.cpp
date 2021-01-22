using namespace std;

#include <iostream>

#include "History.h"

History::History(int nRows, int nCols)
{
	r = nRows;
	c = nCols;
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			m_history[i][j] = options[0];
		}
	}
}

bool History::record(int r, int c)
{
	if (r < 1 || c < 1 || r > MAXROWS || c > MAXCOLS)
		return false;
	else
	{
		int pos;
		for (int i = 0; i < options.size(); i++)
		{
			if (m_history[r - 1][c - 1] == options[i])
			{
				pos = i;
				break;
			}
		}
		m_history[r - 1][c - 1] = options[pos + 1];
		return true;
	}
}

void History::display() const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int row, col;

	// Fill displayGrid with dots
	for (row = 1; row <= r; row++)
		for (col = 1; col <= c; col++)
			displayGrid[row - 1][col - 1] = m_history[row - 1][col - 1];

	// Draw the grid
	clearScreen();
	for (row = 1; row <= r; row++)
	{
		for (col = 1; col <= c; col++)
			cout << displayGrid[row - 1][col - 1];
		cout << endl;
	}
	cout << endl;

}