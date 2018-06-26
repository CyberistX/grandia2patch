#pragma once
#include <Windows.h>

namespace PatchLib
{
	HRESULT WriteBytes(void * destination, void * source, int numberOfBytes);
	void * GetFirstModule(void * process);

}
