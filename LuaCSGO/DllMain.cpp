#include "DllMain.h"

typedef void( __thiscall *PaintTraverse_t )( void*, unsigned int, bool, bool );

void __fastcall HookedPaintTraverse( void* _this, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce );

PaintTraverse_t					g_pOriginalPaintTraverse		= nullptr;
std::unique_ptr<LuaState>		g_pLuaState						= nullptr;
std::unique_ptr<LuaDebugger>	g_pLuaDbg						= nullptr;
std::unique_ptr<VTableHook>		g_pPanelHook					= nullptr;
std::unique_ptr<VTableHook>		g_pClientHook					= nullptr;

static SDK::CreateInterfaceFn GetFactory( HMODULE hMod ) {
	return SDK::CreateInterfaceFn( ( SDK::CreateInterfaceFn )GetProcAddress( hMod, CREATEINTERFACE_PROCNAME ) );
}

template<typename T>
static T* CaptureInterface( SDK::CreateInterfaceFn f, const char* szInterfaceVersion ) {
	return (T*)f( szInterfaceVersion, NULL );
}

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

	Offsets::GetOffsets();
	g_pLuaState = std::make_unique<LuaState>();
	g_pLuaDbg = std::make_unique<LuaDebugger>( g_pLuaState.get() );

	g_pLuaState->LoadFile( "C:\\LuaCSGO\\LuaCSGO.lua" );

	SDK::CreateInterfaceFn ClientFactory = GetFactory( (HMODULE)Offsets::g_ClientBase );
	SDK::CreateInterfaceFn EngineFactory = GetFactory( (HMODULE)Offsets::g_EngineBase );
	SDK::CreateInterfaceFn SurfaceFactory = GetFactory( (HMODULE)Offsets::g_MatSurface );
	SDK::CreateInterfaceFn VGui2Factory = GetFactory( (HMODULE)Offsets::g_VGUI2 );

	SDK::Interfaces::g_pClient = CaptureInterface<SDK::IBaseClientDLL>( ClientFactory, INTERFACE_CLIENT_DLL );
	SDK::Interfaces::g_pEngine = CaptureInterface<SDK::IVEngineClient>( EngineFactory, INTERFACE_IVENGINECLIENT );
	SDK::Interfaces::g_pEntityList = CaptureInterface<SDK::IClientEntityList>( ClientFactory, INTERFACE_ENTITY_LIST );
	SDK::Interfaces::g_pVGuiPanel = CaptureInterface<SDK::IPanel>( VGui2Factory, INTERFACE_VGUI_PANEL );
	SDK::Interfaces::g_pVGuiSurface = CaptureInterface<SDK::ISurface>( SurfaceFactory, INTERFACE_VGUI_SURFACE );
	SDK::Interfaces::g_pEngineTrace = CaptureInterface<SDK::IEngineTrace>( EngineFactory, INTERFACE_ENGINETRACE );

	SDK::Interfaces::g_pInput = *reinterpret_cast< SDK::CInput** >( ( *(DWORD**)SDK::Interfaces::g_pClient )[15] + 0x1 );

	//TODO: Find a dynamic way of getting these that doesnt involve sig scanning
	//sig: A1 ? ? ? ? F3 0F 58 40 10
	SDK::Interfaces::g_pGlobals = *( SDK::CGlobalVarsBase** )( Offsets::g_dwGlobalVarsBase );

	g_pPanelHook = std::make_unique<VTableHook>( (PDWORD*)SDK::Interfaces::g_pVGuiPanel );

	g_pOriginalPaintTraverse = (PaintTraverse_t)g_pPanelHook->HookIndex( 41, (DWORD)HookedPaintTraverse );

	if(!g_pOriginalPaintTraverse) {
		DebugString( "Hooking on IPanel::PaintTraverse failed!\n" );
	}

	return TRUE;
}

void __fastcall HookedPaintTraverse( void* _this, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce ) {
	g_pOriginalPaintTraverse( _this, vguiPanel, forceRepaint, allowForce );
	g_pLuaState->CallFunction( "OnPaintTraverse", (double)vguiPanel );
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
