#pragma once
#include <Windows.h>

class VTableHook {
public:
	VTableHook( PDWORD* ppdwClassBase );

	~VTableHook();

	DWORD HookIndex( DWORD index, DWORD newFunc );
	void UnhookIndex( DWORD index );
	DWORD GetVTableLength();
	DWORD GetOriginalVFunc( DWORD index );
	PDWORD GetOriginalVTable();
	void RestoreVTable();

private:
	DWORD GetVTableLength( LPDWORD ppVTable );

private:
	PDWORD*	_ClassBase;
	PDWORD	_OldVTable, _NewVTable;
	DWORD	_VTableLength, _OldProtect;
};
