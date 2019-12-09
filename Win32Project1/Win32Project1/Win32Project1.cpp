// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include "string"

#define MAX_LOADSTRING 100
#define TIMER_SYSINFO 1
#define TIMER_MEMINFO 2
#define TIMER_ALLOCINFO 3
#define TIMER_HEAPINFO 4
#define Second 1000

using namespace std;

struct MEMORY {
	MEMORY_BASIC_INFORMATION inf;
	bool lock = false;
	MEMORY(LPVOID pMemory) {
		VirtualQuery(pMemory, &inf, sizeof(inf));
	}
	string GetProtect() {
		stringstream stream;

		switch (inf.Protect)
		{
		case PAGE_EXECUTE: {
			stream << "PAGE_EXECUTE ";
			break;
		}
		case PAGE_EXECUTE_READ: {
			stream << "PAGE_EXECUTE_READ ";
			break;
		}
		case PAGE_EXECUTE_READWRITE: {
			stream << "PAGE_EXECUTE_READWRITE ";
			break;
		}
		case PAGE_EXECUTE_WRITECOPY: {
			stream << "PAGE_EXECUTE_WRITECOPY ";
			break;
		}
		case PAGE_NOACCESS: {
			stream << "PAGE_NOACCESS ";
			break;
		}
		case PAGE_READONLY: {
			stream << "PAGE_READONLY ";
			break;
		}
		case PAGE_READWRITE: {
			stream << "PAGE_READWRITE ";
			break;
		}
		case PAGE_WRITECOPY: {
			stream << "PAGE_WRITECOPY ";
			break;
		}
		default: {
			stream << "UNKNOWN ";
			break;
		}
		}

		return stream.str();
	}
	string GetState() {
		stringstream stream;

		switch (inf.State)
		{
		case MEM_RESERVE: {
			stream << "MEM_RESERVE ";
			break;
		}
		case MEM_COMMIT: {
			stream << "MEM_COMMIT ";
			break;
		}
		case MEM_RESET: {
			stream << "MEM_RESET ";
			break;
		}
		case MEM_RESET_UNDO: {
			stream << "MEM_RESET_UNDO ";
			break;
		}
		case MEM_LARGE_PAGES: {
			stream << "MEM_LARGE_PAGES ";
			break;
		}
		case MEM_PHYSICAL: {
			stream << "MEM_PHYSICAL ";
			break;
		}
		case MEM_TOP_DOWN: {
			stream << "MEM_TOP_DOWN ";
			break;
		}
		case MEM_WRITE_WATCH: {
			stream << "MEM_WRITE_WATCH ";
			break;
		}
		default: {
			stream << "UNKNOWN ";
			break;
		}
		}

		return stream.str();
	}
	string GetType() { 
		stringstream stream;

		switch (inf.Type)
		{
		case MEM_IMAGE: {
			stream << "MEM_IMAGE ";
			break;
		}
		case MEM_MAPPED: {
			stream << "MEM_MAPPED ";
			break;
		}
		case MEM_PRIVATE: {
			stream << "MEM_PRIVATE ";
			break;
		}
		default: {
			stream << "UNKNOWN ";
			break;
		}
		}

		return stream.str();
	}
	string GetString() {
		stringstream stream;

		VirtualQuery(inf.AllocationBase, &inf, sizeof(inf));

		stream << "  Address : " << setw(8) << hex << uppercase << setfill('0') << inf.BaseAddress << "\r\n";
		stream << "  Size    : " << setw(8) << hex << uppercase << setfill('0') << inf.RegionSize << "\r\n";
		stream << "  Lock    : " << boolalpha << lock << setw(8) << setfill(' ') << "\r\n";
		stream << "  Type    : " << GetType() << "\r\n";
		stream << "  State   : " << GetState() << "\r\n";
		stream << "  Protect : " << GetProtect() << "\r\n";

		return stream.str();
	}
	int Lock() {
		if (lock) {
			return 0;
		}

		lock = true;

		return VirtualLock(inf.BaseAddress,inf.RegionSize);
	}
	int UnLock() {
		if (!lock) {
			return 0;
		}

		lock = false;
		
		return VirtualUnlock(inf.BaseAddress, inf.RegionSize);
	}
	int Free() {
		return VirtualFree(inf.BaseAddress, 0, MEM_RELEASE);
	}
};

SYSTEM_INFO siSysInfo;
MEMORYSTATUSEX lpBuffer = { sizeof(lpBuffer) };

HWND hDialogSysInf = NULL;
HWND hDialogMemInf = NULL;
HWND hDialogAllocInf = NULL;
HWND hDialogHeapInf = NULL;
HWND hDialogWorkInf = NULL;

void GetSystemInformation(HWND hEdit);
void GetMemoryInformation(HWND hEdit);
void ActivateDialog(HWND hWnd, HWND hDialog, int IDD_DIALOG, DLGPROC foo, int hTimer);

INT_PTR CALLBACK SysInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MemInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AllocInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HeapInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK WorkInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
	switch (message)
	{
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT: {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_SYSINFO: {
			ActivateDialog(hWnd, hDialogSysInf, IDD_DIALOG_SYSINFO, SysInfo, TIMER_SYSINFO);
			break;
		}
		case IDM_MEMINFO: {
			ActivateDialog(hWnd, hDialogMemInf, IDD_DIALOG_MEMINFO, MemInfo, TIMER_MEMINFO);
			break;
		}
		case IDM_ALLOCINFO: {
			ActivateDialog(hWnd, hDialogAllocInf, IDD_DIALOG_ALLOCINFO, AllocInfo, TIMER_ALLOCINFO);
			break;
		}
		case IDM_HEAPINFO: {
			ActivateDialog(hWnd, hDialogHeapInf, IDD_DIALOG_HEAPINFO, HeapInfo, TIMER_HEAPINFO);
			break;
		}
		case ID_MEM_ADDITIONALTASK: {
			static LPVOID pMemory = VirtualAlloc(0, 4096 * 40, MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
			static LPVOID pAvMemory = VirtualAlloc(LPVOID(4096*19 - 4096/2), 4096 * 3, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);

			static LPVOID start_block = LPVOID((int)pAvMemory + 4096 /2);
			static LPVOID central_block = LPVOID((int)pAvMemory + 4096);
			static LPVOID end_block = LPVOID((int)pAvMemory + 4096 - 4);

			stringstream first_name;
			stringstream second_name;
			int year = 1999;

			stringstream r_first_name;
			stringstream r_second_name;
			int r_year;

			first_name << "Dmitry";
			second_name << "Khramkov";

			CopyMemory(start_block, &first_name , sizeof(first_name));
			CopyMemory(central_block, &second_name, sizeof(second_name));
			CopyMemory(end_block, &year, 4);

			CopyMemory(&r_first_name, start_block, sizeof(first_name));
			CopyMemory(&r_second_name, central_block, sizeof(second_name));
			CopyMemory(&r_year, end_block, 4);

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

INT_PTR CALLBACK SysInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		GetSystemInformation(GetDlgItem(hDlg, IDC_EDIT_SYSINFO));
		return (INT_PTR)TRUE;
	}
	case WM_TIMER: {
		GetSystemInformation(GetDlgItem(hDlg, IDC_EDIT_SYSINFO));
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDCANCEL) {
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
		GetMemoryInformation(GetDlgItem(hDlg, IDC_EDIT_MEMINFO));
		return (INT_PTR)TRUE;
	}
	case WM_TIMER: {
		GetMemoryInformation(GetDlgItem(hDlg, IDC_EDIT_MEMINFO));
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
INT_PTR CALLBACK AllocInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD CHECK_PARAM = 0;
	static DWORD RADIO_PARAM = 0;
	static MEMORY *pMemory = NULL;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG: {
			SendDlgItemMessage(hDlg, IDC_CHECK_MEM_RESERVE, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_CHECK_MEM_COMMIT, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_CHECK_MEM_TOP_DOWN, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_READWRITE, BM_SETCHECK, BST_CHECKED, 0);
			return (INT_PTR)TRUE;
		}
		case WM_TIMER: {
			stringstream stream;
			stream << "MEMORY ALLOCATION : \r\n";

			if (pMemory != NULL) {
				stream << pMemory->GetString();
			}
			else {
				stream << "  NONE\r\n";
			}

			SendMessageA(GetDlgItem(hDlg, IDC_EDIT_ALLOCINFO), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
			break;
		}
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case IDC_BUTTON_ALLOCINFO_ALLOCATE: {
				CHECK_PARAM = 0;
				RADIO_PARAM = 0;

				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_RESERVE, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_RESERVE;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_COMMIT, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_COMMIT;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_RESET, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_RESET;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_RESET_UNDO, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_RESET_UNDO;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_LARGE_PAGES, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_LARGE_PAGES;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_PHYSICAL, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_PHYSICAL;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_TOP_DOWN, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_TOP_DOWN;
				if (SendDlgItemMessage(hDlg, IDC_CHECK_MEM_WRITE_WATCH, BM_GETCHECK, 0, 0)) CHECK_PARAM = CHECK_PARAM | MEM_WRITE_WATCH;

				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_EXECUTE, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_EXECUTE;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_EXECUTE_READ, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_EXECUTE_READ;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_EXECUTE_READWRITE, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_EXECUTE_READWRITE;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_EXECUTE_WRITECOPY, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_EXECUTE_WRITECOPY;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_NOACCESS, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_NOACCESS;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_READONLY, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_READONLY;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_READWRITE, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_READWRITE;
				if (SendDlgItemMessage(hDlg, IDC_RADIO_PAGE_WRITECOPY, BM_GETCHECK, 0, 0)) RADIO_PARAM = PAGE_WRITECOPY;

				int address = GetDlgItemInt(hDlg, IDC_EDIT_ALLOCINFO_ADDRESS, NULL, FALSE);
				int size = GetDlgItemInt(hDlg, IDC_EDIT_ALLOCINFO_SIZE, NULL, FALSE);
				LPVOID NewAddress = VirtualAlloc((LPVOID)address, size, CHECK_PARAM, RADIO_PARAM);

				stringstream stream;

				if (!pMemory && NewAddress != NULL) {

					pMemory = new MEMORY(NewAddress);

					stream << "Memory allocation was successful!\r\n";
					stream << "  Input  address : " << setw(8) << hex << uppercase << setfill('0') << address << "\r\n";
					stream << "  Input  size    : " << setw(8) << hex << uppercase << setfill('0') << size << "\r\n";

					stream << "  Output address : " << setw(8) << hex << uppercase << setfill('0') << pMemory->inf.BaseAddress << "\r\n";
					stream << "  Output size    : " << setw(8) << hex << uppercase << setfill('0') << pMemory->inf.RegionSize << "\r\n";

				}
				else stream << "Memory allocation failed!\r\n";

				SendMessageA(GetDlgItem(hDlg, IDC_EDIT_ALLOCINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
				break;
			}
			case IDC_BUTTON_ALLOCINFO_FREE: {
				stringstream stream;

				if (pMemory != NULL && pMemory->Free() > 0) {
					stream << "Memory has been freed!\r\n";
					delete[] pMemory;
					pMemory = NULL;
				}
				else {
					stream << "Memory has not been freed!\r\n";
				}

				SendMessageA(GetDlgItem(hDlg, IDC_EDIT_ALLOCINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
				break;
			}
			case IDC_BUTTON_ALLOCINFO_LOCK: {
				stringstream stream;

				if (pMemory) {
					if (pMemory->Lock() > 0) stream << "Memory lock was successful!\r\n";
					else stream << "Memory lock failed!\r\n";
				}
				else {
					stream << "Memory lock failed!\r\n";
				}

				SendMessageA(GetDlgItem(hDlg, IDC_EDIT_ALLOCINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
				break;
			}
			case IDC_BUTTON_ALLOCINFO_UNLOCK: {
				stringstream stream;

				if (pMemory) {
					if (pMemory->UnLock() > 0) stream << "Memory unlock was successful!\r\n";
					else stream << "Memory unlock failed!\r\n";
				}
				else {
					stream << "Memory unlock failed!\r\n";
				}

				SendMessageA(GetDlgItem(hDlg, IDC_EDIT_ALLOCINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
				break;
			}
			case IDCANCEL: {
				KillTimer(hDlg, TIMER_ALLOCINFO);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			}
		}
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK HeapInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE pHeap = NULL;
	static LPVOID pHeapMemory = NULL;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_TIMER: {
		stringstream stream;
		stream << "HEAP INFORMATION : \r\n";

		if (pHeap != NULL) {
			stream << "  Handle  : " << hex << setw(8) << setfill('0') << pHeap << "\r\n";
			stream << "  Address : " << hex << setw(8) << setfill('0') << pHeapMemory << "\r\n";

			if (pHeapMemory) {
				stream << "  Size    : " << hex << setw(8) << setfill('0') << HeapSize(pHeap, NULL, pHeapMemory) << "\r\n";
			}
			else {
				stream << "  Size    : " << hex << setw(8) << setfill('0') << 0 << "\r\n";
			}
		}
		else {
			stream << "  NONE\r\n";
		}

		SendMessageA(GetDlgItem(hDlg, IDC_EDIT_HEAPINFO_SETTINGS), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_HEAPINFO_CREATE: {
			stringstream stream;

			if (!pHeap) {

				int size = GetDlgItemInt(hDlg, IDC_EDIT_HEAPINFO_SIZE, NULL, FALSE);
				pHeap = HeapCreate(HEAP_NO_SERIALIZE, size, size);

				if (pHeap != NULL) stream << "Heap creation was successful!\r\n";
				else stream << "Heap creation failed!\r\n";
			}
			else {
				stream << "Heap creation failed!\r\n";
			}

			SendMessageA(GetDlgItem(hDlg, IDC_EDIT_HEAPINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
			break;
		}
		case IDC_BUTTON_HEAPINFO_DELETE: {
			stringstream stream;

			if (pHeap != NULL) {
				if (HeapDestroy(pHeap) != NULL) {
					pHeap = NULL;
					pHeapMemory = NULL;
					stream << "Heap delete was successful!\r\n";
				}
				else {
					stream << "Heap deleted failed!\r\n";
				}
			}
			else {
				stream << "Heap deleted failed!\r\n";
			}

			SendMessageA(GetDlgItem(hDlg, IDC_EDIT_HEAPINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
			break;
		}
		case IDC_BUTTON_HEAPINFO_ALLOCE: {
			stringstream stream;

			if (pHeap != NULL) {
				int size = GetDlgItemInt(hDlg, IDC_EDIT_HEAPINFO_SIZE, NULL, FALSE);
				pHeapMemory = HeapAlloc(pHeap, NULL, size);

				if (pHeapMemory != NULL) {
					stream << "Heap allocation was successful!\r\n";
				}
				else {
					stream << "Heap allocation failed!\r\n";
				}
			}
			else {
				stream << "Heap allocation failed!\r\n";
			}

			SendMessageA(GetDlgItem(hDlg, IDC_EDIT_HEAPINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
			break;
		}
		case IDC_BUTTON_HEAPINFO_FREE: {
			stringstream stream;

			if (pHeap != NULL) {

				if (HeapFree(pHeap, NULL, pHeapMemory) != NULL) {
					pHeapMemory = NULL;
					stream << "Heap has been freed!\r\n";
				}
				else {
					stream << "Heap has not been freed!\r\n";
				}
			}
			else {
				stream << "Heap has not been freed!\r\n";
			}

			SendMessageA(GetDlgItem(hDlg, IDC_EDIT_HEAPINFO_ERROR), WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
			break;
		}
		case IDCANCEL: {
			KillTimer(hDlg, TIMER_HEAPINFO);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}
	}
	}
	return (INT_PTR)FALSE;
}

void ActivateDialog(HWND hWnd, HWND hDialog,int IDD_DIALOG, DLGPROC foo,int hTimer) {
	
	if (hDialog == NULL) {
		hDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG), hWnd, foo);

		SetParent(hDialog, hWnd);

		SetTimer(hDialog, hTimer, Second/2, NULL);
		ShowWindow(hDialog, SW_SHOW);
	}
	else {
		SetTimer(hDialog, hTimer, Second/2, NULL);
		ShowWindow(hDialog, SW_SHOW);
	}
}
void GetSystemInformation(HWND hEdit) {
	GetSystemInfo(&siSysInfo);
	stringstream stream;

	stream << "OEM ID: " << siSysInfo.dwOemId << "\r\n";
	stream << "Number of processors: " << siSysInfo.dwNumberOfProcessors << "\r\n";
	stream << "Page size: " << siSysInfo.dwPageSize << "\r\n";
	stream << "Processor type: " << siSysInfo.dwProcessorType << "\r\n";
	stream << "Minimum application address: " << siSysInfo.lpMinimumApplicationAddress << "\r\n";
	stream << "Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << "\r\n";
	stream << "Active processor mask: " << siSysInfo.dwActiveProcessorMask << "\r\n";

	SendMessageA(hEdit, WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
}
void GetMemoryInformation(HWND hEdit) {
	GlobalMemoryStatusEx(&lpBuffer);
	static const int DIV = 1024;
	static const int SIZE = 7;
	stringstream stream;

	stream << "There is  " << setw(SIZE) << setfill('0') << lpBuffer.dwMemoryLoad  << "% percent of memory in use.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalPhys / DIV) << " total KB of physical memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailPhys / DIV) << " free  KB of physical memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalPageFile / DIV) << " total KB of paging file.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailPageFile / DIV) << " free  KB of paging file.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalVirtual / DIV) << " total KB of virtual memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailVirtual / DIV) << " free  KB of virtual memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailExtendedVirtual / DIV) << " free  KB of extended memory.\r\n";

	SendMessageA(hEdit, WM_SETTEXT, 0, (LPARAM)stream.str().c_str());
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
