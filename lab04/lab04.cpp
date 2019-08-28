// lab04.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include "lab04.h"
#include <commdlg.h>
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

INT_PTR CALLBACK	TxtFilesWork(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	BinFilesWork(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DumpsWork(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AdditionalTask(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB04, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB04));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB04));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB04);
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
			case ID_FILE_TXTFILESWORK:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_TXTDIALOG), hWnd, TxtFilesWork);
				break;
			}
			case ID_FILE_BINFILESWORK:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_BINDIALOG), hWnd, BinFilesWork);
				break;
			}
			case ID_FILE_DUMPSWORK:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DUMPDIALOG), hWnd, DumpsWork);
				break;
			}
			case ID_FILE_ADDITIONALTASK:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDITIONALTASK), hWnd, AdditionalTask);
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

INT_PTR CALLBACK TxtFilesWork(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	const DWORD MaxLength = 0x7fff;
	static TCHAR name[256] = L"";
	static OPENFILENAME file;
	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;

	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = hInst;
	file.lpstrFilter = L"Text files (.txt)\0*.txt";
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = L".\\files\\text";
	file.lpstrDefExt = L"txt";

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_CREATETXTFILE:
		{
			hFile = CreateFile(L"files\\text\\textFile.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
			}
			else {
				MessageBox(hDlg, L"File was successfully created", L"Wow", MB_OK);
			}
			CloseHandle(hFile);

			break;
		}
		case IDC_OPENTXTFILE:
		{

			file.lpstrTitle = L"Choose file to open";
			file.Flags = OFN_HIDEREADONLY;

			if (!GetOpenFileName(&file)) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			HWND edit = GetDlgItem(hDlg, IDC_TXTEDIT);
			hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			ReadFile(hFile, text, MaxLength, &nCharRead, NULL);
			SetDlgItemTextA(hDlg, IDC_TXTEDIT, text);

			CloseHandle(hFile);

			break;
		}
		case IDC_SAVETXTFILE:
		{
			if (!GetWindowTextLength(GetDlgItem(hDlg, IDC_TXTEDIT)) == 0) {

				if (!GetSaveFileName(&file)) {
					MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}

				HWND edit = GetDlgItem(hDlg, IDC_TXTEDIT);
				GetWindowTextA(edit, text, MaxLength);
				hFile = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				WriteFile(hFile, text, sizeof(text), 0, NULL);

				CloseHandle(hFile);

				MessageBox(hDlg, L"File info was written", L"Info", MB_OK);
			}
			else {
				MessageBox(hDlg, L"You have to input some text", L"Error", MB_OK);
			}

			break;
		}
		case IDC_GETFILEATTRIBUTES:
		{
			if (!GetOpenFileName(&file)) {
				GetFileAttributes(name);
			}
			break;
		}
		case IDC_SETFILEATTRIBUTES:
		{
			if (!GetOpenFileName(&file)) {
				SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
			}
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK BinFilesWork(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	const DWORD MaxLength = 0x7fff;
	static TCHAR name[256] = L"";
	static OPENFILENAME file;
	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;

	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = hInst;
	file.lpstrFilter = L"Binary files (.bin)\0*.bin";
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = L".\\files\\binary";
	file.lpstrDefExt = L"bin";

	HWND edit;


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_CREATEBINFILE:
		{
			hFile = CreateFile(L"files\\binary\\binFile.bin", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
			}
			else {
				MessageBox(hDlg, L"File was successfully created", L"Wow", MB_OK);
			}

			CloseHandle(hFile);

			break;
		}
		case IDC_OPENBINFILE:
		{
			file.lpstrTitle = L"Choose file to open";
			file.Flags = OFN_HIDEREADONLY;

			if (!GetOpenFileName(&file)) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			edit = GetDlgItem(hDlg, IDC_BINEDIT);
			hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			BYTE byte;
			UINT uint;
			INT32 int32;
			INT64 int64;
			WORD word;
			DWORD dword;
			BOOL boool;
			float fl;
			double db;
			char c;

			string result;

			edit = GetDlgItem(hDlg, IDC_BINEDIT);

			ReadFile(hFile, &byte, sizeof(byte), 0, NULL);
			ReadFile(hFile, &uint, sizeof(uint), 0, NULL);
			ReadFile(hFile, &int32, sizeof(int32), 0, NULL);
			ReadFile(hFile, &int64, sizeof(int64), 0, NULL);
			ReadFile(hFile, &word, sizeof(word), 0, NULL);
			ReadFile(hFile, &dword, sizeof(dword), 0, NULL);
			ReadFile(hFile, &boool, sizeof(boool), 0, NULL);
			ReadFile(hFile, &fl, sizeof(fl), 0, NULL);
			ReadFile(hFile, &db, sizeof(db), 0, NULL);
			ReadFile(hFile, &c, sizeof(c), 0, NULL);

			result = "Info from file: \r\n\tBYTE: " + to_string(byte) + "\r\n\tUINT: " + to_string(uint) + "\r\n\tINT32: " + to_string(int32)
				+ "\r\n\tINT64: " + to_string(int64) + "\r\n\tWORD: " + to_string(word) + "\r\n\tDWORD: " + to_string(dword) + "\r\n\tBOOL: " + to_string(boool)
				+ "\r\n\tFLOAT: " + to_string(fl) + "\r\n\tDOUBLE: " + to_string(db) + "\r\n\tCHAR: " + to_string(c);

			strcpy(text, result.c_str());

			SetDlgItemTextA(hDlg, IDC_BINEDIT, text);

			CloseHandle(hFile);

			break;
		}
		case IDC_SAVEBINFILE:
		{
			if (!GetSaveFileName(&file)) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}
			
			edit = GetDlgItem(hDlg, IDC_BINEDIT);
			GetWindowTextA(edit, text, MaxLength);
			hFile = CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			BYTE byte = 123;
			UINT uint = 10;
			INT32 int32 = 23;
			INT64 int64 = 134;
			WORD word = 3;
			DWORD dword = 43;
			BOOL boool = 0;
			float fl = 12.3f;
			double db = 34.34;
			char c = 's';

			WriteFile(hFile, &byte, sizeof(byte), 0, NULL);
			WriteFile(hFile, &uint, sizeof(uint), 0, NULL);
			WriteFile(hFile, &int32, sizeof(int32), 0, NULL);
			WriteFile(hFile, &int64, sizeof(int64), 0, NULL);
			WriteFile(hFile, &word, sizeof(word), 0, NULL);
			WriteFile(hFile, &dword, sizeof(dword), 0, NULL);
			WriteFile(hFile, &boool, sizeof(boool), 0, NULL);
			WriteFile(hFile, &fl, sizeof(fl), 0, NULL);
			WriteFile(hFile, &db, sizeof(db), 0, NULL);
			WriteFile(hFile, &c, sizeof(c), 0, NULL);

			CloseHandle(hFile);

			MessageBox(hDlg, L"File info was written", L"Info", MB_OK);

			break;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK DumpsWork(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	const DWORD MaxLength = 0x7fff;
	static TCHAR name[256] = L"";
	static OPENFILENAME file;
	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;

	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = hInst;
	file.lpstrFilter = L"Binary files (.bin)\0*.bin";
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = L".\\files\\binary";
	file.lpstrDefExt = L"bin";

	HWND edit1;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (wParam) {
		case IDC_OPENDUMPFILE: 
		{
			file.lpstrTitle = L"Choose file to open";
			file.Flags = OFN_HIDEREADONLY;

			if (!GetOpenFileName(&file)) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			edit1 = GetDlgItem(hDlg, IDC_DUMPEDIT1);

			int size = GetFileSize(hFile, NULL);
			ReadFile(hFile, text, size, NULL, NULL);

			CHAR *q = new CHAR[size];
			for (int i = 0, k = 0; i < size; i++) {
				k += sprintf_s(q + k, size, "%02x ", (unsigned char)text[i]);
			}

			SetDlgItemTextA(hDlg, IDC_DUMPEDIT1, q);

			CloseHandle(hFile);
			
			break;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK AdditionalTask(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	
	const DWORD MaxLength = 0x7fff;
	static TCHAR name[256] = L"";
	static OPENFILENAME file;
	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;

	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = hInst;
	file.lpstrFilter = L"Binary files (.bin)\0*.bin";
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = L".\\files\\binary";
	file.lpstrDefExt = L"bin";

	HWND edit1;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (wParam) {
		case IDC_ADDITIONALTASK:
		{
			file.lpstrTitle = L"Choose file to open";
			file.Flags = OFN_HIDEREADONLY;

			/*if (!GetOpenFileName(&file)) {
				MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}*/

			hFile = CreateFile(L"D:\\study\\System programming\\lab04\\lab04\\files\\binary\\binFile.bin", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			int size = GetFileSize(hFile, NULL);

			char buf[200];

			SetFilePointer(hFile, 0, 0, 0);
			ReadFile(hFile, &buf, size, 0, NULL);

			swap(buf[17], buf[18]);

			SetFilePointer(hFile, 0, 0, 0);
			WriteFile(hFile, &buf, size, 0, 0);

			CloseHandle(hFile);

			break;
		}
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
		break;
	}
	return (INT_PTR)FALSE;
}