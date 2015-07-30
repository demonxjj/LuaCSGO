#pragma once

#include <Windows.h>
#include <string>

#ifdef _DEBUG
#define DebugString(...) printf(__VA_ARGS__)
#else
#define DebugString(...) do {} while(false)
#endif

namespace Offsets {
	extern DWORD			g_ClientBase;
	extern DWORD			g_EngineBase;
	extern DWORD			g_MatSurface;
	extern DWORD			g_VGUI2;
	extern DWORD			g_dwGlobalVarsBase;

	void Dump();
	void GetOffsets();
};