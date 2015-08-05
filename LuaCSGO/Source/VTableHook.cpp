#include "VTableHook.hpp"

VTableHook::VTableHook( unsigned long** ppClass ) {
	_ClassBase = ppClass;
	_OldVTable = *ppClass;
	_VTableLength = GetVTableLength( *_ClassBase );

	_NewVTable = new unsigned long[_VTableLength];
	memcpy( _NewVTable, _OldVTable, sizeof( unsigned long ) * _VTableLength );

	VirtualProtect( ppClass, sizeof( unsigned long ), PAGE_EXECUTE_READWRITE, &_OldProtect );
	*ppClass = _NewVTable;
	VirtualProtect( ppClass, sizeof( unsigned long ), _OldProtect, &_OldProtect );
}

VTableHook::~VTableHook() {
	delete[] _NewVTable;
}

unsigned long VTableHook::GetVTableLength( LPDWORD ppVTable ) {
	__try {
		unsigned long dwIndex = 0;

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

unsigned long VTableHook::GetVTableLength() {
	return _VTableLength;
}

unsigned long VTableHook::HookIndex( unsigned long index, unsigned long newFunc ) {
	if(index > _VTableLength)
		return NULL;
	_NewVTable[index] = newFunc;
	return _OldVTable[index];
}

void VTableHook::UnhookIndex( unsigned long index ) {
	if(index > _VTableLength) return;
	_NewVTable[index] = _OldVTable[index];
}

unsigned long VTableHook::GetOriginalVFunc( unsigned long index ) {
	if(index > _VTableLength)
		return NULL;

	return _OldVTable[index];
}

PDWORD VTableHook::GetOriginalVTable() {
	return _OldVTable;
}

void VTableHook::RestoreVTable() {
	if(_ClassBase) {
		VirtualProtect( _ClassBase, sizeof( unsigned long ), PAGE_EXECUTE_READWRITE, &_OldProtect );
		*_ClassBase = _OldVTable;
		VirtualProtect( _ClassBase, sizeof( unsigned long ), _OldProtect, &_OldProtect );
	}
}