#include "Offsets.hpp"
#include "LuaExports/Utils.hpp"

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

namespace Offsets {
	
	bool Mask( const BYTE* pData, const BYTE* bMask, const char* szMask ) {
		for(; *szMask; ++szMask, ++pData, ++bMask)
			if(*szMask == 'x' && *pData != *bMask)
				return false;
		return ( *szMask ) == NULL;
	}

	DWORD FindPattern( DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char* szMask ) {
		for(DWORD i = 0; i < dwLen; i++)
			if(Mask( (BYTE*)( dwAddress + i ), bMask, szMask ))
				return (DWORD)( dwAddress + i );
		return 0;
	}

	DWORD SigScan( std::string module, BYTE *bSig, const char* szMask ) {
		_MODULEINFO modInfo;
		GetModuleInformation( GetCurrentProcess(), GetModuleHandleA( module.c_str() ), &modInfo, sizeof( _MODULEINFO ) );
		return FindPattern( (DWORD)modInfo.lpBaseOfDll, modInfo.SizeOfImage, bSig, szMask );
	}

	DWORD			g_ClientBase;
	DWORD			g_EngineBase;
	DWORD			g_MatSurface;
	DWORD			g_VGUI2;
	DWORD			g_dwGlobalVarsBase = NULL;

	void GetOffsets() {
		do {
			g_ClientBase	= (DWORD)GetModuleHandleA( "client.dll" );
			g_EngineBase	= (DWORD)GetModuleHandleA( "engine.dll" );
			g_MatSurface	= (DWORD)GetModuleHandleA( "vguimatsurface.dll" );
			g_VGUI2			= (DWORD)GetModuleHandleA( "vgui2.dll" );
			Sleep( 200 );
		} while( !g_ClientBase || !g_EngineBase || !g_MatSurface || !g_VGUI2 );

		g_dwGlobalVarsBase = *(unsigned long*)(SigScan( "client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x5F\x8B\x40\x10", "x????xxxx" ) + 0x1);
			
	}

	void Dump() {
		DebugString(
			"g_ClientBase       = 0x%08X\n"
			"g_EngineBase       = 0x%08X\n"
			"g_MatSurface       = 0x%08X\n"
			"g_VGUI2            = 0x%08X\n"
			"g_dwGlobalVars     = 0x%08X\n",
			g_ClientBase,
			g_EngineBase,
			g_MatSurface,
			g_VGUI2,
			g_dwGlobalVarsBase );
	}
};