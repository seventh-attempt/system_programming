#include "FirstDll.h"

using namespace std;

string GetSystemInformation() {
	SYSTEM_INFO siSysInfo;

	GetSystemInfo(&siSysInfo);
	stringstream stream;

	stream << "OEM ID: " << siSysInfo.dwOemId << "\r\n";
	stream << "Number of processors: " << siSysInfo.dwNumberOfProcessors << "\r\n";
	stream << "Page size: " << siSysInfo.dwPageSize << "\r\n";
	stream << "Processor type: " << siSysInfo.dwProcessorType << "\r\n";
	stream << "Minimum application address: " << siSysInfo.lpMinimumApplicationAddress << "\r\n";
	stream << "Maximum application address: " << siSysInfo.lpMaximumApplicationAddress << "\r\n";
	stream << "Active processor mask: " << siSysInfo.dwActiveProcessorMask << "\r\n";


	return stream.str();
}

string GetMemoryInformation() {
	MEMORYSTATUSEX lpBuffer = { sizeof(lpBuffer) };
	GlobalMemoryStatusEx(&lpBuffer);
	static const int DIV = 1024;
	static const int SIZE = 7;
	stringstream stream;

	stream << "There is  " << setw(SIZE) << setfill('0') << lpBuffer.dwMemoryLoad << "% percent of memory in use.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalPhys / DIV) << " total KB of physical memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailPhys / DIV) << " free  KB of physical memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalPageFile / DIV) << " total KB of paging file.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailPageFile / DIV) << " free  KB of paging file.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullTotalVirtual / DIV) << " total KB of virtual memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailVirtual / DIV) << " free  KB of virtual memory.\r\n";
	stream << "There are " << setw(SIZE) << setfill('0') << (lpBuffer.ullAvailExtendedVirtual / DIV) << " free  KB of extended memory.\r\n";

	return stream.str();
}