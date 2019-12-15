// TicTacToe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TicTacToe.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL GetGameBoardRect(HWND, RECT*);
void DrawLine(HDC, int, int, int, int);
int GetCellNumberFromPoint(HWND, int, int);
BOOL GetCellRect(HWND, int, RECT*);
int GetWinner(int[3]);
void DrawCenteredIcon(HDC, RECT*, HICON);
void Game(HWND, LPARAM);

const int CELL_SIZE = 100;
int playerTurn = 1;
int winner = 0;
int wins[3];
int gameBoard[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
HICON hIcon1, hIcon2;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TICTACTOE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      500, 200, 314, 358, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: {
		hIcon1 = LoadIcon(hInst, MAKEINTRESOURCE(ID_CROSS_ICO));
		hIcon2 = LoadIcon(hInst, MAKEINTRESOURCE(ID_CIRCLE_ICO));

		break;
	}
    case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEWGAME: {	
			playerTurn = 1;
			winner = 0;
			ZeroMemory(gameBoard, sizeof(gameBoard));
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}
		case IDM_ABOUT: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_EXIT: {
			DestroyWindow(hWnd);
			break;
		}
		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		}
		break;
	}
	case WM_LBUTTONDOWN: {
		if (playerTurn == 0) {
			break;
		}
		if (playerTurn == 2) {
			MessageBox(hWnd, L"It's players' 2 turn", L"Info", MB_OK);
			break;
		}

		Game(hWnd, lParam);
		
		break;
	}
	case WM_RBUTTONDOWN: {
		if (playerTurn == 0) {
			break;
		}
		if (playerTurn == 1) {
			MessageBox(hWnd, L"It's players' 1 turn", L"Info", MB_OK);
			break;
		}

		Game(hWnd, lParam);
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT r;

		if (!GetGameBoardRect(hWnd, &r)) {
			MessageBox(hWnd, L"Getting rectangle failed", L"Error", MB_OK);
			break;
		}

		FillRect(hdc, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// drawing the borders
		for (int i = 0; i < 4; i++) {
			DrawLine(hdc, r.left + CELL_SIZE * i, r.top, r.left + CELL_SIZE * i, r.bottom);
			DrawLine(hdc, r.left, r.top + CELL_SIZE * i, r.right, r.top + CELL_SIZE * i);
		}

		RECT rCell;

		// filling cells after resizing the window
		for (int i = 0; i < ARRAYSIZE(gameBoard); i++) {
			if (gameBoard[i] != 0 && GetCellRect(hWnd, i, &rCell)) {
				DrawCenteredIcon(hdc, &rCell, gameBoard[i] == 1 ? hIcon1 : hIcon2);
			}
		}
		
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY: {
		DestroyIcon(hIcon1);
		DestroyIcon(hIcon2);

		PostQuitMessage(0);
		break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    }
    return 0;
}

BOOL GetGameBoardRect(HWND hwnd, RECT * pRect) {
	RECT r;

	if (GetClientRect(hwnd, &r)) {
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		pRect->left = (width - CELL_SIZE * 3) / 2;
		pRect->top = (height - CELL_SIZE * 3) / 2;

		pRect->right = pRect->left + CELL_SIZE * 3;
		pRect->bottom = pRect->top + CELL_SIZE * 3;
		
		return TRUE;
	}

	SetRectEmpty(pRect);
	return FALSE;
}
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
int GetCellNumberFromPoint(HWND hwnd, int x, int y) {
	RECT r;
	POINT pt;

	pt.x = x;
	pt.y = y;

	if (GetGameBoardRect(hwnd, &r)) {
		if (PtInRect(&r, pt)) {
			x = pt.x - r.left;
			y = pt.y - r.top;

			int column = x / CELL_SIZE;
			int row = y / CELL_SIZE;

			return column + row * 3;
		}
	}

	return -1;
}
BOOL GetCellRect(HWND hwnd, int index, RECT * pRect) {
	RECT rBoard;

	SetRectEmpty(pRect);

	if (index < 0 || index > 8) {
		return FALSE;
	}

	if (GetGameBoardRect(hwnd, &rBoard)) {
		int row = index / 3;
		int column = index % 3;

		pRect->left = rBoard.left + column * CELL_SIZE + 1;
		pRect->top = rBoard.top + row * CELL_SIZE + 1;
		pRect->right = pRect->left + CELL_SIZE - 1;
		pRect->bottom = pRect->top + CELL_SIZE - 1;

		return TRUE;
	}

	return FALSE;
}
int GetWinner(int wins[3]) {
	int cells[] = { 0, 1, 2,  3, 4, 5,  6, 7, 8,  0, 3, 6,  1, 4, 7,  2, 5, 8,  0, 4, 8,  2, 4, 6 };

	for (int i = 0; i < ARRAYSIZE(cells); i += 3) {
		if (gameBoard[cells[i]] != 0) {
			if (gameBoard[cells[i]] == gameBoard[cells[i + 1]] && gameBoard[cells[i]] == gameBoard[cells[i + 2]]) {
				wins[0] = cells[i];
				wins[1] = cells[i + 1];
				wins[2] = cells[i + 2];

				return gameBoard[cells[i]];
			}
		}
	}

	for (int i = 0; i < ARRAYSIZE(gameBoard); i++) {
		if (gameBoard[i] == 0) {
			return 0;
		}
	}
	
	return 3;
}
void DrawCenteredIcon(HDC hdc, RECT * pRect, HICON hIcon) {
	const int ICON_WIDTH = 32;
	const int ICON_HEIGHT = 32;

	if (pRect != NULL) {
		int left = pRect->left + (pRect->right - pRect->left - ICON_WIDTH) / 2;
		int top = pRect->top + (pRect->bottom - pRect->top - ICON_HEIGHT) / 2;

		DrawIcon(hdc, left, top, hIcon);
	}
}
void Game(HWND hWnd, LPARAM lParam) {
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	int index = GetCellNumberFromPoint(hWnd, xPos, yPos);

	if (index != -1) {
		RECT rCell;
		HDC hdc = GetDC(hWnd);

		if (gameBoard[index] == 0 && GetCellRect(hWnd, index, &rCell)) {
			gameBoard[index] = playerTurn;

			//DrawIcon(hdc, rCell.left, rCell.top, playerTurn == 1 ? hIcon1 : hIcon2);
			DrawCenteredIcon(hdc, &rCell, playerTurn == 1 ? hIcon1 : hIcon2);

			winner = GetWinner(wins);

			if (winner == 1 || winner == 2) {
				MessageBox(hWnd, winner == 1 ? L"Player 1 won" : L"Player 2 won", L"Congratulations", MB_OK);
				playerTurn = 0;
			}
			else if (winner == 3) {
				MessageBox(hWnd, L"It's a draw", L"Info", MB_OK);
				playerTurn = 0;
			}
			else if (winner == 0) {
				playerTurn = playerTurn == 1 ? 2 : 1;
			}
		}

		ReleaseDC(hWnd, hdc);
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
