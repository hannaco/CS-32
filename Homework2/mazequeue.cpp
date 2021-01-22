#include <queue>
#include <string>
#include <iostream>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	Coord start(sr, sc);

	queue<Coord> traveled;

	traveled.push(start);
	maze[sr][sc] = '&';

	while (!traveled.empty())
	{
		Coord cur = traveled.front();
		traveled.pop();
		if (cur.r() == er && cur.c() == ec)
			return true;
		if (cur.c() - 1 >= 0 && cur.c() - 1 < nCols && maze[cur.r()][cur.c() - 1] == '.')
		{
			maze[cur.r()][cur.c() - 1] = '&';
			traveled.push(Coord(cur.r(), cur.c() - 1));
		}
		if (cur.c() + 1 >= 0 && cur.c() + 1 < nCols && maze[cur.r()][cur.c() + 1] == '.')
		{
			maze[cur.r()][cur.c() + 1] = '&';
			traveled.push(Coord(cur.r(), cur.c() + 1));
		}
		if (cur.r() - 1 >= 0 && cur.r() - 1 < nRows && maze[cur.r() - 1][cur.c()] == '.')
		{
			maze[cur.r() - 1][cur.c()] = '&';
			traveled.push(Coord(cur.r() - 1, cur.c()));
		}
		if (cur.r() + 1 >= 0 && cur.r() + 1 < nRows && maze[cur.r() + 1][cur.c()] == '.')
		{
			maze[cur.r() + 1][cur.c()] = '&';
			traveled.push(Coord(cur.r() + 1, cur.c()));
		}
	}

	return false;
}


int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X.XXX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}