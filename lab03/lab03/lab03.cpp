// lab03.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "lab03.h"
#include <iostream>
#include <string>


#define MAX_LOADSTRING 100

#define PATH L"\\\\.\\F:"
#define DUMP_SIZE 512
#define META_DATA_BOOT_SIZE 18
#define SMALL_FILE 0x40009C00


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND hEditDump;
HWND hEditMetaData;
HWND hEditFile;
HANDLE hFile = NULL;
char DUMP[DUMP_SIZE];

using namespace std;

typedef struct fileBoot{
	BYTE dJump[3];
	BYTE dSystemId[8];
	WORD dBytesPerSector;
	BYTE dSectorPerCuster;
	BYTE dUnusedA[5];
	BYTE dMediaD;
	BYTE dUnuseB[2];
	WORD dSectorPerTrack;
	BYTE dUnuseC[8];
	BYTE dUsualy[4];
	INT64 dNumberOSectors;
	INT64 dLCNofMFT;
	INT64 dLCNofMFTMirr;
	DWORD ClusterPerMFT;
	DWORD ClusterPerIndexes;
	BYTE dSerialNumber[8];
	BYTE dDataCoder[432];
} METADATABOOT;
typedef struct StandartInformation {

	FILETIME FileCreated;
	FILETIME FileModifider;
	FILETIME RecordChanged;
	FILETIME LastAccess;

	DWORD FilePremissions;
	DWORD MaxNumberOfVersion;
	DWORD VersionNumber;
	DWORD ClassId;
	DWORD OwnewId;
	DWORD SecurityId;
	INT64 QuotaCharged;
	INT64 UpdateSequenceNumber;
} inf;

int SIZE_META_DATA[META_DATA_BOOT_SIZE] = {
	3, 8, 2,
	1, 7, 1,
	2, 2, 2,
	8, 4, 8,
	8, 8, 4,
	4, 8, 432
};
int OFFSET_META_DATA[META_DATA_BOOT_SIZE] = {
	0x0, 0x3, 0xb,
	0xd, 0xe, 0x15,
	0x16, 0x18, 0x1a,
	0x1c, 0x24, 0x28,
	0x30, 0x38, 0x40,
	0x44, 0x48, 0x50
};

char NAME_META_DATA[META_DATA_BOOT_SIZE][40] = {
	{ "Jump to the boot loader routine: " }, { "System Id: 'NTFS': " }, { "Bytes per sector: " },
	{ "Sectors per claster: " }, { "Unused: " }, { "Media descriptor (a): " },
	{ "Unused: " }, { "Sectors per track: " }, { "Number of heads: " },
	{ "Unused: " }, { "Usually 80 00 80 00 (b): " }, { "Number of sectors in the volume: " },
	{ "LCN of VCN 0 of the $MFT: " }, { "LCN of VCN 0 of the $MFTMirr: " }, { "Clasters per MFT Record (c): " },
	{ "Clasters per Index Record (c): " }, { "Volume serial number: " }, { "Bootable code: " }
};

void ReadDumpBoot(HANDLE hFile, char* Dump, int size);
void ReadDumpMFT(HANDLE hFile, char* Dump, int size);
void ReadDumpFile(HANDLE hFile, char* Dump, int size);
void SendDump(HWND hEdit, char*Dump, int size);

void SendStandartInformation(HWND hEdit, char*Dump);
void SendData(HWND hEdit, char*Dump);
void SendFileName(HWND hEdit, char*Dump);
void SendFileAttributes(HWND hEdit, char*Dump);

char* ReadMetaDataBoot(char*Dump, int offset, int size);
void SendMetaDataBoot(HWND hEdit, char* msg);

char* ReadMFT(char*Dump, int offset, int size);
void SendMFT(HWND hEdit, char* msg);


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
    LoadStringW(hInstance, IDC_LAB03, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB03));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB03));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB03);
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
		RECT rect;
		GetClientRect(hWnd, &rect);

		/*hEditDump = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, rect.top, rect.right / 3, rect.bottom - rect.top, hWnd, nullptr, hInst, nullptr);*/

		hEditMetaData = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.left, rect.top, rect.right / 1.5, rect.bottom, hWnd, nullptr, hInst, nullptr);

		hEditFile = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPSIBLINGS | WS_BORDER | WS_VSCROLL,
			rect.right - rect.right / 3, rect.top, rect.right - rect.right / 1.5, rect.bottom, hWnd, nullptr, hInst, nullptr);

		//SendMessageA(hEditDump, EM_REPLACESEL, 0, (LPARAM)"Dump Boot record: \r\n");
		SendMessageA(hEditMetaData, EM_REPLACESEL, 0, (LPARAM)"Meta Data Boot Record: \r\n");

		//hFile = CreateFile(L"\\\\.\\F:\\", GENERIC_READ, FILE_SHARE_READ, NULL, 
		hFile = CreateFile(PATH, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE) {
			ReadDumpBoot(hFile, DUMP, DUMP_SIZE);
			//SendDump(hEditDump, DUMP, DUMP_SIZE);

			char tab[] = "\t* ";
			char new_line[] = "\r\n";

			for (int i = 0; i < META_DATA_BOOT_SIZE; i++) {
				SendMetaDataBoot(hEditMetaData, tab);
				SendMetaDataBoot(hEditMetaData, NAME_META_DATA[i]);
				SendMetaDataBoot(hEditMetaData, ReadMetaDataBoot(DUMP, OFFSET_META_DATA[i], SIZE_META_DATA[i]));
				SendMetaDataBoot(hEditMetaData, new_line);
			}

			ReadDumpFile(hFile, DUMP, DUMP_SIZE);
			SendFileAttributes(hEditFile, DUMP);
		}
		else {
			MessageBox(NULL, L"File doesn't exist", L"Error", MB_OK | MB_ICONWARNING);
		}

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

void ReadDumpBoot(HANDLE hFile, char* Dump, int size) {
	SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
	ReadFile(hFile, Dump, size, NULL, NULL);
	SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
}
void ReadDumpMFT(HANDLE hFile, char* Dump, int size) {
	INT64 offset = *((INT64*)(Dump + OFFSET_META_DATA[12])) * 8 * 512;

	SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
	ReadFile(hFile, Dump, size, NULL, NULL);
	SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
}
void ReadDumpFile(HANDLE hFile, char* Dump, int size) {
	SetFilePointer(hFile, SMALL_FILE, NULL, FILE_BEGIN);
	ReadFile(hFile, Dump, size, NULL, NULL);
	SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
}
void SendDump(HWND hEdit, char*Dump, int size) {
	char buffer[4];

	for (int i = 0; i < size; i++) {

		if (i % 16 == 0 && i != 0) {
			SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
		}

		sprintf(buffer, "%0.2X ", (unsigned char)Dump[i]);
		SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)buffer);
	}
}

char* ReadMetaDataBoot(char*Dump, int offset, int size) {
	char *result = new char[size * 2 + size];
	int ofst = 0;

	for (int i = offset; i < offset + size; i++) {
		ofst += sprintf(result + ofst, "%0.2X ", (unsigned char)Dump[i]);
	}

	return result;
}
void SendMetaDataBoot(HWND hEdit, char* msg) {
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)msg);
}

void SendStandartInformation(HWND hEdit, char*Dump) {
	inf info = *((inf*)(Dump + 0x50));
	LPSYSTEMTIME lpSystemTime = new _SYSTEMTIME();
	string text = "";

	FileTimeToSystemTime(&info.FileCreated, lpSystemTime);
	text += string("File Created: ") + to_string(lpSystemTime->wDay) + string(".") +
		to_string(lpSystemTime->wMonth) +
		string(".") + to_string(lpSystemTime->wYear) + string("  ")
		+ to_string(lpSystemTime->wHour) + string(":")
		+ to_string(lpSystemTime->wMinute) + string("\r\n");

	FileTimeToSystemTime(&info.FileModifider, lpSystemTime);
	text += string("File Modifider: ") + to_string(lpSystemTime->wDay) +
		string(".") + to_string(lpSystemTime->wMonth) +
		string(".") + to_string(lpSystemTime->wYear) + string("  ")
		+ to_string(lpSystemTime->wHour) + string(":")
		+ to_string(lpSystemTime->wMinute) + string("\r\n");

	FileTimeToSystemTime(&info.RecordChanged, lpSystemTime);
	text += string("Record Changed: ") + to_string(lpSystemTime->wDay) +
		string(".") + to_string(lpSystemTime->wMonth) +
		string(".") + to_string(lpSystemTime->wYear) + string("  ")
		+ to_string(lpSystemTime->wHour) + string(":")
		+ to_string(lpSystemTime->wMinute) + string("\r\n");

	FileTimeToSystemTime(&info.LastAccess, lpSystemTime);
	text += string("Last Access: ") + to_string(lpSystemTime->wDay) +
		string(".") + to_string(lpSystemTime->wMonth) +
		string(".") + to_string(lpSystemTime->wYear) + string("  ")
		+ to_string(lpSystemTime->wHour) + string(":")
		+ to_string(lpSystemTime->wMinute) + string("\r\n");

	text += string("File Premissions: ") + to_string(info.FilePremissions) + string("\r\n");
	text += string("Max Number Of Version: ") + to_string(info.MaxNumberOfVersion) + string("\r\n");
	text += string("Version Number: ") + to_string(info.VersionNumber) + string("\r\n");
	text += string("Class Id: ") + to_string(info.ClassId) + string("\r\n");
	text += string("Ownew Id: ") + to_string(info.OwnewId) + string("\r\n");
	text += string("Security Id: ") + to_string(info.SecurityId) + string("\r\n");
	text += string("Quota Charged: ") + to_string(info.QuotaCharged) + string("\r\n");
	text += string("Update Sequence Number: ") + to_string(info.UpdateSequenceNumber) + string("\r\n");

	SendMessageA(hEditFile, EM_REPLACESEL, 0, (LPARAM)"$STANDART_INFORMATION: \r\n");
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
}
void SendData(HWND hEdit, char*Dump) {
	int start = 0x130 + *((WORD*)(Dump + 0x13A));
	int end = start + *((WORD*)(Dump + 0x140));
	string text = string(&Dump[start], end - start);

	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n$DATA: \r\n");
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
}
void SendFileName(HWND hEdit, char*Dump) {
	int start = 0xf2;
	int end = start + *((WORD*)(Dump + 0xf0));
	wstring text = wstring((WCHAR*)&Dump[start], end - start);

	SendMessageA(hEditFile, EM_REPLACESEL, 0, (LPARAM)"$FILE_NAME: \r\n");
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
}
void SendFileAttributes(HWND hEdit, char*Dump) {
	string text = "";
	int offset = 0x38;
	int typeAttribute = 0;

	while (typeAttribute != 0xFFFFFFFF)
	{
		typeAttribute = *((int*)(Dump + offset));

		switch (typeAttribute) {
		case 0x10:
			text += "$STANDART_INFORMATION:\r\n";
			{
				inf info = *((inf*)(Dump + offset + 0x18));

				LPSYSTEMTIME lpSystemTime = new _SYSTEMTIME();

				FileTimeToSystemTime(&info.FileCreated, lpSystemTime);
				text += string("\tFile Created: ") + to_string(lpSystemTime->wDay) + string(".") +
					to_string(lpSystemTime->wMonth) +
					string(".") + to_string(lpSystemTime->wYear) + string("  ")
					+ to_string(lpSystemTime->wHour) + string(":")
					+ to_string(lpSystemTime->wMinute) + string("\r\n");

				FileTimeToSystemTime(&info.FileModifider, lpSystemTime);
				text += string("\tFile Modifider: ") + to_string(lpSystemTime->wDay) +
					string(".") + to_string(lpSystemTime->wMonth) +
					string(".") + to_string(lpSystemTime->wYear) + string("  ")
					+ to_string(lpSystemTime->wHour) + string(":")
					+ to_string(lpSystemTime->wMinute) + string("\r\n");

				FileTimeToSystemTime(&info.RecordChanged, lpSystemTime);
				text += string("\tRecord Changed: ") + to_string(lpSystemTime->wDay) +
					string(".") + to_string(lpSystemTime->wMonth) +
					string(".") + to_string(lpSystemTime->wYear) + string("  ")
					+ to_string(lpSystemTime->wHour) + string(":")
					+ to_string(lpSystemTime->wMinute) + string("\r\n");

				FileTimeToSystemTime(&info.LastAccess, lpSystemTime);
				text += string("\tLast Access: ") + to_string(lpSystemTime->wDay) +
					string(".") + to_string(lpSystemTime->wMonth) +
					string(".") + to_string(lpSystemTime->wYear) + string("  ")
					+ to_string(lpSystemTime->wHour) + string(":")
					+ to_string(lpSystemTime->wMinute) + string("\r\n");

				text += string("\tFile Premissions: ") + to_string(info.FilePremissions) + string("\r\n");
				text += string("\tMax Number Of Version: ") + to_string(info.MaxNumberOfVersion) + string("\r\n");
				text += string("\tClass Id: ") + to_string(info.ClassId) + string("\r\n");
				text += string("\tOwnew Id: ") + to_string(info.OwnewId) + string("\r\n");
				text += string("\tSecurity Id: ") + to_string(info.SecurityId) + string("\r\n");
				text += string("\tQuota Charged: ") + to_string(info.QuotaCharged) + string("\r\n");
				text += string("\tUpdate Sequence Number: ") + to_string(info.UpdateSequenceNumber) + string("\r\n\r\n");
			}
			break;
		case 0x20:
			text += "$ATTRIBUTE_LIST:\r\n";
			break;
		case 0x30:
		{
			text += "$FILE_NAME: ";
			int start = 0xf2;
			int end = start + *((WORD*)(Dump + 0xf0));
			wstring name = wstring((WCHAR*)&Dump[start], end - start) + L"\r\n\r\n";
			string n = string(name.begin(), name.end());

			text += n;
			break;
		}
		case 0x80:
			text += "$DATA:\r\n";
			{
				int start = 0x130 + *((WORD*)(Dump + 0x13A));
				int end = start + *((WORD*)(Dump + 0x140));
				text += string(&Dump[start], end - start) + "\r\n\r\n";
			}
			break;
		case 0x90:
			text += "$INDEX_ROOT:\r\n";
			break;
		case 0xB0:
			text += "$BITMAP:\r\n";
			break;
		}

		offset += *((int*)(Dump + offset + 4));
	}

	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)text.c_str());

}
char* ReadMFT(char*Dump, int offset, int size) {
	char *result = new char[size * 2 + size];
	int ofst = 0;

	for (int i = offset; i < offset + size; i++) {
		ofst += sprintf(result + ofst, "%0.2X ", (unsigned char)Dump[i]);
	}

	return result;
}
void SendMFT(HWND hEdit, char* msg) {
	SendMessageA(hEdit, EM_REPLACESEL, 0, (LPARAM)msg);
}
