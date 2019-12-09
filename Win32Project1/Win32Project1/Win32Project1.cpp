// Win32Project1.cpp: определяет точку входа для приложения.
//


#include "stdafx.h"
#include "Win32Project1.h"
#include "string"

#include "C:\\Users\\user\\Desktop\\5\FirstDll\\FirstDll\\FirstDll.h"
#include "C:\\Users\\user\\Desktop\\5\\SecondDll\\SecondDll\\SecondDll.h"

#pragma comment(lib, "C:\\Users\\user\\Desktop\\5\\FirstDll\\Debug\\FirstDll.lib")

#define MAX_LOADSTRING 100
#define TIMER_SYSINFO 1
#define TIMER_MEMINFO 2
#define Second 1000

HWND hDialogSysInf = NULL;
HWND hDialogMemInf = NULL;

void ActivateDialog(HINSTANCE hInst, HWND hWnd, HWND hDialog, int IDD_DIALOG, DLGPROC foo, int hTimer);
INT_PTR CALLBACK SysInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MemInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HANDLE hThread;
DWORD WINAPI ThreadFunc1(LPVOID);
DWORD WINAPI ThreadFunc2(LPVOID);

HMODULE secondDll;

using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

    MSG msg;

    // Цикл основного сообщения:
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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD dwThreadIdArray;

    switch (message)
    {
	case WM_CREATE: {
		secondDll = LoadLibrary(L"C:\\Users\\user\\Desktop\\5\\SecondDll\\Debug\\SecondDll.dll");

		if (!secondDll) {
			MessageBox(NULL, L"SecondDll not included", L"Error", MB_OK | MB_ICONWARNING);
		}

		HMODULE firstDll = LoadLibrary(L"C:\\Users\\user\\Desktop\\5\\FirstDll\\Debug\\FirstDll.dll");

		if (firstDll != NULL) {
			string result;

			string(*DllFunc)();
			DllFunc = ((string(*)())GetProcAddress(firstDll, (LPCSTR)MAKEINTRESOURCE(1)));

			if (DllFunc != NULL) {
				result = DllFunc();
			}
			else {
				MessageBox(NULL, L"DllFunc not found", L"Error", MB_OK | MB_ICONWARNING);
			}

			FreeLibrary(firstDll);
		}
		else {
			MessageBox(NULL, L"FIRST_DLL not included", L"Error", MB_OK | MB_ICONWARNING);
		}
		
		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_SYSINFO: {
			hThread = CreateThread(NULL, 0, ThreadFunc1, 0, NULL, &dwThreadIdArray);
			break;
		}
		case IDM_MEMINFO: {
			hThread = CreateThread(NULL, 0, ThreadFunc2, 0, NULL, &dwThreadIdArray);
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
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
	case WM_INITDIALOG: {
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SysInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		SendMessageA(GetDlgItem(hDlg, IDC_EDIT_SYSTEM_INFO), WM_SETTEXT, 0, (LPARAM)GetSystemInformation().c_str());
		break;
	}
	case WM_TIMER: {
		SendMessageA(GetDlgItem(hDlg, IDC_EDIT_SYSTEM_INFO), WM_SETTEXT, 0, (LPARAM)GetSystemInformation().c_str());
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDCANCEL)
		{
			KillTimer(hDlg, TIMER_SYSINFO);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK MemInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		SendMessageA(GetDlgItem(hDlg, IDC_EDIT_MEMORY_INFO), WM_SETTEXT, 0, (LPARAM)GetMemoryInformation().c_str());
		break;
	}
	case WM_TIMER: {
		SendMessageA(GetDlgItem(hDlg, IDC_EDIT_MEMORY_INFO), WM_SETTEXT, 0, (LPARAM)GetMemoryInformation().c_str());
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDCANCEL) {
			KillTimer(hDlg, TIMER_MEMINFO);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	}
	return (INT_PTR)FALSE;
}

void ActivateDialog(HINSTANCE hInst,HWND hWnd, HWND hDialog, int IDD_DIALOG, DLGPROC foo, int hTimer) {
	if (hDialog == NULL) {
		hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, foo);

		SetParent(hDialog, hWnd);

		SetTimer(hDialog, hTimer, Second / 2, NULL);
		ShowWindow(hDialog, SW_SHOW);
	}
	else {
		SetTimer(hDialog, hTimer, Second / 2, NULL);
		ShowWindow(hDialog, SW_SHOW);
	}
}

DWORD WINAPI ThreadFunc1(LPVOID lparam) {
	ActivateDialog(secondDll, NULL, hDialogSysInf, IDD_DIALOG_SYSINFO, SysInfo, TIMER_SYSINFO);

	return 0;
}
DWORD WINAPI ThreadFunc2(LPVOID lparam) {
	ActivateDialog(secondDll, NULL, hDialogMemInf, IDD_DIALOG_MEMINFO, MemInfo, TIMER_MEMINFO);

	return 0;
}