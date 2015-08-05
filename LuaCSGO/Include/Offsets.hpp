#pragma once

#include <Windows.h>
#include <string>

#ifdef _DEBUG
#define DebugString(...) printf(__VA_ARGS__)
#else
#define DebugString(...) do {} while(false)
#endif

namespace Offsets {
	extern unsigned long			g_ClientBase;
	extern unsigned long			g_EngineBase;
	extern unsigned long			g_MatSurface;
	extern unsigned long			g_VGUI2;
	extern unsigned long			g_dwGlobalVarsBase;

	void Dump();
	void GetOffsets();
};