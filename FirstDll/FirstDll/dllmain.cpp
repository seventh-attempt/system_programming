// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		MessageBox(NULL, L"PROCESS_1_ATTACH", L"msg", MB_OK);
		break;
	}
	case DLL_THREAD_ATTACH: {
		MessageBox(NULL, L"THREAD_1_ATTACH", L"msg", MB_OK);
		break; 
	}
	case DLL_THREAD_DETACH: {
		MessageBox(NULL, L"THREAD_1_DETACH", L"msg", MB_OK);
		break;
	}
	case DLL_PROCESS_DETACH: {
		MessageBox(NULL, L"PROCESS_1_DETACH", L"msg", MB_OK);
		break;
	}
	}
	return TRUE;
}
