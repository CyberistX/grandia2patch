// Grandia2-Launcher.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>


int main()
{

	const char* buffer = "Grandia2-Dll.dll";
	const char* exePath = "Grandia2.exe";



	/*
	* 
	*/
	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));

	HRESULT hResult = CreateProcessA(NULL, (LPSTR) exePath, NULL, NULL, 
		FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo);

	if (hResult == 0)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return E_FAIL;
	}

	// Wait until child process exits.
	//WaitForSingleObject(processInfo.hProcess, INFINITE);

	// Close process and thread handles. 
	//CloseHandle(processInfo.hProcess);
	//CloseHandle(processInfo.hThread);

	/*
	* Get process handle passing in the process ID.
	*/
	//int procID = 1056;

	int procID = processInfo.dwProcessId;
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	if (process == NULL) {
		printf("Error: the specified process couldn't be found.\n");
		return -1;
	}

	/*
	* Get address of the LoadLibrary function.
	*/
	LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");

	if (addr == NULL) {
		printf("Error: the LoadLibraryA function was not found inside kernel32.dll library.\n");
		return -2;
	}

	/*
	* Allocate new memory region inside the process's address space.
	*/
	LPVOID arg = (LPVOID)VirtualAllocEx(process, NULL, strlen(buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (arg == NULL) {
		printf("Error: the memory could not be allocated inside the chosen process.\n");
		return -3;
	}

	/*
	* Write the argument to LoadLibraryA to the process's newly allocated memory region.
	*/
	int n = WriteProcessMemory(process, arg, buffer, strlen(buffer), NULL);

	if (n == 0) {
		printf("Error: there was no bytes written to the process's address space.\n");
		return -4;
	}

	/*
	* Inject our DLL into the process's address space.
	*/
	HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, NULL, NULL);

	if (threadID == NULL) {
		printf("Error: the remote thread could not be created.\n");
		return -5;
	}
	else {
		printf("Success: the remote thread was successfully created.\n");
	}

	ResumeThread(processInfo.hThread);

	/*
	* Close the handle to the process, becuase we've already injected the DLL.
	*/
	//CloseHandle(process);
	// Close process and thread handles. 
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	getchar();

	return 0;
}

