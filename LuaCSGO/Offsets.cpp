#include "Offsets.hpp"
#include "Utils.hpp"

namespace Offsets {
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

		g_dwGlobalVarsBase = *(unsigned long*)(Utils::SigScan( "client.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x5F\x8B\x40\x10", "x????xxxx" ) + 0x1);
			
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