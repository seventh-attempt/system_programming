// DllApp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DllApp.h"

bool IsCorrectMachineGuid() {
	char value[37];
	DWORD BufferSize = sizeof(value);

	LONG operationResult = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid", RRF_RT_REG_SZ, NULL, value, &BufferSize);

	if (operationResult != ERROR_SUCCESS) {
		return false;
	}

	if (strcmp(value, "7fef1237-4ade-41c0-bf14-cf5b1d4bfad8") != 0) {
		return false;
	}

	return true;
}
