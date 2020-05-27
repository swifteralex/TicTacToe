#pragma once
#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "BaseWindow.h"
#include "TicTacToeGrid.h"

class MainWindow : public BaseWindow<MainWindow> {
private:
	ID2D1Factory* pFactory = 0;
	ID2D1HwndRenderTarget* pRenderTarget = 0;
	ID2D1SolidColorBrush* pBrush = 0;
	bool firstTimeWindowActivation = true;
	char currentTurn = 'x'; // what turn the game is currently at
	bool isHumanTurn = false; // if the human player is allowed to go
	int lastHighlightedIndex = 0;
	TicTacToeGrid grid;

public:
	PCWSTR ClassName() const { return L"Basic Window Class"; }
	void HighlightWinningCombo(char mark);
	void SetUpGame(); 
	bool CheckIfGameOver(); // checks if the game has ended and sets up a new one if so
	void UpdateCurrentTurn(); // changes currentTurn
	void DrawNewMark(char mark, int index, D2D1_COLOR_F color); // draws a new 'x' or 'o'
	void DrawGrid(); // draws the background and grid lines
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};