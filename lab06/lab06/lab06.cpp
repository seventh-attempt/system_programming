// lab06.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab06.h"
#include <string>
#include <sstream>

#define MAX_LOADSTRING 100
#define EDIT_4_ID 1104
#define EDIT_8_ID 1108
#define EDIT_16_ID 1116

using namespace std;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

string getDump(WCHAR*);
string hex(BYTE);

BOOL LaunchTicTac();
BOOL LaunchNightSky();

HWND h4Static;
HWND h8Static;
HWND h16Static;
HWND h4Edit;
HWND h8Edit;
HWND h16Edit;

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
    LoadStringW(hInstance, IDC_LAB06, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB06));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB06));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB06);
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
   DWORD editStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_CLIPSIBLINGS | WS_BORDER | ES_AUTOVSCROLL | WS_VSCROLL;
   DWORD staticStyle = WS_CHILD | WS_VISIBLE;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   
   h4Static = CreateWindow(L"STATIC", L"4 bits", staticStyle, 170, 0, 40, 20, hWnd, NULL, hInst, NULL);
   h4Static = CreateWindow(L"STATIC", L"8 bits", staticStyle, 540, 0, 40, 20, hWnd, NULL, hInst, NULL);
   h4Static = CreateWindow(L"STATIC", L"16 bits", staticStyle, 910, 0, 45, 20, hWnd, NULL, hInst, NULL);

   h4Edit = CreateWindow(L"edit", NULL, editStyle, 10, 25, 360, 500, hWnd, (HMENU)EDIT_4_ID, hInst, NULL);
   h8Edit = CreateWindow(L"edit", NULL, editStyle, 380, 25, 360, 500, hWnd, (HMENU)EDIT_8_ID, hInst, NULL);
   h16Edit = CreateWindow(L"edit", NULL, editStyle, 750, 25, 360, 500, hWnd, (HMENU)EDIT_16_ID, hInst, NULL);

   //CreateWindowEx(0, L"SCROLLBAR", (PTSTR)NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, 330, 0, 20, 500, h8Edit, NULL, hInst, NULL);

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
			case ID_IMAGES_DUMP: {
				SetDlgItemTextA(hWnd, EDIT_4_ID, getDump((WCHAR*)L"D:\\study\\system_programming\\lab06\\lab06\\images\\bmp_16_4.bmp").c_str());
				SetDlgItemTextA(hWnd, EDIT_8_ID, getDump((WCHAR*)L"D:\\study\\system_programming\\lab06\\lab06\\images\\bmp_32_8.bmp").c_str());
				SetDlgItemTextA(hWnd, EDIT_16_ID, getDump((WCHAR*)L"D:\\study\\system_programming\\lab06\\lab06\\images\\bmp_12_16.bmp").c_str());

				break;
			}
			case ID_GRAPHICS_TICTACTOE: {
				if (!LaunchTicTac()) {
					MessageBox(NULL, L"Something went wrong with launching 'TicTacToe'", L"Error", MB_OK);
				}
				
				break;
			}
			case ID_GRAPHICS_NIGHTSKY: {
				if (!LaunchNightSky()) {
					MessageBox(NULL, L"Something went wrong with launching 'Night Sky'", L"Error", MB_OK);
				}

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
        }
        break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
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

BOOL LaunchTicTac() {
	WCHAR exePath[] = L"D:\\study\\system_programming\\lab06\\Debug\\TicTacToe.exe";
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {
		sizeof(si)
	};

	if (!CreateProcess(exePath, NULL, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		return FALSE;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	return TRUE;
}
BOOL LaunchNightSky() {
	WCHAR exePath[] = L"D:\\study\\system_programming\\lab06\\Debug\\NightSky.exe";
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {
		sizeof(si)
	};

	if (!CreateProcess(exePath, NULL, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) {
		return FALSE;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	return TRUE;
}

string getDump(WCHAR* path) {

	stringstream stream;
	tagBITMAPFILEHEADER bmpFileHeader = { 0 };
	BITMAPINFOHEADER bmpInfoHeader = { 0 };
	tagRGBQUAD rgbQuad = { 0 };

	HANDLE BmpFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	SetFilePointer(BmpFile, NULL, NULL, FILE_BEGIN);
	ReadFile(BmpFile, &bmpFileHeader, sizeof(tagBITMAPFILEHEADER), NULL, NULL);

	SetFilePointer(BmpFile, sizeof(tagBITMAPFILEHEADER), NULL, FILE_BEGIN);
	ReadFile(BmpFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), NULL, NULL);

	SetFilePointer(BmpFile, sizeof(tagBITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), NULL, FILE_BEGIN);
	ReadFile(BmpFile, &rgbQuad, sizeof(tagRGBQUAD), NULL, NULL);

	stream << "BITMAPFILEHEADER :\r\n"
		<< "  Type : " << bmpFileHeader.bfType << "\r\n"
		<< "  Size : " << bmpFileHeader.bfSize << "\r\n"
		<< "  Reserved1 : " << bmpFileHeader.bfReserved1 << "\r\n"
		<< "  Reserved2 : " << bmpFileHeader.bfReserved2 << "\r\n"
		<< "  OffBits : " << bmpFileHeader.bfOffBits << "\r\n";

	stream << "\r\nBITMAPINFOHEADER :\r\n"
		<< "  Size : " << bmpInfoHeader.biSize << "\r\n"
		<< "  Width : " << bmpInfoHeader.biWidth << "\r\n"
		<< "  Height : " << bmpInfoHeader.biHeight << "\r\n"
		<< "  Planes : " << bmpInfoHeader.biPlanes << "\r\n"
		<< "  BitCount : " << bmpInfoHeader.biBitCount << "\r\n"
		<< "  Compression : " << bmpInfoHeader.biCompression << "\r\n"
		<< "  SizeImage : " << bmpInfoHeader.biSizeImage << "\r\n"
		<< "  XPelsPerMeter : " << bmpInfoHeader.biXPelsPerMeter << "\r\n"
		<< "  YPelsPerMeter : " << bmpInfoHeader.biYPelsPerMeter << "\r\n"
		<< "  ClrUsed : " << bmpInfoHeader.biClrUsed << "\r\n"
		<< "  ClrImportant : " << bmpInfoHeader.biClrImportant << "\r\n";

	stream << "\r\nRGBQUAD :\r\n"
		<< "  Blue : " << (int)rgbQuad.rgbBlue << "\r\n"
		<< "  Green : " << (int)rgbQuad.rgbGreen << "\r\n"
		<< "  Red : " << (int)rgbQuad.rgbRed << "\r\n"
		<< "  Reserved : " << (int)rgbQuad.rgbReserved << "\r\n";
	
	BYTE value;
	DWORD ReadByte = 1;
	string result;
	int i = 0;
	SetFilePointer(BmpFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);

	stream << "\r\nBitDump :";

	do {
		ReadFile(BmpFile, &value, sizeof(value), &ReadByte, NULL);

		if (i % 16 == 0) {
			stream << "\r\n  ";
		}
		if (ReadByte != 0) {
			stream << hex(value).c_str() << " ";
		}

		i++;
	} while (ReadByte != 0);

	return stream.str();
}
string hex(BYTE value) {
	string result;
	stringstream stream;

	if (value != '\t' && value != '\0') {
		stream << uppercase << hex << static_cast<unsigned int> (static_cast<unsigned char> (value));
		result = stream.str();

		if (result.size() < 2) {
			result = "0" + result;
		}
	}
	else {
		result = "00";
	}

	return result;
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
