#pragma once
#include <Windows.h>
#include <tchar.h>
#include <psapi.h>
#include <TlHelp32.h>
#include <string>
#include <exception>

namespace PatchLib
{
	HRESULT WriteBytes(void * destination, void * source, int numberOfBytes);
	//void * GetModuleByName(DWORD processID, LPCWSTR moduleName);
	//void * GetFirstModule(DWORD processID);
	void * GetBaseAddress (void * threadHandle);

}
