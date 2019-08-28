// lab05.cpp : Defines the entry point for the application.
//

#include "header.h"
#include "lab05.h"
#include <string>

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

INT_PTR CALLBACK	CreateMainProcess(HWND, UINT, WPARAM, LPARAM);

HANDLE hFile;

using namespace std;

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
	LoadStringW(hInstance, IDC_LAB05, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB05));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB05));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB05);
	wcex.lpszClassName = szWindowClass;
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
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_PROCESS_CREATE:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATEPROCESS), hWnd, CreateMainProcess);
			break;
		}
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
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

INT_PTR CALLBACK CreateMainProcess(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPCWSTR program = L"C:\\Windows\\notepad.exe";

	LPCWSTR txtFile = L"D:\\study\\System programming\\lab05\\lab05\\files\\txtFile.txt";
	LPCWSTR binFile = L"D:\\study\\System programming\\lab05\\lab05\\files\\binFile.bin";

	WCHAR exeTxtFile[] = L"D:\\study\\System programming\\lab05\\lab05_txtFiles\\Debug\\lab05_txtFiles.exe";
	WCHAR exeBinFile[] = L"D:\\study\\System programming\\lab05\\lab05_binFiles\\Debug\\lab05_binFiles.exe";
	WCHAR cmdTxtParam[100];
	WCHAR cmdNumParam[sizeof(HANDLE)];
	WCHAR cmdBinParam[sizeof(HANDLE)];

	WCHAR cmdLineTxt[100] = L"\"C:\\Windows\\notepad.exe\" D:\\study\\System programming\\lab05\\lab05\\files\\txtFile.txt";
	WCHAR cmdLineBin[100] = L"\"C:\\Windows\\notepad.exe\" D:\\study\\System programming\\lab05\\lab05\\files\\binFile.bin";

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hTxtFile;
	HANDLE hBinFile;

	char text[] = "Some additional text\r\nto write ";
	int value = 15;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:
		switch (wParam) {
		case IDC_CREATE:
		{
			if ((hTxtFile = CreateFile(txtFile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, &sa,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Couldn't create text file", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}
			if ((hBinFile = CreateFile(binFile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, &sa,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Couldn't create bin file", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			WriteFile(hTxtFile, text, sizeof(text), 0, NULL);
			WriteFile(hBinFile, &value, sizeof(value), 0, NULL);
			WriteFile(hBinFile, &value, sizeof(value), 0, NULL);

			wsprintf(cmdTxtParam, L"%d", hTxtFile);
			wsprintf(cmdBinParam, L"%d", hBinFile);

			STARTUPINFO si = {
				sizeof(si)
			};
			PROCESS_INFORMATION pi[3];

			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			CreateProcess(exeTxtFile, cmdTxtParam, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi[0]);
			WaitForSingleObject(pi[0].hProcess, INFINITE);
			CreateProcess(exeBinFile, cmdBinParam, NULL, NULL, TRUE, ABOVE_NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi[1]);
			WaitForSingleObject(pi[1].hProcess, INFINITE);

			wsprintf(cmdNumParam, L"%d", hBinFile);

			CreateProcess(exeBinFile, cmdNumParam, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi[2]);
			WaitForSingleObject(pi[2].hProcess, INFINITE);

			SetPriorityClass(pi[1].hProcess, BELOW_NORMAL_PRIORITY_CLASS);

			HWND hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
			HWND hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
			HWND hEdit3 = GetDlgItem(hDlg, IDC_EDIT3);

			string oldTxt;
			wstring oldBin;
			DWORD readByte;

			SetFilePointer(hTxtFile, NULL, NULL, NULL);
			oldTxt = new CHAR[GetFileSize(hTxtFile, NULL)];
			int i = 0;
			do {
				CHAR value;
				ReadFile(hTxtFile, &value, sizeof(value), &readByte, NULL);
				oldTxt[i] = value;
				i++;
			} while (readByte != 0);
			SetWindowTextA(hEdit2, (string("Txt file after modding: ") + string(oldTxt)).c_str());

			SetFilePointer(hBinFile, NULL, NULL, NULL);
			do {
				ReadFile(hBinFile, &value, sizeof(value), &readByte, NULL);
				WCHAR buffer[sizeof(value)];

				if (readByte != 0) {
					wsprintf(buffer, L"%d", value);
					oldBin += wstring(buffer) + L" ";
				}
			} while (readByte != 0);
			SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)(wstring(L"Bin file after modding: ") + oldBin).c_str());

			WCHAR procInfo[500];
			MEMORY_PRIORITY_INFORMATION priority1;
			MEMORY_PRIORITY_INFORMATION priority2;
			GetProcessInformation(pi[0].hProcess, ProcessMemoryPriority, &priority1, sizeof(priority1));
			GetProcessInformation(pi[1].hProcess, ProcessMemoryPriority, &priority2, sizeof(priority2));
			wsprintf(procInfo, L"Process 1 Info:\r\n\tProcessID: %d\r\n\thProcess: %d\r\n\thPriority: %d\r\n\r\nProcess 2 Info:\r\n\tProcessID: %d\r\n\thProcess: %d\r\n\thPriority: %d", pi[0].dwProcessId, pi[0].hProcess, priority1, pi[1].dwProcessId, pi[1].hProcess, priority2);
			SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)procInfo);

			CloseHandle(hTxtFile);
			CloseHandle(hBinFile);

			break;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		}
	}
	return (INT_PTR)FALSE;
}