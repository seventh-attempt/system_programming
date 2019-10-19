// course_work.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "course_work.h"
#include "windows.h"
#include "accctrl.h"
#include "aclapi.h"
#pragma comment(lib, "advapi32.lib")
#include "string"

using namespace std;

#define MAX_LOADSTRING 100
#define ID_DENY_BUTTON 1101
#define ID_TRUSTEE_BUTTON 1102

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

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
	LoadStringW(hInstance, IDC_COURSEWORK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEWORK));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEWORK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_COURSEWORK);
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

	DWORD button_style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	HWND deny_button = CreateWindow(L"button", L"Deny all", button_style, 50, 50, 100, 25, hWnd, (HMENU)ID_DENY_BUTTON, hInstance, NULL);
	HWND view_trustee = CreateWindow(L"button", L"View trustee", button_style, 175, 50, 100, 25, hWnd, (HMENU)ID_TRUSTEE_BUTTON, hInstance, NULL);

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

void Cleanup(PSID pSIDAdmin, PSID pSIDEveryone, PSECURITY_DESCRIPTOR pSD, PACL pNewDACL)
{
	if (pSIDAdmin)
		FreeSid(pSIDAdmin);
	if (pSIDEveryone)
		FreeSid(pSIDEveryone);
	if (pSD)
		LocalFree((HLOCAL)pSD);
	if (pNewDACL)
		LocalFree((HLOCAL)pNewDACL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/*case WM_CREATE: {
	}*/
	case WM_COMMAND: {
		// Name of object, here we will add ACE for a file
		// the file is already created
		LPCTSTR pszObjName = L"D:\\study\\system_programming\\course_work\\files\\4gb_patch.exe";
		// use openfile!!!
		
		// Result
		DWORD dwRes = 0;

		// Existing and new DACL pointers...
		PACL pDACL = NULL;
		
		// EXPLICIT_ACCESS structure.  For more than one entries,
		// declare an array of the EXPLICIT_ACCESS structure
		const int EAS = 2;
		EXPLICIT_ACCESS ea[EAS];

		// Reserving memory for security ID's
		PSID pSIDAdmin = NULL;
		PSID pSIDEveryone = NULL;

		// Security descriptor
		PSECURITY_DESCRIPTOR pSD = NULL;
		SecureZeroMemory(&pSD, sizeof(PSECURITY_DESCRIPTOR));

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_DENY_BUTTON: {
			SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
			SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;

			// Specify the DACL to use
			// Create a SID for the Everyone group
			if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID,
				0, 0, 0, 0, 0, 0, 0, &pSIDEveryone)) {
				MessageBox(hWnd, (LPCWSTR)GetLastError(), L"AllocateAndInitializeSid (Everyone) error", MB_OK);
				Cleanup(pSIDAdmin, pSIDEveryone, pSD, pDACL);
				return -1;
			}
			// Create a SID for the BUILTIN\Administrators group
			if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSIDAdmin)) {
				MessageBox(hWnd, (LPCWSTR)GetLastError(), L"AllocateAndInitializeSid (Admin) error", MB_OK);
				return -1;
			}

			ZeroMemory(&ea, EAS * sizeof(EXPLICIT_ACCESS));
			
			//---------------------------------------------
			// block access by certain user
			//---------------------------------------------
			// was
			/*ea[0].grfAccessPermissions = dwAccessRights;
			ea[0].grfAccessMode = AccessMode;
			ea[0].grfInheritance = dwInheritance;
			ea[0].Trustee.TrusteeForm = TrusteeForm;

			 Test for Administrators group, a new trustee for the ACE
			 For other trustees, you can try changing
			 the array index to 1, 2 and 3 and rerun, see the effect
			 ea[0].Trustee.ptstrName = (LPTSTR)(pszTrustee[0]);*/
			// endwas

			// became
			ea[0].grfAccessPermissions = GENERIC_READ;
			ea[0].grfAccessMode = SET_ACCESS;
			ea[0].grfInheritance = NO_INHERITANCE;
			ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea[0].Trustee.ptstrName = (LPTSTR)pSIDEveryone;

			ea[1].grfAccessPermissions = GENERIC_ALL;
			ea[1].grfAccessMode = SET_ACCESS;
			ea[1].grfInheritance = NO_INHERITANCE;
			ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
			ea[1].Trustee.ptstrName = (LPTSTR)pSIDAdmin;
			// endbecame

			// Create a new ACL that merges the new ACE into the existing DACL.
			dwRes = SetEntriesInAcl(EAS, ea, NULL, &pDACL);

			//  Verify
			if (dwRes != ERROR_SUCCESS) {
				MessageBox(hWnd, (LPCWSTR)dwRes, L"SetEntriesInAcl failed", MB_OK);
				Cleanup(pSIDAdmin, pSIDEveryone, pSD, pDACL);
				return -1;
			}

			// Attach the new ACL as the object's DACL.
			dwRes = SetNamedSecurityInfo((LPWSTR)pszObjName, SE_FILE_OBJECT,
				DACL_SECURITY_INFORMATION, NULL, NULL, pDACL, NULL);

			// was
			/*if (dwRes != ERROR_SUCCESS) {
				MessageBox(hWnd, (LPCWSTR)dwRes, L"SetNamedSecurityInfo failed", MB_OK);
				Cleanup(pSD, pNewDACL);
			}*/
			// endwas

			// became
			if (dwRes == ERROR_SUCCESS) {
				MessageBox(hWnd, L"Successfully changed DACL", L"Info", MB_OK);
				Cleanup(pSIDAdmin, pSIDEveryone, pSD, pDACL);
				return 0;
			}
			if (dwRes != ERROR_ACCESS_DENIED) {
				MessageBox(hWnd, (LPCWSTR)dwRes, L"First SetNamedSecurityInfo call failed", MB_OK);
				Cleanup(pSIDAdmin, pSIDEveryone, pSD, pDACL);
				return -1;
			}
			// endbecame
			break;
		}
		case ID_TRUSTEE_BUTTON: {
			DWORD SECURITY_INFORMATION = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
			// Get a pointer to the existing DACL.
			dwRes = GetNamedSecurityInfo(pszObjName, SE_FILE_OBJECT,
				SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSD);

			// Verify

			if (dwRes != ERROR_SUCCESS)
			{
				MessageBox(hWnd, (LPCWSTR)dwRes, L"GetNamedSecurityInfo failed", MB_OK);
				Cleanup(pSIDAdmin, pSIDEveryone, pSD, pDACL);
				return -1;
			}

			// Initialize an EXPLICIT_ACCESS structure for the new ACE.
			// For more entries, declare an array of the EXPLICIT_ACCESS structure
			SecureZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
			

			MessageBox(hWnd, L"Trustee worked perfectly", L"Info", MB_OK);

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
