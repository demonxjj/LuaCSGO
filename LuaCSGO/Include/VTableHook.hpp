#pragma once
#include <Windows.h>

class VTableHook {
public:
	VTableHook( PDWORD* ppdwClassBase );

	~VTableHook();

	unsigned long HookIndex( unsigned long index, unsigned long newFunc );
	void UnhookIndex( unsigned long index );
	unsigned long GetVTableLength();
	unsigned long GetOriginalVFunc( unsigned long index );
	PDWORD GetOriginalVTable();
	void RestoreVTable();

private:
	unsigned long GetVTableLength( LPDWORD ppVTable );

private:
	PDWORD*	_ClassBase;
	PDWORD	_OldVTable, _NewVTable;
	unsigned long	_VTableLength, _OldProtect;
};
