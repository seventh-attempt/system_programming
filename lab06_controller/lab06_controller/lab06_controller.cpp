// lab06_controller.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "lab06_controller.h"
#include <string>
#include "shellapi.h"
#include <TlHelp32.h>
#include <strsafe.h>

#define _CRT_NON_CONFORMING_SWPRINTFS

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

//static WCHAR *parentProcID;

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
    LoadStringW(hInstance, IDC_LAB06CONTROLLER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB06CONTROLLER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB06CONTROLLER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB06CONTROLLER);
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
		case ID_PROCESS_CREATE:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CREATEPROCESS), hWnd, CreateMainProcess);
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
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

INT_PTR CALLBACK CreateMainProcess(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	LPCWSTR txtFile = L"D:\\study\\System programming\\lab06\\files\\txtFile.txt";
	LPCWSTR binFile = L"D:\\study\\System programming\\lab06\\files\\binFile.bin";

	WCHAR exeTxtFile[] = L"D:\\study\\System programming\\lab06\\lab06_txtFiles\\Debug\\lab06_txtFiles.exe";
	WCHAR exeBinFile[] = L"D:\\study\\System programming\\lab06\\lab06_binFiles\\Debug\\lab06_binFiles.exe";
	WCHAR cmdTxtParam[sizeof(HANDLE)];
	WCHAR cmdBinParam[sizeof(HANDLE)];

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hTxtFile;
	HANDLE hBinFile;

	//????????????????????????????

	LPVOID lpvEnv = GetEnvironmentStrings();
	LPCTSTR lpszVariable = (LPCTSTR)lpvEnv;

	while (lpszVariable[0] != '\0') {
		int size = _tcslen(lpszVariable);
		lpszVariable += _tcslen(lpszVariable) + 1;
	}

	FreeEnvironmentStrings((LPTSTR)lpvEnv);

	DWORD procID = GetCurrentProcessId();
	WCHAR parentProcID[20] = L"";
	wsprintf(parentProcID, L"%d", procID);
	SetEnvironmentVariable(L"parentProcID", parentProcID);
	
	WCHAR fio[] = L"Khramkov Dmitry Sergeevich";
	SetEnvironmentVariable(L"FIO", fio);

	//????????????????????????????

	char text[] = "Some additional text to write ";
	int value = 15;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_CREATE:
		{
			if ((hTxtFile = CreateFile(txtFile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, &sa,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Couldn't create text file", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}
			if ((hBinFile = CreateFile(binFile, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, &sa,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Couldn't create binary file", L"Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			wsprintf(cmdTxtParam, L"%d", hTxtFile);
			wsprintf(cmdBinParam, L"%d", hBinFile);

			/*WriteFile(hTxtFile, text, sizeof(text), 0, NULL);*/
			WriteFile(hBinFile, &value, sizeof(value), 0, NULL);

			STARTUPINFO si = {
				sizeof(si)
			};
			PROCESS_INFORMATION pi[2];

			CreateProcess(exeTxtFile, cmdTxtParam, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi[0]);
			CreateProcess(exeBinFile, cmdBinParam, NULL, NULL, TRUE, ABOVE_NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi[0]);
			//CreateProcess(exeTxtFile, cmdFioParam, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, lpvEnv, NULL, &si, &pi[0]);

			//--------------------

			HWND hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
			HWND hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
			HWND hEdit3 = GetDlgItem(hDlg, IDC_EDIT3);

			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pEntry;

			pEntry.dwSize = sizeof(PROCESSENTRY32);

			wstring message = L"All processes information: \r\n\tProcessID:\tParentProcessID:\tProcessName:\r\n";

			if (Process32First(hSnap, &pEntry)) {
				do {
					message += L"\t[" + to_wstring(pEntry.th32ProcessID) + L"]\t\t[" + to_wstring(pEntry.th32ParentProcessID) + L"]\t\t" + pEntry.szExeFile + wstring(L"\r\n");
				} while (Process32Next(hSnap, &pEntry));
			}

			SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)message.c_str());

			CloseHandle(hSnap);

			//--------------------

			SetPriorityClass(pi[1].hProcess, BELOW_NORMAL_PRIORITY_CLASS);

			WaitForSingleObject(pi[0].hProcess, INFINITE);
			WaitForSingleObject(pi[1].hProcess, INFINITE);

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

			FILETIME creationTime;
			FILETIME exitTime;
			FILETIME kernelTime;
			FILETIME userTime;
			SYSTEMTIME systemTime;

			GetProcessTimes(pi[0].hProcess, &creationTime, &exitTime, &kernelTime, &userTime);
			wstring time = L"Process work time: \r\n";

			FileTimeToLocalFileTime(&creationTime, &creationTime);
			
			FileTimeToSystemTime(&creationTime, &systemTime);
			time += L"\tCreation time: " + to_wstring(systemTime.wHour) + L":" + to_wstring(systemTime.wMinute) + L":" + to_wstring(systemTime.wSecond) + L"\r\n";
			FileTimeToSystemTime(&userTime, &systemTime);
			time += L"\tUser time: " + to_wstring(systemTime.wHour) + L":" + to_wstring(systemTime.wMinute) + L":" + to_wstring(systemTime.wSecond) + L"\r\n";
			FileTimeToSystemTime(&kernelTime, &systemTime);
			time += L"\tKernel time: " + to_wstring(systemTime.wHour) + L":" + to_wstring(systemTime.wMinute) + L":" + to_wstring(systemTime.wSecond) + L"\r\n";

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
	}
	return (INT_PTR)FALSE;
}