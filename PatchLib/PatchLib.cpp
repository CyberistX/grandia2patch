#include "stdafx.h"
#include "PatchLib.h"

HRESULT PatchLib::WriteBytes(void * destination, void * source, int numberOfBytes)
{

	DWORD destinationMemoryProtection;
	int result = S_OK;
	result += VirtualProtect(destination, numberOfBytes, PAGE_EXECUTE_READWRITE, &destinationMemoryProtection);
	memcpy(destination, source, numberOfBytes);

	DWORD oldMemoryProtection;
	result += VirtualProtect(destination, numberOfBytes, destinationMemoryProtection, &oldMemoryProtection);
	//if ( !result )
	//throw std::exception("Byte copy failed");
	//return S_OK;		
	return S_OK;
}

void * PatchLib::GetBaseAddress(void * threadHandle)
{
	CONTEXT context;
	context.ContextFlags = CONTEXT_ALL;
	if (!GetThreadContext(threadHandle, &context) != 0)
		throw std::exception("couldn't get thread context");

	return ( void * ) context.Eip;
}


//
//void * PatchLib::GetModuleByName(DWORD processID, LPCWSTR moduleName)
//{
//	HANDLE moduleSnapshot = INVALID_HANDLE_VALUE;
//	MODULEENTRY32 moduleEntry;
//
//	//  Take a snapshot of all modules in the specified process. 
//	moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32, processID);
//	if (moduleSnapshot == INVALID_HANDLE_VALUE)
//	{
//		DWORD tmp = GetLastError();
//		throw std::exception("Couldn't create toolhelp snapshot");
//	}
//
//	//  Set the size of the structure before using it. 
//	moduleEntry.dwSize = sizeof(MODULEENTRY32);
//
//	//  Retrieve information about the first module, 
//	//  and exit if unsuccessful 
//	if (!Module32First(moduleSnapshot, &moduleEntry))
//	{
//		DWORD tmp = GetLastError();
//		throw std::exception("Couldn't get first module handle");
//	}
//
//	//  Now walk the module list of the process, 
//	//  and display information about each module 
//	do
//	{
//		if ( _wcsicmp(moduleEntry.szModule, moduleName))
//			return (void *) moduleEntry.modBaseAddr;
//
//	} while (Module32Next(moduleSnapshot, &moduleEntry));
//
//	CloseHandle( moduleSnapshot);
//	throw std::exception("Couldn't find specified module");
//	/*HMODULE processModules[1024];
//	DWORD numberOfModules;
//	bool result = EnumProcessModulesEx(processHandle, processModules, sizeof(processModules), &numberOfModules, LIST_MODULES_32BIT);
//	if (result)
//	{
//		for (unsigned int i = 0; i < (numberOfModules / sizeof(HMODULE)); i++)
//		{
//			char currentModuleName[40];
//
//			if (GetModuleBaseNameA(processHandle, processModules[i], currentModuleName, 40))
//			{
//				return (void *) processModules[i];
//			}
//		}
//	}
//
//	HRESULT err = GetLastError();
//	throw std::exception("couldn't locate module");*/
//}
