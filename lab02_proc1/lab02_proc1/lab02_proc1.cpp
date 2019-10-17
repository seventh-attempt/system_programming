// lab02_proc1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "lab02_proc1.h"
#include "string"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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

char* ReadTxt(HANDLE hnd);
void WorkWithBigFile(HWND hWnd);
BOOL Page();


wstring msg = L"message";
wstring old_msg = L"old message";
wstring new_msg = L"new message";
LPCWSTR this_proc_txt = L"proc1_txt.txt";
LPCWSTR this_proc_bmp = L"proc1_bmp.bmp";
LPCWSTR other_proc_txt = L"proc2_txt.txt";
LPCWSTR other_proc_bmp = L"proc2_bmp.bmp";

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB02PROC1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB02PROC1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB02PROC1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB02PROC1);
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
	case WM_CREATE: {
		{
			hFileTxt = CreateFile(this_proc_txt, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			hFileBmp = CreateFile(this_proc_bmp, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			WriteFile(hFileTxt, msg.c_str(), msg.length(), 0, NULL);
			SetFilePointer(hFileTxt, NULL, NULL, NULL);

			hMappingFileTxt = CreateFileMapping(hFileTxt, NULL, PAGE_READWRITE, 0, GetFileSize(hFileTxt, NULL), this_proc_txt);
			hMappingFileBmp = CreateFileMapping(hFileBmp, NULL, PAGE_READONLY, 0, 0, this_proc_bmp);

			hProjectionFileTxt = MapViewOfFile(hMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
			hProjectionFileBmp = MapViewOfFile(hMappingFileBmp, FILE_MAP_READ, 0, 0, 0);
		}
		Sleep(500);	
		{
			hAnotherMappingFileTxt = OpenFileMapping(FILE_MAP_READ, FALSE, other_proc_txt);
			hAnotherMappingFileBmp = OpenFileMapping(FILE_MAP_READ, FALSE, other_proc_bmp);

			hAnotherProjectionFileTxt = MapViewOfFile(hAnotherMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
			hAnotherProjectionFileBmp = MapViewOfFile(hAnotherMappingFileBmp, FILE_MAP_READ, 0, 0, 0);
		}
		
		RECT rect;
		GetClientRect(hWnd, &rect);

		int height = (rect.bottom - rect.top) / 2;


		hEditOldTxtValue = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, 0, (rect.right - rect.left) / 2, height, hWnd, nullptr, hInst, nullptr);
		hEditNewTxtValue = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, height, (rect.right - rect.left) / 2, height, hWnd, nullptr, hInst, nullptr);


		SendMessage(hEditOldTxtValue, EM_REPLACESEL, 0, (LPARAM)old_msg.c_str());
		SendMessageA(hEditOldTxtValue, EM_REPLACESEL, 0, (LPARAM)ReadTxt(hProjectionFileTxt));

		SetFilePointer(hFileTxt, GetFileSize(hFileTxt, NULL), NULL, NULL);
		WriteFile(hFileTxt, ReadTxt(hAnotherProjectionFileTxt), string(ReadTxt(hAnotherProjectionFileTxt)).length(), NULL, NULL);

		SendMessage(hEditNewTxtValue, EM_REPLACESEL, 0, (LPARAM)new_msg.c_str());
		SendMessageA(hEditNewTxtValue, EM_REPLACESEL, 0, (LPARAM)ReadTxt(hProjectionFileTxt));

		{
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
		break;
	}
    case WM_COMMAND: {
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
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		GetClientRect(hWnd, &rect);
		int height = (rect.bottom - rect.top) / 2;

		BITMAPFILEHEADER* bFileHeader = (BITMAPFILEHEADER*)hProjectionFileBmp;
		BITMAPINFO* bInfo = (BITMAPINFO*)((char*)hProjectionFileBmp + 14);

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

char* ReadTxt(HANDLE hnd) {
	return (char*)hnd;
}

void WorkWithBigFile(HWND hWnd) {
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);

	HANDLE hBigFile = CreateFileA("B:\\тварение\\4\\Winapi\\2\\Крылов Д.А. гр.10702217.rar", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	HANDLE hHugeFileMapping = CreateFileMapping(hBigFile, NULL, PAGE_READONLY, 0, 0, NULL);

	DWORD dwFileSizeHigh;
	__int64 dwFileSize = GetFileSize(hBigFile, &dwFileSizeHigh);
	dwFileSize += (((__int64)dwFileSizeHigh) << 32);
	CloseHandle(hBigFile);
	__int64 dwFileOffset = 0;
	DWORD dwNumOfOs = 0;
	char symbol[100] = "";
	DWORD dwPrBarRange = dwFileSize / sinf.dwAllocationGranularity;
	while (dwFileSize > 0)
	{

		DWORD dwBytesInBlock = sinf.dwAllocationGranularity;
		if (dwFileSize < sinf.dwAllocationGranularity) dwBytesInBlock = (DWORD)dwFileSize;
		PBYTE hHugeMapFileStartAddr = (PBYTE)MapViewOfFile(hHugeFileMapping, FILE_MAP_READ,
			(DWORD)(dwFileOffset >> 32), // начальный байт
			(DWORD)(dwFileOffset & 0xFFFFFFFF), // в файле
			dwBytesInBlock); // число проецируемых байтов

							 //MessageBox(hWnd,(LPCWSTR)(char*)hHugeMapFileStartAddr, L"f", MB_OK);

							 //SetDlgItemText(hDlg, IDC_EDIT9, (char*)hHugeMapFileStartAddr);

		UnmapViewOfFile(hHugeMapFileStartAddr);
		dwFileOffset += dwBytesInBlock;
		dwFileSize -= dwBytesInBlock;
	}
}
BOOL Page() {
	BOOL START = FALSE;

	hMappingFileTxt = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100, this_proc_txt);
	hMappingFileBmp = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READONLY, 0, 100, this_proc_bmp);

	if (hMappingFileTxt == NULL) MessageBox(NULL, L"Невозможно создать проекцию файла TXT !", this_proc_txt, MB_OK | MB_ICONWARNING);
	else if (hMappingFileBmp == NULL) MessageBox(NULL, L"Невозможно создать проекцию файла BMP !", this_proc_bmp, MB_OK | MB_ICONWARNING);
	else {


		hProjectionFileTxt = MapViewOfFile(hMappingFileTxt, FILE_MAP_READ, 0, 0, 0);
		hProjectionFileBmp = MapViewOfFile(hMappingFileBmp, FILE_MAP_READ, 0, 0, 0);

		START = TRUE;
	}
	return START;
}
