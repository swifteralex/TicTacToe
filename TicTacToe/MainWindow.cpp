#include "MainWindow.h"

void MainWindow::HighlightWinningCombo(char mark) {
	//check rows and columns
	int rowSum = 0;
	int colSum = 0;
	for (int i = 0; i < 9; i++) {
		rowSum = rowSum + grid.getMarkAtIndex(i);
		colSum = colSum + grid.getMarkAtIndex((3 * i) % 9 + i / 3);
		if (i % 3 == 2) {
			if (rowSum / 3 == mark) {
				DrawNewMark(mark, i, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				DrawNewMark(mark, i - 1, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				DrawNewMark(mark, i - 2, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				return;
			} else if (colSum / 3 == mark) {
				DrawNewMark(mark, (3 * (i-2)) % 9 + (i-2) / 3, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				DrawNewMark(mark, (3 * (i-1)) % 9 + (i-1) / 3, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				DrawNewMark(mark, (3 * i) % 9 + i / 3, D2D1::ColorF(0.75F, 0.0F, 0.0F));
				return;
			}
			rowSum = 0;
			colSum = 0;
		}
	}

	// check diagonals
	if (grid.getMarkAtIndex(0) == mark && grid.getMarkAtIndex(4) == mark && grid.getMarkAtIndex(8) == mark) {
		DrawNewMark(mark, 0, D2D1::ColorF(0.75F, 0.0F, 0.0F));
		DrawNewMark(mark, 4, D2D1::ColorF(0.75F, 0.0F, 0.0F));
		DrawNewMark(mark, 8, D2D1::ColorF(0.75F, 0.0F, 0.0F));
	} else if (grid.getMarkAtIndex(2) == mark && grid.getMarkAtIndex(4) == mark && grid.getMarkAtIndex(6) == mark) {
		DrawNewMark(mark, 2, D2D1::ColorF(0.75F, 0.0F, 0.0F));
		DrawNewMark(mark, 4, D2D1::ColorF(0.75F, 0.0F, 0.0F));
		DrawNewMark(mark, 6, D2D1::ColorF(0.75F, 0.0F, 0.0F));
	}
}

void MainWindow::SetUpGame() {
	DrawGrid();
	grid.clearGrid();
	pBrush->SetColor(D2D1::ColorF(0.05F, 0.23F, 0.3F));

	if (MessageBox(m_hwnd, L"Would you like to go first?", L"Choose a turn", MB_YESNO) == IDNO) {
		DrawNewMark(currentTurn, grid.getBestNextMove(currentTurn), D2D1::ColorF(0.05F, 0.23F, 0.3F));
	}
	isHumanTurn = true;
}

bool MainWindow::CheckIfGameOver() {
	char winner = grid.getWinner();
	if (grid.getNumMarks() == 9 && winner == 0) {
		if (MessageBox(m_hwnd, L"Draw!! Would you like to play again?", L"Game Over", MB_YESNO) == IDYES) {
			SetUpGame();
		}
		return true;
	}

	if (winner == 'x') {
		HighlightWinningCombo('x');
		if (MessageBox(m_hwnd, L"X's win!! Would you like to play again?", L"Game Over", MB_YESNO) == IDYES) {
			SetUpGame();
		}
		return true;
	} else if (winner == 'o') {
		HighlightWinningCombo('o');
		if (MessageBox(m_hwnd, L"O's win!! Would you like to play again?", L"Game Over", MB_YESNO) == IDYES) {
			SetUpGame();
		}
		return true;
	}

	return false;
}

void MainWindow::UpdateCurrentTurn() {
	(currentTurn == 'x') ? currentTurn = 'o' : currentTurn = 'x';
}

void MainWindow::DrawNewMark(char mark, int index, D2D1_COLOR_F color) {
	D2D1_POINT_2F centerOfSquareAtIndex;
	centerOfSquareAtIndex.x = (index % 3) * 200 + 100;
	centerOfSquareAtIndex.y = (index / 3) * 200 + 100;

	pRenderTarget->BeginDraw();
	D2D1_RECT_F rectangle = D2D1::RectF(centerOfSquareAtIndex.x - 90, centerOfSquareAtIndex.y - 90,
		centerOfSquareAtIndex.x + 90, centerOfSquareAtIndex.y + 90);
	pBrush->SetColor(D2D1::ColorF(0.61F, 0.89F, 1.0F));
	pRenderTarget->FillRectangle(&rectangle, pBrush);
	pBrush->SetColor(color);
	if (mark == 'o') {
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(centerOfSquareAtIndex, 60, 60);
		pRenderTarget->DrawEllipse(ellipse, pBrush, 16, NULL);
	} else {
		pRenderTarget->DrawLine(D2D1::Point2F(centerOfSquareAtIndex.x - 60, centerOfSquareAtIndex.y - 60),
			D2D1::Point2F(centerOfSquareAtIndex.x + 60, centerOfSquareAtIndex.y + 60), pBrush, 16, NULL);
		pRenderTarget->DrawLine(D2D1::Point2F(centerOfSquareAtIndex.x + 60, centerOfSquareAtIndex.y - 60),
			D2D1::Point2F(centerOfSquareAtIndex.x - 60, centerOfSquareAtIndex.y + 60), pBrush, 16, NULL);
	}
	pRenderTarget->EndDraw();

	UpdateCurrentTurn();
}

void MainWindow::DrawGrid() {
	pRenderTarget->BeginDraw();
	D2D1_RECT_F rectangle = D2D1::RectF(0, 0, 600, 600);
	pBrush->SetColor(D2D1::ColorF(0.61F, 0.89F, 1.0F));
	pRenderTarget->FillRectangle(&rectangle, pBrush);
	pBrush->SetColor(D2D1::ColorF(0.50F, 0.78F, 0.89F));
	pRenderTarget->DrawLine(D2D1::Point2F(200, 20), D2D1::Point2F(200, 580), pBrush, 8, NULL);
	pRenderTarget->DrawLine(D2D1::Point2F(400, 20), D2D1::Point2F(400, 580), pBrush, 8, NULL);
	pRenderTarget->DrawLine(D2D1::Point2F(20, 200), D2D1::Point2F(580, 200), pBrush, 8, NULL);
	pRenderTarget->DrawLine(D2D1::Point2F(20, 400), D2D1::Point2F(580, 400), pBrush, 8, NULL);
	pRenderTarget->EndDraw();
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_ACTIVATEAPP: {
		if (firstTimeWindowActivation) {
			firstTimeWindowActivation = false;
			SetUpGame();
		}
	}

	case WM_SETCURSOR: {
		if (LOWORD(lParam) == HTCLIENT) {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return TRUE;
		}
		break;
	}

	case WM_LBUTTONDOWN: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (isHumanTurn) {
			isHumanTurn = false;
			int indexClickedOn = x / 200 + (y / 200) * 3;
			if (grid.getMarkAtIndex(indexClickedOn) != 0) {
				isHumanTurn = true;
				return 0;
			}
			grid.newMark(currentTurn, indexClickedOn);
			DrawNewMark(currentTurn, indexClickedOn, D2D1::ColorF(0.05F, 0.23F, 0.3F));
			if (CheckIfGameOver())
				return 0;
			DrawNewMark(currentTurn, grid.getBestNextMove(currentTurn), D2D1::ColorF(0.05F, 0.23F, 0.3F));
			if (CheckIfGameOver())
				return 0;
			isHumanTurn = true;
		}
	}

	case WM_MOUSEMOVE: {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (isHumanTurn) {
			pRenderTarget->BeginDraw();
			int indexHoveredOn = x / 200 + (y / 200) * 3;
			D2D1_POINT_2F centerOfSquareAtIndex;
			if (grid.getMarkAtIndex(lastHighlightedIndex) == 0) {
				centerOfSquareAtIndex.x = (lastHighlightedIndex % 3) * 200 + 100;
				centerOfSquareAtIndex.y = (lastHighlightedIndex / 3) * 200 + 100;
				D2D1_RECT_F rectangle = D2D1::RectF(centerOfSquareAtIndex.x - 90, centerOfSquareAtIndex.y - 90,
					centerOfSquareAtIndex.x + 90, centerOfSquareAtIndex.y + 90);
				pBrush->SetColor(D2D1::ColorF(0.61F, 0.89F, 1.0F));
				pRenderTarget->FillRectangle(&rectangle, pBrush);
			}
			if (grid.getMarkAtIndex(indexHoveredOn) == 0) {
				centerOfSquareAtIndex.x = (indexHoveredOn % 3) * 200 + 100;
				centerOfSquareAtIndex.y = (indexHoveredOn / 3) * 200 + 100;
				D2D1_RECT_F rectangle = D2D1::RectF(centerOfSquareAtIndex.x - 90, centerOfSquareAtIndex.y - 90,
					centerOfSquareAtIndex.x + 90, centerOfSquareAtIndex.y + 90);
				pBrush->SetColor(D2D1::ColorF(0.57F, 0.85F, 0.96F));
				pRenderTarget->FillRectangle(&rectangle, pBrush);
				lastHighlightedIndex = indexHoveredOn;
			} 
			pRenderTarget->EndDraw();
		}
		return 0;
	}

	case WM_CREATE: {
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
			return -1;
		}
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0F, 0.0F, 0.0F), &pBrush);

		return 0;
	}

	case WM_GETMINMAXINFO: {
		MINMAXINFO* info = (MINMAXINFO*)lParam;
		info->ptMaxTrackSize.x = 616;
		info->ptMaxTrackSize.y = 638;
		info->ptMinTrackSize.x = 616;
		info->ptMinTrackSize.y = 638;
		return 0;
	}

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	default: {
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}

	}
	return TRUE;
}