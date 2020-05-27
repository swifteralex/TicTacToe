#pragma once
#include <vector>

using namespace std;

class TicTacToeGrid {
public:
	TicTacToeGrid();
	TicTacToeGrid(char grid[9]);
	void clearGrid(); // sets every mark to 0 and sets numMarks to 0
	void newMark(char mark, int index); // fills in an 'x' or 'o' at the respective index
	char getWinner() const; // returns the winning player in a grid, or 0 if no winner
	int getBestNextMove(int turn); // marks and returns the index of the best possible move, depending on the turn
	int getNumMarks() const;
	char getMarkAtIndex(int index) const;

private:
	char grid[9]; // stores the state of the board, indices 0-2 are the first row, 3-5 the second, 6-8 the third
	int numMarks;
	char calculateNextMove(int turn, bool isFirstPosition); // fills in the best move to make, depending on the turn
};