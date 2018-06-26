#include "stdafx.h"
#include "PatchLib.h"

HRESULT PatchLib::WriteBytes(void * destination, void * source, int numberOfBytes)
{

	DWORD destinationMemoryProtection;
	DWORD sourceMemoryProtection;
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
