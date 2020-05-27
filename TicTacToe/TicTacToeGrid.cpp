#include <random>
#include "TicTacToeGrid.h"

TicTacToeGrid::TicTacToeGrid() : numMarks(0), grid() {}

TicTacToeGrid::TicTacToeGrid(char grid[9]) : numMarks(0), grid() {
	for (int i = 0; i < 9; i++) {
		if (grid[i] != 0)
			numMarks++;
		this->grid[i] = grid[i];
	}
}

void TicTacToeGrid::clearGrid() {
	for (int i = 0; i < 9; i++) {
		grid[i] = 0;
	}
	numMarks = 0;
}

void TicTacToeGrid::newMark(char mark, int index) {
	grid[index] = mark;
	numMarks++;
}

char TicTacToeGrid::getWinner() const {
	// check for three of a kind along rows and columns
	int rowSum = 0;
	int colSum = 0;
	for (int i = 0; i < 9; i++) {
		// sum entries in a row/column and divide by 3
		rowSum = rowSum + grid[i];
		colSum = colSum + grid[(3 * i) % 9 + i / 3];
		if (i % 3 == 2) {
			if (rowSum / 3 == 'x' || colSum / 3 == 'x') {
				return 'x';
			} else if (rowSum / 3 == 'o' || colSum / 3 == 'o') {
				return 'o';
			}
			rowSum = 0;
			colSum = 0;
		}
	}

	// check for three of a kind along diagonals
	if (grid[0] == 'x' && grid[4] == 'x' && grid[8] == 'x' ||
		grid[2] == 'x' && grid[4] == 'x' && grid[6] == 'x') {
		return 'x';
	}
	if (grid[0] == 'o' && grid[4] == 'o' && grid[8] == 'o' ||
		grid[2] == 'o' && grid[4] == 'o' && grid[6] == 'o') {
		return 'o';
	}

	return 0;
}

int TicTacToeGrid::getBestNextMove(int turn) {
	char pre[9];
	for (int i = 0; i < 9; i++) {
		pre[i] = grid[i];
	}
	calculateNextMove(turn, true);
	char post[9];
	for (int i = 0; i < 9; i++) {
		post[i] = grid[i];
	}

	for (int i = 0; i < 9; i++) {
		if (pre[i] != post[i]) {
			return i;
		}
	}
	return -1;
}

int TicTacToeGrid::getNumMarks() const {
	return numMarks;
}

char TicTacToeGrid::getMarkAtIndex(int index) const {
	return grid[index];
}

char TicTacToeGrid::calculateNextMove(int turn, bool isFirstPosition) {
	// base case
	char winner = getWinner();
	if (winner != 0)
		return winner;
	if (numMarks == 9)
		return 0;

	// generate possible boards from position
	vector<TicTacToeGrid*> boards;
	for (int i = 0; i < 9; i++) { // generate (9 - numMarks) boards
		while (grid[i] != 0 && i < 9) // get to the next empty mark on the grid
			i++;
		if (i > 8)
			break;
		TicTacToeGrid* newBoard = new TicTacToeGrid(grid);
		newBoard->newMark(turn, i);
		boards.push_back(newBoard);
	}

	// recursively calculate the winner of the generated boards
	char opponentTurn;
	if (turn == 'x') {
		opponentTurn = 'o';
	} else {
		opponentTurn = 'x';
	}
	int winningIndex = -1;
	int drawingIndex = -1;
	vector<int> winningIndices;
	vector<int> drawingIndices;
	for (unsigned i = 0; i < boards.size(); i++) {
		char winnerOfPosition = boards.at(i)->calculateNextMove(opponentTurn, false);
		if (winnerOfPosition == turn) {
			winningIndices.push_back(i);
			break;
		} else if (winnerOfPosition == 0) {
			drawingIndices.push_back(i);
		}
	}

	// update the first board with the best move
	if (isFirstPosition) {
		int bestBoardIndex;
		if (winningIndices.size() > 0) {
			bestBoardIndex = winningIndices.at(rand() % winningIndices.size());
		} else if (drawingIndices.size() > 0) {
			bestBoardIndex = drawingIndices.at(rand() % drawingIndices.size());
		} else {
			bestBoardIndex = 0;
		}
		for (int i = 0; i < 9; i++) {
			grid[i] = boards.at(bestBoardIndex)->grid[i];
		}
		numMarks++;
		return 0;
	}

	for (unsigned i = 0; i < boards.size(); i++) {
		delete boards.at(i);
	}

	if (winningIndices.size() > 0) {
		return turn;
	} else if (drawingIndices.size() > 0) {
		return 0;
	} else {
		return opponentTurn;
	}
}