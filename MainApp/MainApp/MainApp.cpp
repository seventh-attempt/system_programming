// MainApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MainApp.h"
#include "aclapi.h"
#include <commdlg.h>

#include "..\\DllApp\\DllApp.h"
#pragma comment(lib, "..\\x64\\Debug\\DllApp.lib")

using namespace std;

#define MAX_LOADSTRING 100
#define ID_RUN_BTN 1101
#define ID_ABOUT_BTN 1102

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Help(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Attributor(HWND, UINT, WPARAM, LPARAM);

HWND hWnd;
HBITMAP hImage;

void Cleanup(PSID pSIDEveryone, PSECURITY_DESCRIPTOR pSD, PACL pNewDACL);

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
    LoadStringW(hInstance, IDC_MAINAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAINAPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAINAPP);
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
   DWORD btnStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
   DWORD txtStyle = WS_CHILD | WS_VISIBLE;

   int wndWidth = 400;
   int wndHeight = 500;
   int btnWidth = 100;
   int btnHeight = 30;

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      500, 100, wndWidth, wndHeight, nullptr, nullptr, hInstance, nullptr);


   HWND bgsStatic = CreateWindow(L"STATIC", NULL, txtStyle,
	   0, 0, wndWidth, wndHeight, hWnd, NULL, hInstance, NULL);

   /*HANDLE hImage = LoadImage(NULL, L"D:\\study\\system_programming\\MainApp\\files\\lock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   SendMessage(hWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);*/

   hImage = (HBITMAP)LoadImage(NULL, L"D:\\study\\system_programming\\MainApp\\files\\lock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);

   HWND madeByStatic = CreateWindow(L"STATIC", L"Made by", txtStyle,
	   220, 280, 100, 20, hWnd, NULL, hInstance, NULL);
   HWND nameStatic = CreateWindow(L"STATIC", L"Khramkov Dmitry", txtStyle,
	   250, 300, 150, 20, hWnd, NULL, hInstance, NULL);
   HWND groupStatic = CreateWindow(L"STATIC", L"gr. 10702217", txtStyle,
	   250, 320, 150, 20, hWnd, NULL, hInstance, NULL);

   HWND startBtn = CreateWindow(L"button", L"Run", btnStyle, wndWidth-btnWidth-40, wndHeight-btnHeight-80, btnWidth, btnHeight,
	   hWnd, (HMENU)ID_RUN_BTN, hInstance, NULL);
   HWND aboutBtn = CreateWindow(L"Button", L"About", btnStyle, 25, wndHeight-btnHeight-80, btnWidth, btnHeight,
	   hWnd, (HMENU)ID_ABOUT_BTN, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
// FUNCTION: Cleanup(PSID, PSECURITY_DESCRIPTOR, PACL)
//
// PURPOSE: Free all dynamic resources
//
//
void Cleanup(PSID pSIDEveryone, PSECURITY_DESCRIPTOR pSD, PACL pNewDACL) {
	if (pSIDEveryone) {
		FreeSid(pSIDEveryone);
	}
	if (pSD) {
		LocalFree((HLOCAL)pSD);
	}
	if (pNewDACL) {
		LocalFree((HLOCAL)pNewDACL);
	}
	//if (filename) {
	//	delete[] filename;
	//}
	//if (file_access_choice) {
	//	delete[] file_access_choice;
	//}
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
	// Here we checking if the application is runned at specific machine by its GUID
	case WM_CREATE: {
		char *machineGUID = GetMachineGuid();

		if (strcmp(machineGUID, "7fef1237-4ade-41c0-bf14-cf5b1d4bfad8") != 0) {
			MessageBox(hWnd, L"This software doesn't belong to you", L"Error", MB_OK);
			return -1;
		}

		break;
	}
	case WM_COMMAND: {

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_RUN_BTN: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN_MENU), hWnd, Attributor);
			break;
		}
		case ID_MORE_HELP: {
			DialogBox(hInst, MAKEINTRESOURCE(ID_HELPBOX), hWnd, Help);
			break;
		}
		case ID_ABOUT_BTN: {
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
		HDC hdcMem = CreateCompatibleDC(hdc); // hDC is a DC structure supplied by Win32API

		SelectObject(hdcMem, hImage);
		StretchBlt(
			hdc,         // destination DC
			0,        // x upper left
			0,         // y upper left
			400,       // destination width
			250,      // destination height
			hdcMem,      // you just created this above
			0,
			0,          // x and y upper left
			400,          // source bitmap width
			250,          // source bitmap height
			SRCCOPY);   // raster operation

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

//
//  FUNCTION: Attributor(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: allows us to play around with file access attributes.
//
//	WM_INITDIALOG	- adds values in ComboBox and sets default file
//  WM_COMMAND		- process the messages
//  WM_DESTROY		- post a quit message and return
//
//
INT_PTR CALLBACK Attributor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	HWND fNameEdit = GetDlgItem(hDlg, ID_FNAME_EDIT);
	HWND accessCB = GetDlgItem(hDlg, ID_ACCESS_COMBO_BOX);

	const DWORD MaxLength = 0x7fff;
	static TCHAR name[256] = L"";
	static OPENFILENAME file;
	static HANDLE hFile;
	static char text[MaxLength];
	static DWORD nCharRead;

	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = hInst;
	file.lpstrFilter = L"Binary files (.exe)\0*.exe";
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = L"..\\files";
	file.lpstrDefExt = L"exe";

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		// Adding values to ComboBox
		SendMessage(accessCB, CB_ADDSTRING, NULL, (LPARAM)L"Set Access");
		SendMessage(accessCB, CB_ADDSTRING, NULL, (LPARAM)L"Grant Access");
		SendMessage(accessCB, CB_ADDSTRING, NULL, (LPARAM)L"Deny Access");
		SendMessage(accessCB, CB_ADDSTRING, NULL, (LPARAM)L"Revoke Access");

		//SendMessage(accessCB, CB_SETITEMHEIGHT, 1, 15);
		//SendMessage(accessCB, CB_SELECTSTRING, 0, NULL);

		SetWindowText(fNameEdit, L"D:\\study\\system_programming\\MainApp\\files\\4gb_patch.exe");
		break;
	}
	case WM_COMMAND: {

		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case ID_MAIN_RUN_BTN: {
			int filename_length = GetWindowTextLength(fNameEdit);
			char *filename = new char[filename_length];

			GetWindowTextA(fNameEdit, filename, filename_length+1);

			if (strcmp(filename, "") == 0) {
				//MessageBox(hWnd, L"You have to choose the file first", L"Error", MB_OK);
				return -1;
			}

			// Defines the access mode
			const int access_mode_length = GetWindowTextLength(accessCB);
			char *access_mode_choice = new char[access_mode_length];

			GetWindowTextA(accessCB, access_mode_choice, access_mode_length+1);
			ACCESS_MODE access_mode;

			if (strcmp(access_mode_choice, "Set Access") == 0) {
				access_mode = SET_ACCESS;
			}
			else if (strcmp(access_mode_choice, "Grant Access") == 0) {
				access_mode = GRANT_ACCESS;
			}
			else if (strcmp(access_mode_choice, "Deny Access") == 0) {
				access_mode = DENY_ACCESS;
			}
			else if (strcmp(access_mode_choice, "Revoke Access") == 0) {
				access_mode = REVOKE_ACCESS;
			}
			else {
				//MessageBox(hDlg, L"You have to choose access permission", L"Error", MB_OK);
				return -1;
			}

			// Defines the file permission
			DWORD file_permission = 0;

			if (SendDlgItemMessage(hDlg, ID_FILE_PERMISSION_READ, BM_GETCHECK, NULL, NULL)) {
				file_permission |= GENERIC_READ;
			}
			if (SendDlgItemMessage(hDlg, ID_FILE_PERMISSION_WRITE, BM_GETCHECK, NULL, NULL)) {
				file_permission |= GENERIC_WRITE;
			}
			if (SendDlgItemMessage(hDlg, ID_FILE_PERMISSION_EXECUTE, BM_GETCHECK, NULL, NULL)) {
				file_permission |= GENERIC_EXECUTE;
			}
			if (SendDlgItemMessage(hDlg, ID_FILE_PERMISSION_ALL, BM_GETCHECK, NULL, NULL)) {
				file_permission = GENERIC_ALL;
			}
			if (file_permission == 0) {
				// MessageBox(hDlg, L"You have to choose at least one permission", L"Error", MB_OK);
				return -1;
			}


			DWORD dwRes = 0;
			EXPLICIT_ACCESS ea;

			// Existing and new DACL pointers...
			PACL pDACL = NULL;

			// Reserving memory for security ID's
			PSID pSIDEveryone = NULL;

			// Security descriptor
			PSECURITY_DESCRIPTOR pSD = NULL;
			SecureZeroMemory(&pSD, sizeof(PSECURITY_DESCRIPTOR));

			SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
			SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

			// Specify the DACL to use
			// Create a SID for the Everyone group
			if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID,
				0, 0, 0, 0, 0, 0, 0, &pSIDEveryone)) {
				// MessageBox(hDlg, (LPCWSTR)GetLastError(), L"AllocateAndInitializeSid (Everyone) error", MB_OK);
				Cleanup(pSIDEveryone, pSD, pDACL);
				return -1;
			}

			ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));

			//---------------------------------------------
			// block access by certain user
			//---------------------------------------------
			ea.grfAccessPermissions = file_permission;
			ea.grfAccessMode = access_mode;
			ea.grfInheritance = NO_INHERITANCE;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName = (LPTSTR)pSIDEveryone;

			// Creates a new ACL that merges the new ACE into the existing DACL.
			dwRes = SetEntriesInAcl(1, &ea, NULL, &pDACL);

			//  Verify
			if (dwRes != ERROR_SUCCESS) {
				// MessageBox(hDlg, (LPCWSTR)dwRes, L"SetEntriesInAcl failed", MB_OK);
				Cleanup(pSIDEveryone, pSD, pDACL);
				return -1;
			}
			
			wchar_t *filename_buf = new wchar_t[filename_length];
			mbstowcs(filename_buf, filename, filename_length+1); // converts char* into wchar_t*

			// Attach the new ACL as the object's DACL.
			dwRes = SetNamedSecurityInfo((LPWSTR)filename_buf, SE_FILE_OBJECT,
				DACL_SECURITY_INFORMATION, NULL, NULL, pDACL, NULL);

			if (dwRes == ERROR_ACCESS_DENIED) {
				// MessageBox(hDlg, L"Chosen file is absent", L"Error", MB_OK);
				Cleanup(pSIDEveryone, pSD, pDACL);
				return -1;
			}

			if (dwRes == ERROR_SUCCESS) {
				// MessageBox(hDlg, L"Successfully changed DACL", L"Info", MB_OK);
				Cleanup(pSIDEveryone, pSD, pDACL);
				return 0;
			}

			break;
		}
		case ID_F_SELECT_BTN: {
			file.lpstrTitle = L"Choose file to open";

			if (!GetOpenFileName(&file)) {
				// MessageBox(hDlg, L"Something went wrong", L"Error", MB_OK | MB_ICONERROR);
				return -1;
			}

			SetWindowText(fNameEdit, file.lpstrFile);

			break;
		}
		}
		break;
	}
	case WM_DESTROY: {
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	default: {
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	}
	return 0;
}

//
//  FUNCTION: About(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: displays About dialog
//
//  WM_COMMAND		- process the messages
//
//
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
    }
    return (INT_PTR)FALSE;
}

//
//  FUNCTION: Help(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: displays Help dialog with the information about how to use the application
//
//  WM_COMMAND		- process the messages
//  WM_DESTROY		- post a quit message and return
//
//
INT_PTR CALLBACK Help(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON1: {
			MessageBox(hDlg, L"Wow", L"It works", MB_OK);
			break;
		}
		case IDM_EXIT: {
			DestroyWindow(hDlg);
			break;
		}
		default: {
			return DefWindowProc(hDlg, message, wParam, lParam);
		}
		}
		break;
	}
	case WM_DESTROY: {
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	default: {
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	}
	return 0;
}
