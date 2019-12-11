// lab07_receiver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab07_receiver.h"
#include <string>

#define MAX_LOADSTRING 100

#define STRUCT_TEXT_ID 1101
#define STRUCT_STRUCT_ID 1102

#define EDIT_TEXT 1201
#define EDIT_STRUCT 1202
#define EDIT_BUFFER_TEXT 1203
#define EDIT_BUFFER_IMAGE 1204


HWND hEditStructText;
HWND hEditStructStruct;
HWND hEditPasteText;
HWND hEditPasteImage;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace std;

struct userStruct {
	int code1;
	int code2;
} info;

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
    LoadStringW(hInstance, IDC_LAB07RECEIVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB07RECEIVER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB07RECEIVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB07RECEIVER);
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
   DWORD editStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_CLIPSIBLINGS | WS_BORDER;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 316, 359, nullptr, nullptr, hInstance, nullptr);

   hEditStructText = CreateWindow(L"edit", NULL, editStyle,
	   0, 0, 150, 150, hWnd, (HMENU)EDIT_TEXT, hInstance, nullptr);
   hEditStructStruct = CreateWindow(L"edit", NULL, editStyle,
	   0, 150, 150, 150, hWnd, (HMENU)EDIT_STRUCT, hInstance, nullptr);
   hEditPasteText = CreateWindow(L"edit", NULL, editStyle,
	   150, 0, 150, 150, hWnd, (HMENU)EDIT_BUFFER_TEXT, hInstance, nullptr);
   hEditPasteImage = CreateWindow(L"edit", NULL, editStyle,
	   150, 150, 150, 150, hWnd, (HMENU)EDIT_BUFFER_IMAGE, hInstance, nullptr);

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
	case WM_COPYDATA: {
		PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
		char *text;

		switch (pcds->dwData)
		{
		case STRUCT_TEXT_ID: {
			SetDlgItemTextA(hWnd, EDIT_TEXT, (char*)pcds->lpData);
			break;
		}
		case STRUCT_STRUCT_ID: {
			userStruct *info = (userStruct*)pcds->lpData;
			int code1 = info->code1;
			int code2 = info->code2;
			string msg = to_string(code1) + "\n" + to_string(code2);
			SetDlgItemTextA(hWnd, EDIT_STRUCT, msg.c_str());
			break;
		}
		}

		break;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_EDIT_PASTE_TEXT: {
			if (OpenClipboard(hWnd)) {
				HANDLE hData = GetClipboardData(CF_TEXT);
				SetDlgItemTextA(hWnd, EDIT_BUFFER_TEXT, (char*)hData);
				CloseClipboard();
			}

			break;
		}
		case ID_EDIT_PASTE_IMAGE: {
			if (OpenClipboard(hWnd)) {
				HDC hDc = GetDC(hWnd);
				HDC hDcMem = CreateCompatibleDC(hDc);

				if (hDcMem) {
					HBITMAP hBmp = (HBITMAP)GetClipboardData(CF_BITMAP);
					SelectObject(hDcMem, hBmp);
					
					RECT *rect = new RECT();
					GetWindowRect(hEditPasteImage, rect);

					POINT pointTL;
					POINT pointBR;

					pointTL.x = rect->left;
					pointTL.y = rect->top;
					pointBR.x = rect->right;
					pointBR.y = rect->bottom;

					ScreenToClient(hWnd, &pointTL);
					ScreenToClient(hWnd, &pointBR);

					rect->left = pointTL.x;
					rect->top = pointTL.y;
					rect->right = pointBR.x;
					rect->bottom = pointBR.y;

					RECT r = *rect;

					BITMAP bm;
					GetObject(hBmp, sizeof(bm), (LPSTR)&bm);

					StretchBlt(hDc, r.left, r.top, r.right/1.7, r.bottom/2, hDcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
					DeleteDC(hDcMem);
				}
			}
			break;
		}
		case ID_EDIT_PASTE_JORA: {
			UINT jora = RegisterClipboardFormatA("JORA");

			if (OpenClipboard(hWnd)) {
				if (IsClipboardFormatAvailable(jora)) {
					HANDLE hData = GetClipboardData(jora);
					string msg = to_string((DWORD)hData);
					SetDlgItemTextA(hWnd, EDIT_BUFFER_TEXT, msg.c_str());
					CloseClipboard();
				}
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
