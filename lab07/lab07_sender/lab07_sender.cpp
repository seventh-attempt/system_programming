// lab07_sender.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab07_sender.h"
#include <string>

#define MAX_LOADSTRING 100

#define STRUCT_TEXT_ID 1101
#define STRUCT_STRUCT_ID 1102

#define WINDOW_NAME L"lab07_receiver"

HWND hEditCopyImage;
HBITMAP hBmp;

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
    LoadStringW(hInstance, IDC_LAB07SENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB07SENDER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB07SENDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB07SENDER);
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
      CW_USEDEFAULT, 0, 300, 300, nullptr, nullptr, hInstance, nullptr);

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
	HWND wnd = FindWindow(NULL, WINDOW_NAME);

    switch (message)
    {
	case WM_CREATE: {
		if (!wnd) {
			MessageBox(hWnd, L"There is no receiving window named lab07_receiver", L"Error", MB_OK);
			break;
		}

		char text1[] = "I'm a process traveller";
		char text2[] = "I'm a structured process traveller";

		COPYDATASTRUCT cds1;
		cds1.dwData = STRUCT_TEXT_ID;
		cds1.cbData = sizeof(text1);
		cds1.lpData = &text1;

		SendMessage(wnd, WM_COPYDATA, (WPARAM)(HWND)hWnd, (LPARAM)(LPVOID)&cds1);

		info.code1 = 3223;
		info.code2 = 782;

		COPYDATASTRUCT cds2;
		cds2.dwData = STRUCT_STRUCT_ID;
		cds2.cbData = sizeof(info);
		cds2.lpData = &info;

		SendMessage(wnd, WM_COPYDATA, (WPARAM)(HWND)hWnd, (LPARAM)(LPVOID)&cds2);

		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_EDIT_COPY_TEXT: {
				if (!wnd) {
					MessageBox(hWnd, L"There is no receiving window named lab07_receiver", L"Error", MB_OK);
					break;
				}

				if (OpenClipboard(wnd)) {
					if (EmptyClipboard()) {
						char buffer[] = "buffer";
						HGLOBAL hGl = GlobalAlloc(GMEM_DDESHARE, sizeof(buffer));
						memcpy(hGl, buffer, sizeof(buffer));
						SetClipboardData(CF_TEXT, hGl);
						CloseClipboard();
					}
				}
				break;
			}
			case ID_EDIT_COPY_IMAGE: {
				if (OpenClipboard(wnd)) {
					if (EmptyClipboard()) {
						hBmp = (HBITMAP)LoadImageA(NULL, "nut256.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						SetClipboardData(CF_BITMAP, hBmp);
						CloseClipboard();
					}
				}

				break;
			}
			case ID_EDIT_COPY_JORA: {
				if (OpenClipboard(wnd)) {
					if (EmptyClipboard()) {
						UINT jora = RegisterClipboardFormatA("JORA");
+						DWORD grove = 25;
						HGLOBAL hGl = GlobalAlloc(GMEM_DDESHARE, sizeof(grove));
						memcpy(hGl, &grove, sizeof(grove));
						SetClipboardData(jora, hGl);
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
