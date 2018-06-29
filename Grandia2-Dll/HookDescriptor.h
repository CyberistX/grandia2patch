#pragma once
#include "stdafx.h"
#include <string>
#include <exception>

/*
	Generic templated class for a function hook descriptor
	T is assumed to be a typedef for the hooked function type
*/

template<class T>
class HookDescriptor
{

public:
	HookDescriptor<T>() {};
	~HookDescriptor<T>() {};

public:
	T originalFunction;
	T proxyFunction;
	DWORD memoryProtection;
	bool isSet = false;
	unsigned char oldBytes[6];
	unsigned char newBytes[6] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 };
	
public:

	/*
		Initializes a generic function hook.
		The generic parameter T must be a typedef for the hooked function.
	
		API Hooking is performed by rewriting the first bytes of the target 
		function with a jump instruction to our proxy function.
	*/

	template <typename T> 
	static HookDescriptor<T> CreateHook( void * oldFunction, void * newFunction )
	{

		HookDescriptor<T> self = HookDescriptor<T>();
		DWORD jumpSize = ( (DWORD) newFunction - (DWORD) oldFunction - 5 ); 
		memcpy ( &(self.newBytes[1]), &jumpSize, 4);

		memcpy (self.oldBytes, oldFunction, 6);
		CopyBytes (oldFunction, self.newBytes, 6);   

		self.originalFunction = (T) oldFunction;
		self.proxyFunction = (T) newFunction;
		self.isSet = true;
		
		return self;
	}
	
	HRESULT SetHook ()
	{
		if ( !this->isSet )
			CopyBytes ( (void *) this->originalFunction, this->newBytes, 6);
		this->isSet = true;
		return S_OK;
	}

	HRESULT UnsetHook()
	{
		if (this->isSet)
			CopyBytes ( (void *) this->originalFunction, this->oldBytes, 6);
		this->isSet = false;
		return S_OK;
	}

private:
	static HRESULT CopyBytes (void * destination, void * source, int numberOfBytes)
	{		

		DWORD oldMemoryProtection;
		int result = S_OK;

		VirtualProtect ( destination, numberOfBytes, PAGE_EXECUTE_READWRITE, &oldMemoryProtection );
		memcpy (destination, source, numberOfBytes);	

		DWORD tempMemoryProtection;
		VirtualProtect ( destination, numberOfBytes, oldMemoryProtection, &tempMemoryProtection ); 	
		return S_OK;
	}

};
