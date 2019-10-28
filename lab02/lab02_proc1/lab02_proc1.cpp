// lab02_proc1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab02_proc1.h"
#include "string"

using namespace std;

#define MAX_LOADSTRING 100

#define WINDOW_W 400
#define WINDOW_H 300

HWND hEditOldTxtValue;
HWND hEditNewTxtValue;

HANDLE hFileTxt = NULL;
HANDLE hFileBmp = NULL;
HANDLE hMappingFileTxt = NULL;
HANDLE hMappingFileBmp = NULL;
HANDLE hProjectionFileTxt = NULL;
HANDLE hProjectionFileBmp = NULL;
HANDLE hAnotherMappingFileTxt = NULL;
HANDLE hAnotherMappingFileBmp = NULL;
HANDLE hAnotherProjectionFileTxt = NULL;
HANDLE hAnotherProjectionFileBmp = NULL;

BOOL InitializationAllFile();
BOOL OpenFiles(LPCWSTR txt, LPCWSTR bmp);

LPCWSTR this_txt_mapping_name = L"txt1_mapping_name";
LPCWSTR this_bmp_mapping_name = L"bmp1_mapping_name";
LPCWSTR other_txt_mapping_name = L"txt2_mapping_name";
LPCWSTR other_bmp_mapping_name = L"bmp2_mapping_name";

wchar_t this_proc_txt[] = L"D:\\study\\system_programming\\lab02\\lab02_proc1\\proc1_txt.txt";
wchar_t this_proc_bmp[] = L"D:\\study\\system_programming\\lab02\\lab02_proc1\\proc1_bmp.bmp";
wchar_t other_proc_txt[] = L"D:\\study\\system_programming\\lab02\\lab02_proc2\\proc2_txt.txt";
wchar_t other_proc_bmp[] = L"D:\\study\\system_programming\\lab02\\lab02_proc2\\proc2_bmp.bmp";

char* ReadTxt(HANDLE hnd);
void WorkWithBigFile(HWND hWnd);
BOOL Page();
void CloseAllHandle();

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB02PROC1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB02PROC1));

	MSG msg;

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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB02PROC1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB02PROC1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_W, WINDOW_H, nullptr, nullptr, hInstance, nullptr);

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
	switch (message)
	{
	case WM_CREATE: {
		if (!InitializationAllFile()) {
			DestroyWindow(hWnd);
		}

		while (!OpenFiles(other_txt_mapping_name, other_bmp_mapping_name)) {
			Sleep(100);
		}

		RECT rect;
		GetClientRect(hWnd, &rect);

		int height = (rect.bottom - rect.top) / 2;

		hEditOldTxtValue = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, 0, (rect.right - rect.left) / 2, height, hWnd, nullptr, hInst, nullptr);
		hEditNewTxtValue = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, height, (rect.right - rect.left) / 2, height, hWnd, nullptr, hInst, nullptr);

		SendMessageA(hEditOldTxtValue, EM_REPLACESEL, 0, (LPARAM)ReadTxt(hProjectionFileTxt));

		SetFilePointer(hFileTxt, GetFileSize(hFileTxt, NULL), NULL, NULL);
		WriteFile(hFileTxt, ReadTxt(hAnotherProjectionFileTxt), string(ReadTxt(hAnotherProjectionFileTxt)).length(), NULL, NULL);

		SendMessageA(hEditNewTxtValue, EM_REPLACESEL, 0, (LPARAM)ReadTxt(hProjectionFileTxt));
		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
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
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		GetClientRect(hWnd, &rect);
		int height = (rect.bottom - rect.top) / 2;

		BITMAPFILEHEADER *bFileHeader = (BITMAPFILEHEADER*)hProjectionFileBmp;
		BITMAPINFO *bInfo = (BITMAPINFO*)((char*)hProjectionFileBmp + 14);

		HBITMAP hBmpFile = CreateDIBitmap(hdc, &(bInfo->bmiHeader), CBM_INIT, (char*)hProjectionFileBmp + bFileHeader->bfOffBits, bInfo, DIB_PAL_COLORS);
		HDC hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, hBmpFile);
		StretchBlt(hdc, (rect.right - rect.left) / 2, rect.top, (rect.right - rect.left) / 2, height, hMemDC, 0, 0, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, SRCCOPY);

		bFileHeader = (BITMAPFILEHEADER*)hAnotherProjectionFileBmp;
		bInfo = (BITMAPINFO*)((char*)hAnotherProjectionFileBmp + 14);

		hBmpFile = CreateDIBitmap(hdc, &(bInfo->bmiHeader), CBM_INIT, (char*)hAnotherProjectionFileBmp + bFileHeader->bfOffBits, bInfo, DIB_PAL_COLORS);
		hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, hBmpFile);
		StretchBlt(hdc, (rect.right - rect.left) / 2, height, (rect.right - rect.left) / 2, height, hMemDC, 0, 0, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY: {
		CloseAllHandle();
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

BOOL InitializationAllFile() {

	BOOL START = FALSE;

	hFileTxt = CreateFile(this_proc_txt, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	hFileBmp = CreateFile(this_proc_bmp, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	string mesage1 = "Txt file proc 1\r\n";

	WriteFile(hFileTxt, mesage1.c_str(), mesage1.length(), 0, NULL);
	SetFilePointer(hFileTxt, NULL, NULL, NULL);

	if (hFileTxt == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Txt file wasn't created", L"Error", MB_OK | MB_ICONWARNING);
		return -1;
	}
	else if (hFileBmp == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"Bmp file wasn't created", L"Error", MB_OK | MB_ICONWARNING);
		return -1;
	}
	else {
		DWORD txt_size = GetFileSize(hFileTxt, NULL);

		hMappingFileTxt = CreateFileMapping(hFileTxt, NULL, PAGE_READWRITE, 0, txt_size, this_txt_mapping_name);
		hMappingFileBmp = CreateFileMapping(hFileBmp, NULL, PAGE_READONLY, 0, 0, this_bmp_mapping_name);

		if (hMappingFileTxt == NULL) {
			MessageBox(NULL, L"File mapping for txt file wasn't created", this_proc_txt, MB_OK | MB_ICONWARNING);
			return -1;
		}
		else if (hMappingFileBmp == NULL) {
			MessageBox(NULL, L"File mapping for bmp file wasn't created", this_proc_bmp, MB_OK | MB_ICONWARNING);
			return -1;
		}
		else {
			hProjectionFileTxt = MapViewOfFile(hMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
			hProjectionFileBmp = MapViewOfFile(hMappingFileBmp, FILE_MAP_READ, 0, 0, 0);

			START = TRUE;
		}
	}
	return START;
}
BOOL OpenFiles(LPCWSTR txt, LPCWSTR bmp) {

	BOOL START = FALSE;

	hAnotherMappingFileTxt = OpenFileMapping(FILE_MAP_READ, FALSE, txt);
	hAnotherMappingFileBmp = OpenFileMapping(FILE_MAP_READ, FALSE, bmp);

	if (hAnotherMappingFileTxt == NULL);
	else if (hAnotherMappingFileBmp == NULL);
	else {
		hAnotherProjectionFileTxt = MapViewOfFile(hAnotherMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
		hAnotherProjectionFileBmp = MapViewOfFile(hAnotherMappingFileBmp, FILE_MAP_READ, 0, 0, 0);

		if (hAnotherProjectionFileTxt == NULL);
		else if (hAnotherProjectionFileBmp == NULL);
		else START = TRUE;
	}

	return START;
}

char* ReadTxt(HANDLE hnd) {
	return (char*)hnd;
}

void WorkWithBigFile(HWND hWnd) {
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);

	HANDLE hBigFile = CreateFileA("", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	HANDLE hHugeFileMapping = CreateFileMapping(hBigFile, NULL, PAGE_READONLY, 0, 0, NULL);

	DWORD dwFileSizeHigh;
	__int64 dwFileSize = GetFileSize(hBigFile, &dwFileSizeHigh);
	dwFileSize += (((__int64)dwFileSizeHigh) << 32);
	CloseHandle(hBigFile);
	__int64 dwFileOffset = 0;
	DWORD dwNumOfOs = 0;
	char symbol[100] = "C:\\Users\\user\\AppData\\Local\\Discord\\Update.exe";
	DWORD dwPrBarRange = dwFileSize / sinf.dwAllocationGranularity;

	while (dwFileSize > 0)
	{
		DWORD dwBytesInBlock = sinf.dwAllocationGranularity;
		
		if (dwFileSize < sinf.dwAllocationGranularity) {
			dwBytesInBlock = (DWORD)dwFileSize;
		}

		PBYTE hHugeMapFileStartAddr = (PBYTE)MapViewOfFile(hHugeFileMapping, FILE_MAP_READ,
			(DWORD)(dwFileOffset >> 32), (DWORD)(dwFileOffset & 0xFFFFFFFF), dwBytesInBlock);

		UnmapViewOfFile(hHugeMapFileStartAddr);
		dwFileOffset += dwBytesInBlock;
		dwFileSize -= dwBytesInBlock;
	}
}
BOOL Page() {
	BOOL START = FALSE;

	hMappingFileTxt = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100, this_proc_txt);
	hMappingFileBmp = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READONLY, 0, 100, this_proc_bmp);

	if (hMappingFileTxt == NULL) {
		MessageBox(NULL, L"File mapping for txt file wasn't created", this_proc_txt, MB_OK | MB_ICONWARNING);
		return -1;
	}
	else if (hMappingFileBmp == NULL) {
		MessageBox(NULL, L"File mapping for bmp file wasn't created", this_proc_bmp, MB_OK | MB_ICONWARNING);
		return -1;
	}
	else {
		hProjectionFileTxt = MapViewOfFile(hMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
		hProjectionFileBmp = MapViewOfFile(hMappingFileBmp, FILE_MAP_READ, 0, 0, 0);

		START = TRUE;
	}

	return START;
}
void CloseAllHandle() {

	UnmapViewOfFile(hProjectionFileTxt);
	UnmapViewOfFile(hProjectionFileBmp);

	CloseHandle(hMappingFileTxt);
	CloseHandle(hMappingFileBmp);


	UnmapViewOfFile(hAnotherProjectionFileTxt);
	UnmapViewOfFile(hAnotherProjectionFileBmp);

	CloseHandle(hAnotherMappingFileTxt);
	CloseHandle(hAnotherMappingFileBmp);


	CloseHandle(hFileTxt);
	CloseHandle(hFileBmp);
}