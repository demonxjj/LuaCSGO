#include <Windows.h>

#include "LuaState.h"

DWORD WINAPI InitThread( LPVOID ) {
	LuaState luaState;
	LuaDebugger dbg( luaState );
	dbg.SetCount( 10 );

	luaState.LoadFile( "test.lua" );
	return TRUE;
}

int main( int argc, char** argv ) {
	InitThread( NULL );
	system( "pause" );
	return 0;
}

/*
BOOL WINAPI DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved ) {
	switch(dwReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( hMod );
			CreateThread( NULL, NULL, InitThread, NULL, NULL, NULL );
			break;
		case DLL_PROCESS_DETACH:
		default:
			break;
	}
	return TRUE;
}
*/