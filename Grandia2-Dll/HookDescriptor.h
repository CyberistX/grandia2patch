#pragma once
#include "stdafx.h"
#include <string>
#include <exception>
#include "Logger.h"

/*
	Generic templated class for a function hook descriptor
	T is assumed to be a typedef for the hooked function type
*/


template<class T>
class HookDescriptor
{

public:
	HookDescriptor<T>() {};
	~HookDescriptor<T>()
	{
		delete[] oldBytes;
		delete[] newBytes;
		
	};

public:
	T originalFunction;
	T proxyFunction;
	DWORD memoryProtection;
	bool isSet;
	unsigned char * oldBytes;
	unsigned char * newBytes;
	
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
		//#ifdef _DEBUG
		std::string debugString = "Called Function CreateHook:\n";
		debugString += "\t void* oldFunction:\t";
		debugString += std::to_string((unsigned int)oldFunction);
		debugString += "\n";
		debugString += "\t void* newFunction:\t";
		debugString += std::to_string( (unsigned int) newFunction);
		debugString += "\n";

		Logger::Instance() -> Log (debugString);
	//	#endif

		HookDescriptor<T> self = HookDescriptor<T>();

		self.newBytes = new unsigned char [6];
		self.oldBytes = new unsigned char [6];
		unsigned char tempBuffer[6] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 };
		memcpy (self.newBytes, & (tempBuffer [0]), 6);

		DWORD jumpSize = ( (DWORD) newFunction - (DWORD) oldFunction - 5 ); 
		//CopyBytes (self.newBytes, &jumpSize, 4);
		memcpy(self.newBytes, &jumpSize, 4);

		memcpy(self.oldBytes, oldFunction, 6);
		CopyBytes (oldFunction, self.newBytes, 6);   

		self.originalFunction = (T) oldFunction;
		self.proxyFunction = (T) newFunction;
		self.isSet = true;
		return self;

	}

	/*
		Initializes a generic com function hook.
		The generic parameter T must be a typedef for the hooked function.
		The vtable index must be the index to the vtable entry ok the com 
		object containing a pointer to the hooked function.

		API Hooking is performed by rewriting the first bytes of the target 
		function with a jump instruction to our proxy function.
	*/

	template <typename T> 
	static HookDescriptor<T> CreateComHook( IUnknown * comObject, int vtableIndex, void * newFunction )
	{
		void * oldFunction = GetFunctionPointer (comObject, vtableIndex);
		return CreateHook (oldFunction, newFunction);		
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
			CopyBytes( (void *) this->originalFunction, this->oldBytes, 6);
		this->isSet = false;
		return S_OK;
	}


private:
	static HRESULT CopyBytes (void * destination, void * source, int numberOfBytes)
	{		

#		//ifdef _DEBUG
		std::string debugString = "Called Function CopyBytes:\n";
		debugString += "\t void* destination:\t";
		debugString += std::to_string((unsigned int)destination);
		debugString += "\n";
		debugString += "\t void* source:\t";
		debugString += std::to_string((unsigned int)source);
		debugString += "\n";
		debugString += "\t int numberOfBytes:\t";
		debugString += std::to_string(numberOfBytes) + "\n";

		Logger::Instance()->Log(debugString);
		//#endif

		DWORD destinationMemoryProtection;
		DWORD sourceMemoryProtection;
		int result = S_OK;

		Logger::Instance()->Log("before calling virtualprotect\n");
		//result += VirtualProtect ( source, numberOfBytes, PAGE_EXECUTE_READ, &sourceMemoryProtection ); 
		debugString = "Changing protection to source: ";
		debugString += std::to_string(result);
		Logger::Instance()->Log(debugString);
		result += VirtualProtect ( destination, numberOfBytes, PAGE_EXECUTE_READWRITE, &destinationMemoryProtection );
		debugString = "Changing protection to destination: ";
		debugString += std::to_string(result);
		Logger::Instance()->Log(debugString);
		memcpy (destination, source, numberOfBytes);	

		Logger::Instance()->Log("bytes copied");
		//result += VirtualProtect ( source, numberOfBytes, sourceMemoryProtection, NULL ); 
		DWORD newMemoryProtection;
		result += VirtualProtect ( destination, numberOfBytes, destinationMemoryProtection, &newMemoryProtection ); 
		//if ( result != S_OK)
			//throw std::exception("Byte copy failed");
		//return S_OK;		
		Logger::Instance()->Log("returning");
		return result;
	}

	
	static void * GetFunctionPointer (IUnknown * comObject, int vtableIndex)
	{
		//vtable black magic, needs simplification
		void ** vtable = * (void ***) comObject;
		return vtable[vtableIndex];
	}



};
