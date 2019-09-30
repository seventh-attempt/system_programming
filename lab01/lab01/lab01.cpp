// lab01.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "lab01.h"
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

INT_PTR CALLBACK MainTask(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Task1(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Task2(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Task4(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Task5(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Task6(HWND, UINT, WPARAM, LPARAM);

CRITICAL_SECTION critSect;

const int thread_count = 3;
long var1 = 0;
long* var2 = new long[5];
long* var4 = new long[10];
long* var6 = new long[10];
long* var_main = new long[100];
int a = 0;
int b = 0;

UINT_PTR timer_id = 1;

HANDLE hMutex;
HANDLE hEvent;
HANDLE hSemaphore;

HWND hEdit;
HWND hEditResult;
HWND hEditChange;

HANDLE hThread[thread_count];
DWORD dwThreadId[thread_count];
DWORD dwThreadParam = 0;

DWORD WINAPI main_task_thread1(LPVOID);
DWORD WINAPI main_task_thread2(LPVOID);
DWORD WINAPI main_task_thread3(LPVOID);
DWORD WINAPI task1_func1(LPVOID);
DWORD WINAPI task1_func2(LPVOID);
DWORD WINAPI task1_func3(LPVOID);
DWORD WINAPI task2_func1(LPVOID);
DWORD WINAPI task2_func2(LPVOID);
DWORD WINAPI task4(LPVOID);
DWORD WINAPI task5_func1(LPVOID);
DWORD WINAPI task5_func2(LPVOID);
DWORD WINAPI task6(LPVOID);

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
	LoadStringW(hInstance, IDC_LAB01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB01));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB01));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB01);
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
	case WM_CREATE: {
		RECT rect;
		GetClientRect(hWnd, &rect);

		hEditResult = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_CLIPSIBLINGS | WS_BORDER,
			rect.left, rect.top, rect.right, rect.bottom, hWnd, nullptr, hInst, nullptr);

		// SetTimer(hWnd, timer_id, 1000, NULL);
		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_THREADS_MAINTASK: {
			SetTimer(hWnd, timer_id, 1000, NULL);
			// DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, MainTask);
			break;
		}
		case ID_THREADS_TASK1: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, Task1);
			break;
		}
		case ID_THREADS_TASK2: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, Task2);
			break;
		}
		case ID_THREADS_TASK4: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, Task4);
			break;
		}
		case ID_THREADS_TASK5: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, Task5);
			break;
		}
		case ID_THREADS_TASK6: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, Task6);
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
	case WM_TIMER: {
		hThread[0] = CreateThread(0, 0, main_task_thread1, &dwThreadParam, 0, &dwThreadId[0]);
		WaitForSingleObject(hThread[0], INFINITE);
		hThread[1] = CreateThread(0, 0, main_task_thread2, &dwThreadParam, 0, &dwThreadId[1]);
		WaitForSingleObject(hThread[1], INFINITE);
		hThread[2] = CreateThread(0, 0, main_task_thread3, &dwThreadParam, 0, &dwThreadId[2]);
		WaitForSingleObject(hThread[2], INFINITE);

		// DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, MainTask);

		for (int i = 0; i < 3; i++) {
			CloseHandle(hThread[i]);
		}

		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY: {
		// KillTimer(hWnd, timer_id);
		PostQuitMessage(0);
		break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
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

INT_PTR CALLBACK MainTask(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {

			wstring msg = L"[ ";

			for (int i = 0; i < a; i++) {
				msg += to_wstring(var_main[i]) + L" ";
			}

			msg += L"]";

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SetWindowText(hEditResult, msg.c_str());
			// SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());

			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}
}
INT_PTR CALLBACK Task1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {
			CreateThread(0, 0, task1_func1, &dwThreadParam, 0, &dwThreadId[0]);
			CreateThread(0, 0, task1_func2, &dwThreadParam, 0, &dwThreadId[1]);
			CreateThread(0, 0, task1_func3, &dwThreadParam, 0, &dwThreadId[2]);

			WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

			wstring msg = L"var1 after operation: " + to_wstring(var1);

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);
			CloseHandle(hThread[2]);
			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}

	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Task2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {
			InitializeCriticalSection(&critSect);
			hThread[0] = CreateThread(0, 0, task2_func1, &dwThreadParam, 0, &dwThreadId[0]);
			hThread[1] = CreateThread(0, 0, task2_func2, &dwThreadParam, 0, &dwThreadId[1]);
			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
			LeaveCriticalSection(&critSect);

			wstring msg = L"[ ";

			for (int i = 0; i < 5; i++) {
				msg += to_wstring(var2[i]) + L" ";
			}

			msg += L"]";

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);
			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}

	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Task4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {
			hMutex = CreateMutex(NULL, FALSE, L"Individual");

			if (hMutex == NULL)
			{
				MessageBox(hDlg, (LPCWSTR)GetLastError(), L"Error", MB_OK);
				return 1;
			}

			for (int i = 0; i < 2; i++) {
				hThread[i] = CreateThread(0, 0, task4, &dwThreadParam, 0, &dwThreadId[i]);
				WaitForSingleObject(hThread[i], INFINITE);
			}

			WaitForMultipleObjects(1, hThread, TRUE, INFINITE);

			wstring msg = L"[ ";

			for (int i = 0; i < 10; i++) {
				msg += to_wstring(var4[i]) + L" ";
			}

			msg += L"]";

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());


			for (int i = 0; i < 2; i++) {
				CloseHandle(hThread[i]);
			}

			CloseHandle(hMutex);

			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}

	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Task5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {
			hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			hThread[0] = CreateThread(0, 0, task5_func1, &dwThreadParam, 0, &dwThreadId[0]);
			hThread[1] = CreateThread(0, 0, task5_func2, &dwThreadParam, 0, &dwThreadId[1]);

			SetEvent(hEvent);

			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

			wstring msg = L"[ ";

			for (int i = 0; i < 5; i++) {
				msg += to_wstring(var2[i]) + L" ";
			}

			msg += L"]";

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);
			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}

	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Task6(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		switch (wmId)
		{
		case IDC_LAUNCH_TASK: {
			hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

			if (hSemaphore == NULL) {
				SendMessage(hDlg, (LPARAM)"Semaphore haven't been created", (LPARAM)"Error", MB_OK);
				return 1;
			}

			hThread[0] = CreateThread(0, 0, task6, &dwThreadParam, 0, &dwThreadId[0]);
			hThread[1] = CreateThread(0, 0, task6, &dwThreadParam, 0, &dwThreadId[1]);

			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

			wstring msg = L"[ ";

			int k = 0;

			if (a == 0) k = 10;
			else k = a;

			for (int i = 0; i < k; i++) {
				msg += to_wstring(var6[i]) + L" ";
			}

			msg += L"]";

			hEdit = GetDlgItem(hDlg, IDC_EDIT);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)msg.c_str());

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);
			CloseHandle(hSemaphore);
			break;
		}
		case IDCANCEL: {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	}

	return (INT_PTR)FALSE;
}

int compare(const void* first, const void* second) {
	return (*(int*)first - *(int*)second);
}

DWORD WINAPI main_task_thread1(LPVOID lParam) {
	if (a == 100) a = 0;

	var_main[a] = a;
	a++;
	return 0;
}
DWORD WINAPI main_task_thread2(LPVOID lParam) {
	// qsort(var_main, a, a * 8, compare);
	return 0;
}
DWORD WINAPI main_task_thread3(LPVOID lParam) {
	string msg = "[ ";

	for (int i = 0; i < a; i++) {
		msg += to_string(var_main[i]) + " ";
	}

	msg += "]";

	return 0;
}

DWORD WINAPI task1_func1(LPVOID lParam) {
	InterlockedExchangeAdd(&var1, 1);
	return 0;
}
DWORD WINAPI task1_func2(LPVOID lParam) {
	InterlockedExchangeAdd(&var1, 1);
	return 0;
}
DWORD WINAPI task1_func3(LPVOID lParam) {
	InterlockedExchangeAdd(&var1, -1);
	return 0;
}

DWORD WINAPI task2_func1(LPVOID lParam) {

	EnterCriticalSection(&critSect);
	for (int i = 0; i < 5; i++) {
		Sleep(10);
		var2[i] = i * 2 + 1;
	}
	LeaveCriticalSection(&critSect);

	return 0;
}
DWORD WINAPI task2_func2(LPVOID lParam) {

	EnterCriticalSection(&critSect);
	for (int i = 0; i < 5; i++) {
		Sleep(10);
		var2[i] = i * 2;
	}
	LeaveCriticalSection(&critSect);
	return 0;
}

DWORD WINAPI task4(LPVOID lParam) {
	int buffer[] = { 1, 2, 3, 4, 5 };

	WaitForSingleObject(OpenMutex(NULL, FALSE, L"Individual"), INFINITE);
	if (a == 10) {
		a = 0;
	}

	for (int i = 0; i < 5; i++) {
		var4[a] = buffer[i];
		a++;
		Sleep(10);
	}
	ReleaseMutex(hMutex);

	return 0;
}

DWORD WINAPI task5_func1(LPVOID lParam) {
	WaitForSingleObject(hEvent, INFINITE);

	for (int i = 0; i < 5; i++) {
		var2[i] = i * 2 + 1;
	}

	return 0;
}
DWORD WINAPI task5_func2(LPVOID lParam) {
	WaitForSingleObject(hEvent, INFINITE);

	for (int i = 0; i < 5; i++) {
		var2[i] = i * 2;
	}

	return 0;
}

DWORD WINAPI task6(LPVOID lParam) {
	WaitForSingleObject(hSemaphore, INFINITE);

	if (a >= 10) {
		a = 0;
	}

	var6[a] = (a+1) * (a+1);
	Sleep(100);
	a++;

	ReleaseSemaphore(hSemaphore, 1, NULL);
	
	return 0;
}
