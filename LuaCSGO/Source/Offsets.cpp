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

	unsigned long FindPattern( unsigned long dwAddress, unsigned long dwLen, BYTE *bMask, const char* szMask ) {
		for(unsigned long i = 0; i < dwLen; i++)
			if(Mask( (BYTE*)( dwAddress + i ), bMask, szMask ))
				return (unsigned long)( dwAddress + i );
		return 0;
	}

	unsigned long SigScan( std::string module, BYTE *bSig, const char* szMask ) {
		_MODULEINFO modInfo;
		GetModuleInformation( GetCurrentProcess(), GetModuleHandleA( module.c_str() ), &modInfo, sizeof( _MODULEINFO ) );
		return FindPattern( (unsigned long)modInfo.lpBaseOfDll, modInfo.SizeOfImage, bSig, szMask );
	}

	unsigned long			g_ClientBase;
	unsigned long			g_EngineBase;
	unsigned long			g_MatSurface;
	unsigned long			g_VGUI2;
	unsigned long			g_dwGlobalVarsBase = NULL;

	void GetOffsets() {
		do {
			g_ClientBase	= (unsigned long)GetModuleHandleA( "client.dll" );
			g_EngineBase	= (unsigned long)GetModuleHandleA( "engine.dll" );
			g_MatSurface	= (unsigned long)GetModuleHandleA( "vguimatsurface.dll" );
			g_VGUI2			= (unsigned long)GetModuleHandleA( "vgui2.dll" );
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