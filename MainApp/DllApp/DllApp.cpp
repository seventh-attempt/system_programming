// DllApp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DllApp.h"

char *GetMachineGuid() {
	char value[37];
	DWORD BufferSize = sizeof(value);

	LONG operationResult = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid", RRF_RT_REG_SZ, NULL, value, &BufferSize);

	if (operationResult != ERROR_SUCCESS) {
		return NULL;
	}

	return value;
}
