// lab1.cpp : Defines the entry point for the application.
//
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include "lab3.h"
#include <comdef.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				UserWindow1(HINSTANCE hInstance);
ATOM				UserWindow2(HINSTANCE hInstance);
ATOM				UserWindow3(HINSTANCE hInstance);
ATOM				Util(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc3(HWND, UINT, WPARAM, LPARAM);

//DWORD dwWindowStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CHILD | WS_SYSMENU;
DWORD dwWindowStyle = WS_CHILDWINDOW | WS_VISIBLE| WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS;
DWORD dwWindowMode = SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOZORDER | SWP_DRAWFRAME;

HWND window1;
HWND window2;
HWND window3;

HWND keyState;
HWND keyCodeVirtual;
HWND keyCodeOEM;
HWND keyCodeANSI;
HWND keyCodeASCII;
HWND keySymbol;

HWND mouseX;
HWND mouseY;
HWND mouseState1;
HWND mouseState2;

HWND timerTime1;
HWND timerTime2;
HWND timerTime3;
HWND timerState1;
HWND timerState2;
HWND timerState3;

HGDIOBJ brushBG1 = CreateSolidBrush(RGB(255, 0, 0));
HGDIOBJ brushBG2 = CreateSolidBrush(RGB(0, 255, 0));
HGDIOBJ brushBG3 = CreateSolidBrush(RGB(0, 0, 255));

int dx;
int dy;

int const timerID1 = 1;
int const timerID2 = 2;
int const timerID3 = 3;
int const timerID4 = 4;

int delta1 = 3000;
int delta2 = 6000;
int delta3 = 9000;
int delta4 = 1000;

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
	LoadStringW(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);
	UserWindow1(hInstance);
	UserWindow2(hInstance);
	UserWindow3(hInstance);
	Util(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
ATOM UserWindow1(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc1;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)brushBG1;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = L"UserWindow1";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
ATOM UserWindow2(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)brushBG2;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = L"UserWindow2";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
ATOM UserWindow3(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc3;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_HELP);
	wcex.hbrBackground = (HBRUSH)brushBG3;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = L"UserWindow3";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
ATOM Util(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc3;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3);
	wcex.lpszClassName = L"Util";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		20, 20, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT r;

	GetClientRect(hWnd, &r);

	dx = r.right;
	dy = r.bottom;

	window1 = CreateWindow(L"UserWindow1", L"Window1", dwWindowStyle,
		0, 0, dx, dy / 3, hWnd, nullptr, hInst, nullptr);

	//if (!window1) {
	//	MessageBox(NULL, L"Somthing is wrong with window1", L"Error", MB_OK);
	//}

	ShowWindow(window1, nCmdShow);
	UpdateWindow(window1);

	window2 = CreateWindow(L"UserWindow2", L"Window2", dwWindowStyle,
		0, dy / 3, dx / 2, dy / 3, hWnd, nullptr, hInst, nullptr);

	//if (!window2) {
	//	MessageBox(NULL, L"Something is wrong with window2", L"Error", MB_OK);
	//}

	ShowWindow(window2, SW_HIDE);
	UpdateWindow(window2);

	window3 = CreateWindow(L"UserWindow3", L"Window3", dwWindowStyle,
		dx / 2, dy / 3, dx / 2, dy / 3, hWnd, nullptr, hInst, nullptr);

	//if (!window3) {
	//	MessageBox(NULL, L"Something is wrong with window3", L"Error", MB_OK);
	//}

	ShowWindow(window3, SW_SHOW);
	UpdateWindow(window3);

	RECT r1;
	RECT r2;
	RECT r3;

	GetClientRect(window1, &r1);
	GetClientRect(window2, &r2);
	GetClientRect(window3, &r3);

	int dx1 = r1.right;
	int dy1 = r1.bottom;
	int dx2 = r2.right;
	int dy2 = r2.bottom;
	int dx3 = r3.right;
	int dy3 = r3.bottom;

	int length1 = dx1 / 3;
	int height1 = dy1 / 6;
	int length2 = length1;
	int height2 = height1;
	int length3 = length1;
	int height3 = height1;

	DWORD chWinStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CLIPSIBLINGS | WS_BORDER;

	{
		keyState = CreateWindow(L"Edit", L"Key State: None", chWinStyle,
			0, 0, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keyState, SW_SHOW);
		UpdateWindow(keyState);

		keyCodeVirtual = CreateWindow(L"Edit", L"Key virtual code: None", chWinStyle,
			0, height1, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keyCodeVirtual, SW_SHOW);
		UpdateWindow(keyCodeVirtual);

		keyCodeOEM = CreateWindow(L"Edit", L"Key OEM Code: None", chWinStyle,
			0, height1 * 2, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keyCodeOEM, SW_SHOW);
		UpdateWindow(keyCodeOEM);

		keyCodeANSI = CreateWindow(L"Edit", L"Key ANSI Code: None", chWinStyle,
			0, height1 * 3, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keyCodeANSI, SW_SHOW);
		UpdateWindow(keyCodeANSI);

		keyCodeASCII = CreateWindow(L"Edit", L"Key ASCII Code: None", chWinStyle,
			0, height1 * 4, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keyCodeASCII, SW_SHOW);
		UpdateWindow(keyCodeASCII);

		keySymbol = CreateWindow(L"Edit", L"Key Symbol: None", chWinStyle,
			0, height1 * 5, length1, height1, window1, nullptr, hInst, nullptr);

		ShowWindow(keySymbol, SW_SHOW);
		UpdateWindow(keySymbol);
	}
	
	{
		mouseX = CreateWindow(L"Edit", L"Mouse X: ", chWinStyle,
			0, 0, length2, height2, window2, nullptr, hInst, nullptr);

		ShowWindow(mouseX, SW_SHOW);
		UpdateWindow(mouseX);

		mouseY = CreateWindow(L"Edit", L"Mouse Y: ", chWinStyle,
			0, height2, length2, height2, window2, nullptr, hInst, nullptr);

		ShowWindow(mouseY, SW_SHOW);
		UpdateWindow(mouseY);

		mouseState1 = CreateWindow(L"Edit", L"Mouse State 1: ", chWinStyle,
			0, height2 * 2, length2, height2, window2, nullptr, hInst, nullptr);

		ShowWindow(mouseState1, SW_SHOW);
		UpdateWindow(mouseState1);

		mouseState2 = CreateWindow(L"Edit", L"Mouse State 2: ", chWinStyle,
			0, height2 * 3, length2, height2, window2, nullptr, hInst, nullptr);

		ShowWindow(mouseState2, SW_SHOW);
		UpdateWindow(mouseState2);
	}
	
	{
		/*timerTime1 = CreateWindow(L"Edit", L"Timer Time 1: ", chWinStyle,
			0, 0, length3, height3, window3, nullptr, hInst, nullptr);

		ShowWindow(timerTime1, SW_SHOW);
		UpdateWindow(timerTime1);

		timerTime2 = CreateWindow(L"Edit", L"Timer Time 2: ", chWinStyle,
			0, height3, length3, height3, window3, nullptr, hInst, nullptr);

		ShowWindow(timerTime2, SW_SHOW);
		UpdateWindow(timerTime2);

		timerTime3 = CreateWindow(L"Edit", L"Timer Time 3: ", chWinStyle,
			0, height3 * 2, length3, height3, window3, nullptr, hInst, nullptr);*/

		ShowWindow(timerTime3, SW_SHOW);
		UpdateWindow(timerTime3);

		timerState1 = CreateWindow(L"Edit", L"Timer State 1: ", chWinStyle,
			0, 0, length3, height3, window3, nullptr, hInst, nullptr);

		ShowWindow(timerState1, SW_SHOW);
		UpdateWindow(timerState1);

		timerState2 = CreateWindow(L"Edit", L"Timer State 2: ", chWinStyle,
			0, height3, length3, height3, window3, nullptr, hInst, nullptr);

		ShowWindow(timerState2, SW_SHOW);
		UpdateWindow(timerState2);

		timerState3 = CreateWindow(L"Edit", L"Timer State 3: ", chWinStyle,
			0, height3 * 2, length3, height3, window3, nullptr, hInst, nullptr);

		ShowWindow(timerState3, SW_SHOW);
		UpdateWindow(timerState3);
	}

	SendMessage(keyState, WM_SETTEXT, 0, (LPARAM)L"Key State: None");
	SendMessage(keyCodeVirtual, WM_SETTEXT, 0, (LPARAM)L"Key Virtual Code: None");
	SendMessage(keyCodeOEM, WM_SETTEXT, 0, (LPARAM)L"Key OEM Code: None");
	SendMessage(keyCodeANSI, WM_SETTEXT, 0, (LPARAM)L"key ANSI Code: None");
	SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)L"Key ASCII Code: None");
	SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)L"Key Symbol: None");

	SendMessage(mouseX, WM_SETTEXT, 0, (LPARAM)L"Mouse X: ");
	SendMessage(mouseY, WM_SETTEXT, 0, (LPARAM)L"Mouse Y: ");
	SendMessage(mouseState1, WM_SETTEXT, 0, (LPARAM)L"Mouse State 1: None");
	SendMessage(mouseState2, WM_SETTEXT, 0, (LPARAM)L"Mouse State 2: None");

	/*SendMessage(timerTime1, WM_SETTEXT, 0, (LPARAM)L"Timer Time 1: ");
	SendMessage(timerTime2, WM_SETTEXT, 0, (LPARAM)L"Timer Time 2: ");
	SendMessage(timerTime3, WM_SETTEXT, 0, (LPARAM)L"Timer Time 3: ");*/
	SendMessage(timerState1, WM_SETTEXT, 0, (LPARAM)L"Timer State 1: Active");
	SendMessage(timerState2, WM_SETTEXT, 0, (LPARAM)L"Timer State 2: Active");
	SendMessage(timerState3, WM_SETTEXT, 0, (LPARAM)L"Timer State 3: Active");

	SetTimer(hWnd, timerID1, delta1, NULL);
	SetTimer(hWnd, timerID2, delta2, NULL);
	SetTimer(hWnd, timerID3, delta3, NULL);

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
	char a[2];

	static wchar_t vCode[50] = L"Key Virtual Code: ";
	static wchar_t OEMCode[50] = L"Key OEM Code: ";
	static wchar_t ANSICode[50] = L"Key ANSI Code: ";
	static wchar_t ASCIICode[50] = L"Key ANCII Code: ";
	static wchar_t symbol[50] = L"Key Symbol: ";

	static wchar_t mX[50] = L"Mouse X: ";
	static wchar_t mY[50] = L"Mouse Y: ";

	/*static wchar_t t1[50] = L"Timer Time 1: ";
	static wchar_t t2[50] = L"Timer Time 2: ";
	static wchar_t t3[50] = L"Timer Time 3: ";*/

	static int x = 0;
	static int y = 0;

	static int time1 = 0;
	static int time2 = 0;
	static int time3 = 0;

	SetFocus(timerState3);

	switch (message)
	{
	case WM_KEYDOWN:
	{
		symbol[12] = wParam;
		a[0] = (int)lParam >> 16;
		a[1] = '\0';
		CharToOem((LPCWSTR)a, (LPSTR)a);
		
		swprintf(&vCode[18], L"%d", VkKeyScan(wParam));
		swprintf(&OEMCode[14], L"%d", a[0]);
		swprintf(&ANSICode[15], L"%d", wParam);
		swprintf(&ASCIICode[16], L"%d", wParam);

		if ((int)lParam >> 16 == 42) {			
			SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)L"Key Symbol: LShift");
		}
		else if ((int)lParam >> 16 == 54) {
			SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)L"Key Symbol: RShift");
		}

		symbol[12] = wParam;

		SendMessage(keyState, WM_SETTEXT, 0, (LPARAM)L"Key State: Pressed");
		SendMessage(keyCodeVirtual, WM_SETTEXT, 0, (LPARAM)vCode);
		SendMessage(keyCodeOEM, WM_SETTEXT, 0, (LPARAM)OEMCode);
		SendMessage(keyCodeANSI, WM_SETTEXT, 0, (LPARAM)ANSICode);
		SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)ASCIICode);
		SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)symbol);

		break;
	}
	case WM_KEYUP:
	{
		SendMessage(keyState, WM_SETTEXT, 0, (LPARAM)L"Key State: None");
		SendMessage(keyCodeVirtual, WM_SETTEXT, 0, (LPARAM)L"Key Virtual Code: None");
		SendMessage(keyCodeOEM, WM_SETTEXT, 0, (LPARAM)L"Key OEM Code: None");
		SendMessage(keyCodeANSI, WM_SETTEXT, 0, (LPARAM)L"key ANSI Code: None");
		SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)L"Key ASCII Code: None");
		SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)L"Key Symbol: None");

		break;
	}
	case WM_CHAR:
	{
		symbol[12] = wParam;
		SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)symbol);
		break;
	}

	case WM_SYSKEYDOWN:
	{
		symbol[12] = wParam;
		a[0] = wParam;
		a[1] = '\0';
		CharToOem((LPCWSTR)a, (LPSTR)a);

		swprintf(&vCode[18], L"%d", VkKeyScan(wParam));
		swprintf(&OEMCode[14], L"%d", a[0]);
		swprintf(&ANSICode[15], L"%d", wParam);

		if (wParam > 0 && wParam <= 127) {
			swprintf(&ASCIICode[16], L"%d", wParam);
			SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)ASCIICode);
		}
		
		symbol[12] = wParam;
		
		SendMessage(keyState, WM_SETTEXT, 0, (LPARAM)L"Key State: Pressed");
		SendMessage(keyCodeVirtual, WM_SETTEXT, 0, (LPARAM)vCode);
		SendMessage(keyCodeOEM, WM_SETTEXT, 0, (LPARAM)OEMCode);
		SendMessage(keyCodeANSI, WM_SETTEXT, 0, (LPARAM)ANSICode);
		SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)ASCIICode);
		SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)symbol);

		switch (wParam) {
		case 0x31:
			//if (GetAsyncKeyState(VK_MENU)) {
			DestroyWindow(window2);
			//}
			break;
		case '2':
			DestroyWindow(window3);
			break;
		}
		break;
	}
	case WM_SYSKEYUP:
	{
		SendMessage(keyState, WM_SETTEXT, 0, (LPARAM)L"Key State: None");
		SendMessage(keyCodeVirtual, WM_SETTEXT, 0, (LPARAM)L"Key Virtual Code: None");
		SendMessage(keyCodeOEM, WM_SETTEXT, 0, (LPARAM)L"Key OEM Code: None");
		SendMessage(keyCodeANSI, WM_SETTEXT, 0, (LPARAM)L"Key ANSI Code: None");
		SendMessage(keyCodeASCII, WM_SETTEXT, 0, (LPARAM)L"Key ASCII Code: None");
		SendMessage(keySymbol , WM_SETTEXT, 0, (LPARAM)L"Key Symbol: None");

		break;
	}
	case WM_SYSCHAR:
	{
		symbol[12] = wParam;
		SendMessage(keySymbol, WM_SETTEXT, 0, (LPARAM)symbol);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		SendMessage(mouseState1, WM_SETTEXT, 0, (LPARAM)L"Mouse State 1: Left Button Pressed");
		break;
	}
	case WM_LBUTTONUP:
	{
		SendMessage(mouseState1, WM_SETTEXT, 0, (LPARAM)L"Mouse State 1: None");
		break;
	}
	case WM_RBUTTONDOWN:
	{
		SendMessage(mouseState1, WM_SETTEXT, 0, (LPARAM)L"Mouse State 1: Right Button Pressed");
		break;
	}
	case WM_RBUTTONUP:
	{
		SendMessage(mouseState1, WM_SETTEXT, 0, (LPARAM)L"Mouse State 1: None");
		break;
	}
	case WM_MOUSEMOVE:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		swprintf(&mX[9], L"%d", x);
		swprintf(&mY[9], L"%d", y);

		SendMessage(mouseX, WM_SETTEXT, 0, (LPARAM)mX);
		SendMessage(mouseY, WM_SETTEXT, 0, (LPARAM)mY);
		SendMessage(mouseState2, WM_SETTEXT, 0, (LPARAM)L"Mouse State 2: In client widnow");
		break;
	}
	case WM_NCMOUSEMOVE:
	{	
		SendMessage(mouseState2, WM_SETTEXT, 0, (LPARAM)L"Mouse State 2: Not in client window");
		break;
	}
	
	case WM_TIMER:
	{

		/*if (timerID1) {
			time1++;
			swprintf(&t1[14], L"%d", time1);
			SendMessage(timerTime1, WM_SETTEXT, 0, (LPARAM)t1);
		}
		if (timerID2) {
			time2++;
			swprintf(&t2[14], L"%d", time2);
			SendMessage(timerTime2, WM_SETTEXT, 0, (LPARAM)t2);
		}
		if (timerID3) {
			time3++;
			swprintf(&t3[14], L"%d", time3);
			SendMessage(timerTime3, WM_SETTEXT, 0, (LPARAM)t3);
		}*/

		switch (wParam) {
		case timerID1:
		{
			ShowWindow(window2, SW_SHOW);
			ShowWindow(window3, SW_SHOW);
			KillTimer(hWnd, timerID1);

			SendMessage(timerState1, WM_SETTEXT, 0, (LPARAM)L"Timer State 1: Inactive");

			break;
		}
		case timerID2:
		{
			RECT r;
			GetClientRect(hWnd, &r);

			SetWindowPos(window1, 0, 0, r.bottom / 3 * 2, r.right, r.bottom / 3, SWP_SHOWWINDOW);
			KillTimer(hWnd, timerID2);

			SendMessage(timerState2, WM_SETTEXT, 0, (LPARAM)L"Timer State 2: Inactive");

			break;
		}
		case timerID3:
			long style = GetWindowLong(window3, GWL_STYLE);
			style = style ^ WS_OVERLAPPEDWINDOW;
			SetWindowLong(window3, GWL_STYLE, style);

			SendMessage(timerState3, WM_SETTEXT, 0, (LPARAM)L"Timer State 3: Inactive");

			KillTimer(hWnd, timerID3);
			break;
		}
		break;
	}
	case WM_EXITSIZEMOVE:
	{
		RECT r;

		GetClientRect(hWnd, &r);

		dx = r.right;
		dy = r.bottom;

		SetWindowPos(window1, 0, 0, 0, dx, dy / 3, SWP_SHOWWINDOW);
		SetWindowPos(window2, 0, 0, dy / 3, dx / 2, dy / 3, SWP_SHOWWINDOW);
		SetWindowPos(window3, 0, dx / 2, dy / 3, dx / 2, dy / 3, SWP_SHOWWINDOW);

		UpdateWindow(window1);
		UpdateWindow(window2);
		UpdateWindow(window3);

		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}


	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}
LRESULT CALLBACK WndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
