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
	DWORD dwIndex = 0;

	for(dwIndex = 0; ppVTable[dwIndex]; dwIndex++) {
		if(IsBadCodePtr( (FARPROC)ppVTable[dwIndex] )) {
			break;
		}
	}
	return dwIndex;
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