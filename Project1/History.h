#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"
#include <string>

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int r;
	int c;
	char m_history[MAXROWS][MAXCOLS];
	std::string options = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif