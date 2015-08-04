#include "VTableHook.hpp"

VTableHook::VTableHook( DWORD** ppClass ) {
	_ClassBase = ppClass;
	_OldVTable = *ppClass;
	_VTableLength = GetVTableLength( *_ClassBase );

	_NewVTable = new DWORD[_VTableLength];
	memcpy( _NewVTable, _OldVTable, sizeof( DWORD ) * _VTableLength );

	VirtualProtect( ppClass, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &_OldProtect );
	*ppClass = _NewVTable;
	VirtualProtect( ppClass, sizeof( DWORD ), _OldProtect, &_OldProtect );
}

VTableHook::~VTableHook() {
	delete[] _NewVTable;
}

DWORD VTableHook::GetVTableLength( LPDWORD ppVTable ) {
	__try {
		DWORD dwIndex = 0;

		for(dwIndex = 0; ppVTable[dwIndex]; dwIndex++) {
			//This sometimes throws access violations for no reason on Windows 10
			//TODO: Find a way to grab the vtable size without using IsBadCodePtr
			if(IsBadCodePtr( (FARPROC)ppVTable[dwIndex] )) {
				break;
			}
		}
		return dwIndex;
	} __except(GetExceptionCode() == STATUS_ACCESS_VIOLATION) {
		//auto pInfo = GetExceptionInformation();
		return 100;
	}
}

DWORD VTableHook::GetVTableLength() {
	return _VTableLength;
}

DWORD VTableHook::HookIndex( DWORD index, DWORD newFunc ) {
	if(index > _VTableLength)
		return NULL;
	_NewVTable[index] = newFunc;
	return _OldVTable[index];
}

void VTableHook::UnhookIndex( DWORD index ) {
	if(index > _VTableLength) return;
	_NewVTable[index] = _OldVTable[index];
}

DWORD VTableHook::GetOriginalVFunc( DWORD index ) {
	if(index > _VTableLength)
		return NULL;

	return _OldVTable[index];
}

PDWORD VTableHook::GetOriginalVTable() {
	return _OldVTable;
}

void VTableHook::RestoreVTable() {
	if(_ClassBase) {
		VirtualProtect( _ClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &_OldProtect );
		*_ClassBase = _OldVTable;
		VirtualProtect( _ClassBase, sizeof( DWORD ), _OldProtect, &_OldProtect );
	}
}