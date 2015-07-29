#include <Windows.h>

#include "LuaState.h"

void AllocDeveloperConsole() {
	if(!AllocConsole()) {
		FreeConsole();
		AllocConsole();
	}

	AttachConsole( GetCurrentProcessId() );
	freopen( "CON", "w", stdout );
}

DWORD WINAPI InitThread( LPVOID ) {
	AllocDeveloperConsole();
	LuaState luaState;
	LuaDebugger dbg( luaState );
	dbg.SetCount( 10 );

	luaState.LoadFile( "C:\\LuaCSGO\\LuaCSGO.lua" );
	luaState.CallFunction( "OnCreateMove", 0, "param1", 2, 3.0f );

	return TRUE;
}

BOOL WINAPI DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved ) {
	switch(dwReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( hMod );
			CreateThread( NULL, NULL, InitThread, NULL, NULL, NULL );
			break;
		case DLL_PROCESS_DETACH:
			FreeConsole();
		default:
			break;
	}
	return TRUE;
}
