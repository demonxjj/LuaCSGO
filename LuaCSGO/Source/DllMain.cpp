#include "DllMain.h"

using namespace LuaCSGO;

typedef void( __thiscall *CreateMove_t )( SDK::IBaseClientDLL*, int, float, bool );
typedef void( __thiscall *FrameStageNotify_t )( SDK::IBaseClientDLL*, SDK::ClientFrameStage_t );
typedef void( __thiscall *PaintTraverse_t )( SDK::IPanel*, unsigned int, bool, bool );

CreateMove_t					g_pOriginalCreateMove;
FrameStageNotify_t				g_pOriginalFrameStageNotify;
PaintTraverse_t					g_pOriginalPaintTraverse;
std::unique_ptr<VTableHook>		g_pPanelHook;
std::unique_ptr<VTableHook>		g_pClientHook;

WNDPROC							g_pWindowProc = NULL;
lua_State*						g_pLuaState = NULL;

///--------------------------
/// Lua-exported instances
///--------------------------
Interfaces						g_Interfaces;
DrawManager						g_DrawManager;
Keys							g_Keys;
Callbacks						g_Callbacks;
Utils							g_Utils;

static SDK::CreateInterfaceFn GetFactory( HMODULE hMod ) {
	return SDK::CreateInterfaceFn( ( SDK::CreateInterfaceFn )GetProcAddress( hMod, CREATEINTERFACE_PROCNAME ) );
}

template<typename T>
static T* CaptureInterface( SDK::CreateInterfaceFn f, const char* szInterfaceVersion ) {
	return (T*)f( szInterfaceVersion, NULL );
}

void __fastcall HookedPaintTraverse( SDK::IPanel* _this, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce ) {
	g_pOriginalPaintTraverse( _this, vguiPanel, forceRepaint, allowForce );
	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_PAINTTRAVERSE )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_PAINTTRAVERSE );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it, vguiPanel );
	}
}

void __fastcall HookedFrameStageNotify( SDK::IBaseClientDLL* _this, void*, SDK::ClientFrameStage_t stage ) {

	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_BEGIN )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_BEGIN );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it, (int)stage );
	}

	g_pOriginalFrameStageNotify( _this, stage );

	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_END )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_END );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it, (int)stage );
	}
}

void __fastcall HookedCreateMove( SDK::IBaseClientDLL* _this, void*, int sequence_number, float input_sample_frametime, bool active ) {
	g_pOriginalCreateMove( _this, sequence_number, input_sample_frametime, active );
	if(!SDK::Interfaces::g_pEngine->IsInGame())
		return;

	SDK::CBaseEntity* pLocal = ( SDK::CBaseEntity* )SDK::Interfaces::g_pEntityList->GetClientEntity( SDK::Interfaces::g_pEngine->GetLocalPlayer() );
	if(!pLocal) return;

	SDK::CUserCmd* pCmd = &SDK::Interfaces::g_pInput->m_pCommands[sequence_number % 150];
	SDK::CVerifiedUserCmd* pVerifiedCmd = &SDK::Interfaces::g_pInput->m_pVerifiedCommands[sequence_number % 150];

	if(pCmd && pVerifiedCmd) {
		if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_CREATEMOVE )) {
			auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_CREATEMOVE );
			for(auto it = v.begin(); it != v.end(); it++)
				luabind::call_function<void>( *it, UserCMD( pCmd ), VerifiedCMD( pVerifiedCmd ) );
		}
	}
}

void AllocDeveloperConsole() {
	if(!AllocConsole()) {
		FreeConsole();
		AllocConsole();
	}

	AttachConsole( GetCurrentProcessId() );
	freopen( "CON", "w", stdout );
}

bool InitializeInterfaces() {
	Offsets::GetOffsets();

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

	if(!SDK::Interfaces::g_pClient || !SDK::Interfaces::g_pEngine || !SDK::Interfaces::g_pEngineTrace || !SDK::Interfaces::g_pEntityList || !SDK::Interfaces::g_pVGuiPanel || !SDK::Interfaces::g_pVGuiSurface) {
		printf( "Failed to locate the interfaces!" );
		return false;
	}

	//TODO: 
	//	- How would we check these for validity? 
	//		A simple nullptr check wouldnt work because this will most likely return something that is not null, even when it fails.
	//	- Find a dynamic way of getting GlobalVars that doesnt involve sig scanning.
	//		Sig: A1 ? ? ? ? F3 0F 58 40 10
	SDK::Interfaces::g_pInput = *reinterpret_cast< SDK::CInput** >( ( *(DWORD**)SDK::Interfaces::g_pClient )[15] + 0x1 );
	SDK::Interfaces::g_pGlobals = *( SDK::CGlobalVarsBase** )( Offsets::g_dwGlobalVarsBase );

	return true;
}

void SetupHooks() {
	g_pPanelHook = std::make_unique<VTableHook>( (PDWORD*)SDK::Interfaces::g_pVGuiPanel );
	g_pClientHook = std::make_unique<VTableHook>( (PDWORD*)SDK::Interfaces::g_pClient );

	g_pOriginalCreateMove = (CreateMove_t)g_pClientHook->HookIndex( 21, (DWORD)HookedCreateMove );
	g_pOriginalFrameStageNotify = (FrameStageNotify_t)g_pClientHook->HookIndex( 36, (DWORD)HookedFrameStageNotify );
	g_pOriginalPaintTraverse = (PaintTraverse_t)g_pPanelHook->HookIndex( 41, (DWORD)HookedPaintTraverse );
}

void LoadScripts() {

	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_SCRIPTUNLOAD )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_SCRIPTUNLOAD );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it, 1 );
	}

	if(luaL_dofile( g_pLuaState, "LuaCSGO\\LuaCSGO.lua" ) != 0) {
		printf( "Error: %s\n", lua_tostring( g_pLuaState, -1 ) );
		return;
	}

	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_SCRIPTLOAD )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_SCRIPTLOAD );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it, 1 );
	}
}

HRESULT __stdcall NewWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	try {
		bool result = false;
		switch(uMsg) {
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{

			#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
			#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

				ButtonCodes button = LUACSGO_BUTTON_NONE;
				Events type = LUACSGO_EVENT_UNKNOWN;
				int x = GET_X_LPARAM( lParam );
				int y = GET_Y_LPARAM( lParam );
				WORD xbutton;
				switch(uMsg) {
					case WM_MOUSEMOVE:
						type = LUACSGO_EVENT_MOUSEMOVE;
						break;
					case WM_LBUTTONDOWN:
						type = LUACSGO_EVENT_MOUSEDOWN;
						button = LUACSGO_BUTTON_LEFT;
						break;
					case WM_LBUTTONUP:
						type = LUACSGO_EVENT_MOUSEUP;
						button = LUACSGO_BUTTON_LEFT;
						break;
					case WM_MBUTTONDOWN:
						type = LUACSGO_EVENT_MOUSEDOWN;
						button = LUACSGO_BUTTON_MIDDLE;
						break;
					case WM_MBUTTONUP:
						type = LUACSGO_EVENT_MOUSEUP;
						button = LUACSGO_BUTTON_MIDDLE;
						break;
					case WM_RBUTTONDOWN:
						type = LUACSGO_EVENT_MOUSEDOWN;
						button = LUACSGO_BUTTON_RIGHT;
						break;
					case WM_RBUTTONUP:
						type = LUACSGO_EVENT_MOUSEUP;
						button = LUACSGO_BUTTON_RIGHT;
						break;
					case WM_XBUTTONDOWN:
						type = LUACSGO_EVENT_MOUSEDOWN;
						xbutton = GET_XBUTTON_WPARAM( wParam );
						if(xbutton == 1)
							button = LUACSGO_BUTTON_MOUSE4;
						else
							button = LUACSGO_BUTTON_MOUSE5;
					case WM_XBUTTONUP:
						type = LUACSGO_EVENT_MOUSEUP;
						xbutton = GET_XBUTTON_WPARAM( wParam );
						if(xbutton == 1)
							button = LUACSGO_BUTTON_MOUSE4;
						else
							button = LUACSGO_BUTTON_MOUSE5;
				}
				if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_MOUSE )) {
					auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_MOUSE );
					result = luabind::call_function<bool>( v.front(), MouseEvent( type, button, x, y ) );
				}
			}
			break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Events type = LUACSGO_EVENT_UNKNOWN;
				KeyCodes key = LUACSGO_KEY_NONE;

				switch(uMsg) {
					case WM_KEYDOWN:
					case WM_SYSKEYDOWN:
						type = LUACSGO_EVENT_KEYDOWN;
						break;
					case WM_KEYUP:
					case WM_SYSKEYUP:
						type = LUACSGO_EVENT_KEYUP;
						break;
				}
				if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_KEYBOARD )) {
					auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_KEYBOARD );
					result = luabind::call_function<bool>( v.front(), KeyboardEvent( type, wParam ) );
				}
			}
			break;
			default:
				break;
		}

		if(result)
			return TRUE;
		else
			return CallWindowProc( g_pWindowProc, hwnd, uMsg, wParam, lParam );
	} catch(const std::exception&) {
		//TODO: Should log the exception here
		return CallWindowProc( g_pWindowProc, hwnd, uMsg, wParam, lParam );
	}

}

void InitializeLua();

void Reload_() {
	g_Callbacks.Reset();
	Sleep( 1000 );
	InitializeLua();
}

void Reload() {
	CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)Reload_, NULL, NULL, NULL );
}

void InitializeLua() {
	if(g_pLuaState != NULL) {
		lua_close( g_pLuaState );
		g_pLuaState = NULL;
	}

	g_pLuaState = luaL_newstate();

	luabind::open( g_pLuaState );
	luaL_openlibs( g_pLuaState );

	luabind::module( g_pLuaState )[
		luabind::class_<Interfaces>( "__Interfaces" )
			.def( "GetEngineInterface", &Interfaces::GetEngineInterface )
			.def( "GetEngineTraceInterface", &Interfaces::GetEngineTraceInterface )
			.def( "GetEntityListInterface", &Interfaces::GetEntityListInterface )
			.def( "GetPanelInterface", &Interfaces::GetPanelInterface )
			.def( "GetGlobalVars", &Interfaces::GetGlobalVars ),
		luabind::class_<DrawManager>( "__DrawManager" )
			.def( "CreateNewFont", &DrawManager::CreateNewFont )
			.def( "DrawLine", &DrawManager::DrawLine )
			.def( "DrawFilledCircle", &DrawManager::DrawFilledCircle )
			.def( "DrawFilledRect", &DrawManager::DrawFilledRect )
			.def( "DrawOutlinedCircle", &DrawManager::DrawOutlinedCircle )
			.def( "DrawOutlinedRect", &DrawManager::DrawOutlinedRect )
			.def( "DrawCircle", &DrawManager::DrawCircle )
			.def( "DrawRect", &DrawManager::DrawRect )
			.def( "DrawText", &DrawManager::DrawText )
			.def( "FillGradient", &DrawManager::FillGradient )
			.def( "GetDrawColor", &DrawManager::GetDrawColor )
			.def( "SetDrawColor", &DrawManager::SetDrawColor )
			.def( "MeasureText", &DrawManager::MeasureText ),
		luabind::class_<Callbacks>( "__Callbacks" )
			.def( "Register", &Callbacks::RegisterCallback )
			.def( "DisableAllExcept", &Callbacks::DisableAllExcept )
			.def( "EnableAllCallbacks", &Callbacks::EnableAllCallbacks ),
		luabind::class_<Utils>( "__Utils" )
			.def( "GetEntityBone", &Utils::GetEntityBone )
			.def( "WorldToScreen", &Utils::WorldToScreen )
			.def( "GetLocalPlayer", &Utils::GetLocalPlayer ),
		luabind::class_<Keys>( "__Keys" )
			.def( "LookupKeyString", &Keys::LookupKeyString ),
		luabind::class_<SDK::Vector2D>( "Size" )
			.def( luabind::constructor<>() )
			.def( luabind::constructor<float, float>() )
			.def( luabind::const_self + luabind::const_self )
			.def( luabind::const_self - luabind::const_self )
			.def( luabind::const_self * luabind::const_self )
			.def( luabind::const_self / luabind::const_self )
			.def( luabind::const_self * luabind::other<float>() )
			.def( luabind::const_self / luabind::other<float>() )
			.def( "IsValid", &SDK::Vector2D::IsValid )
			.property( "Width", &SDK::Vector2D::x, &SDK::Vector2D::x )
			.property( "Height", &SDK::Vector2D::y, &SDK::Vector2D::y ),
		luabind::class_<SDK::Vector>( "Vector" )
			.def( luabind::constructor<>() )
			.def( luabind::constructor<float, float, float>() )
			.def( luabind::const_self + luabind::const_self )
			.def( luabind::const_self - luabind::const_self )
			.def( luabind::const_self * luabind::const_self )
			.def( luabind::const_self / luabind::const_self )
			.def( luabind::const_self * luabind::other<float>() )
			.def( luabind::const_self / luabind::other<float>() )
			.property( "X", &SDK::Vector::x, &SDK::Vector::x )
			.property( "Y", &SDK::Vector::y, &SDK::Vector::y )
			.property( "Z", &SDK::Vector::z, &SDK::Vector::z )
			.def( "Dot", &SDK::Vector::Dot )
			.def( "IsZero", &SDK::Vector::IsZero )
			.def( "IsValid", &SDK::Vector::IsValid )
			.def( "Invalidate", &SDK::Vector::Invalidate )
			.def( "Length", &SDK::Vector::Length )
			.def( "Normalized", &SDK::Vector::Normalized )
			.def( "Zero", &SDK::Vector::Zero ),
		luabind::class_<BaseClientEntity>( "BaseClientEntity" )
			.def( "IsValid", &BaseClientEntity::IsValid )
			.def( "GetOrigin", &BaseClientEntity::GetOrigin )
			.def( "GetClassID", &BaseClientEntity::GetClassID )
			.def( "GetClassName", &BaseClientEntity::GetClassName )
			.def( "GetHealth", &BaseClientEntity::GetHealth )
			.def( "GetFlags", &BaseClientEntity::GetFlags )
			.def( "GetEyePos", &BaseClientEntity::GetEyePos )
			.def( "IsDormant", &BaseClientEntity::IsDormant )
			.def( "IsAlive", &BaseClientEntity::IsAlive )
			.def( "GetTeam", &BaseClientEntity::GetTeam )
			.def( "GetAimPunch", &BaseClientEntity::GetAimPunch )
			.def( "GetViewPunch", &BaseClientEntity::GetViewPunch )
			.def( "SetAimPunch", &BaseClientEntity::SetAimPunch )
			.def( "SetViewPunch", &BaseClientEntity::SetViewPunch ),
		luabind::class_<EngineInterface>( "EngineInterface" )
			.def( "ExecuteCommand", &EngineInterface::ExecuteCommand )
			.def( "GetClientName", &EngineInterface::GetClientName )
			.def( "GetLevelName", &EngineInterface::GetLevelName )
			.def( "GetLocalPlayer", &EngineInterface::GetLocalPlayer )
			.def( "GetMaxClients", &EngineInterface::GetMaxClients )
			.def( "GetScreenSize", &EngineInterface::GetScreenSize )
			.def( "GetViewAngles", &EngineInterface::GetViewAngles )
			.def( "IsConnected", &EngineInterface::IsConnected )
			.def( "IsInGame", &EngineInterface::IsInGame )
			.def( "IsPlayingDemo", &EngineInterface::IsPlayingDemo )
			.def( "IsRecordingDemo", &EngineInterface::IsRecordingDemo )
			.def( "IsTakingScreenshot", &EngineInterface::IsTakingScreenshot )
			.def( "SetViewAngles", &EngineInterface::SetViewAngles ),
		luabind::class_<GameTrace>( "GameTrace" )
			.def( "DidHit", &GameTrace::DidHit )
			.def( "DidHitEntity", &GameTrace::DidHitEntity )
			.def( "GetHitbox", &GameTrace::GetHitbox )
			.def( "IsVisible", &GameTrace::IsVisible )
			.def( "GetEndPos", &GameTrace::GetEndPos ),
		luabind::class_<EngineTraceInterface>( "EngineTraceInterface" )
			.def( "TraceRay", &EngineTraceInterface::TraceRay ),
		luabind::class_<EntityListInterface>( "EntityListInterface" )
			.def( "GetEntityFromIndex", &EntityListInterface::GetEntityFromIndex )
			.def( "GetHighestEntityIndex", &EntityListInterface::GetHighestEntityIndex ),
		luabind::class_<GlobalVars>( "GlobalVars" )
			.def( "GetAbsoluteFrameTime", &GlobalVars::GetAbsFrameTime )
			.def( "GetFrameCount", &GlobalVars::GetFrameCount )
			.def( "GetFrameTime", &GlobalVars::GetFrameTime )
			.def( "GetInterpolationAmount", &GlobalVars::GetInterpolationAmount )
			.def( "GetIntervalPerTick", &GlobalVars::GetIntervalPerTick )
			.def( "GetMaxClients", &GlobalVars::GetMaxClients )
			.def( "GetRealTime", &GlobalVars::GetRealTime )
			.def( "GetTickCount", &GlobalVars::GetTickCount )
			.def( "IsRemoteClient", &GlobalVars::IsRemoteClient ),
		luabind::class_<PanelInterface>( "PanelInterface" )
			.def( "GetName", &PanelInterface::GetName ),
		luabind::class_<MouseEvent>( "MouseEvent" )
			.property( "X", &MouseEvent::GetX )
			.property( "Y", &MouseEvent::GetY )
			.property( "Type", &MouseEvent::GetType )
			.property( "Button", &MouseEvent::GetButton ),
		luabind::class_<KeyboardEvent>( "KeyboardEvent" )
			.property( "Type", &KeyboardEvent::GetType )
			.property( "KeyCode", &KeyboardEvent::GetKeyCode ),
			luabind::class_<UserCMD>( "UserCMD" )
			.property( "Buttons", &UserCMD::GetButtons, &UserCMD::SetButtons )
			.property( "ViewAngles", &UserCMD::GetViewAngles, &UserCMD::SetViewAngles )
			.property( "ForwardMove", &UserCMD::GetForwardMove, &UserCMD::SetForwardMove )
			.property( "SideMove", &UserCMD::GetSideMove, &UserCMD::SetSideMove )
			.property( "UpMove", &UserCMD::GetUpMove, &UserCMD::SetUpMove )
			.def( "GetChecksum", &UserCMD::GetChecksum ),
		luabind::class_<VerifiedCMD>( "VerifiedCMD" )
			.def( "Update", &VerifiedCMD::Update ),
		luabind::def( "RELOAD", &Reload )
	];

	//Export the keys on its own file so as to not spam this file with over 150 lines
	g_Keys.Export( g_pLuaState );

	luabind::globals( g_pLuaState )["CALLBACK_SCRIPTLOAD"] = CallbackType::LUACSGO_CALLBACK_SCRIPTLOAD;
	luabind::globals( g_pLuaState )["CALLBACK_SCRIPTUNLOAD"] = CallbackType::LUACSGO_CALLBACK_SCRIPTUNLOAD;
	luabind::globals( g_pLuaState )["CALLBACK_MOUSE"] = CallbackType::LUACSGO_CALLBACK_MOUSE;
	luabind::globals( g_pLuaState )["CALLBACK_KEYBOARD"] = CallbackType::LUACSGO_CALLBACK_KEYBOARD;
	luabind::globals( g_pLuaState )["CALLBACK_PAINTTRAVERSE"] = CallbackType::LUACSGO_CALLBACK_PAINTTRAVERSE;
	luabind::globals( g_pLuaState )["CALLBACK_CREATEMOVE"] = CallbackType::LUACSGO_CALLBACK_CREATEMOVE;
	luabind::globals( g_pLuaState )["CALLBACK_FRAMESTAGENOTIFY_BEGIN"] = CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_BEGIN;
	luabind::globals( g_pLuaState )["CALLBACK_FRAMESTAGENOTIFY_END"] = CallbackType::LUACSGO_CALLBACK_FRAMESTAGENOTIFY_END;

	luabind::globals( g_pLuaState )["FRAME_UNDEFINED"] = SDK::ClientFrameStage_t::FRAME_UNDEFINED;
	luabind::globals( g_pLuaState )["FRAME_START"] = SDK::ClientFrameStage_t::FRAME_START;
	luabind::globals( g_pLuaState )["FRAME_NET_UPDATE_START"] = SDK::ClientFrameStage_t::FRAME_NET_UPDATE_START;
	luabind::globals( g_pLuaState )["FRAME_NET_UPDATE_POSTDATAUPDATE_START"] = SDK::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START;
	luabind::globals( g_pLuaState )["FRAME_NET_UPDATE_POSTDATAUPDATE_END"] = SDK::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END;
	luabind::globals( g_pLuaState )["FRAME_NET_UPDATE_END"] = SDK::ClientFrameStage_t::FRAME_NET_UPDATE_END;
	luabind::globals( g_pLuaState )["FRAME_RENDER_START"] = SDK::ClientFrameStage_t::FRAME_RENDER_START;
	luabind::globals( g_pLuaState )["FRAME_RENDER_END"] = SDK::ClientFrameStage_t::FRAME_RENDER_END;

	luabind::globals( g_pLuaState )["EVENT_UNKNOWN"] = Events::LUACSGO_EVENT_UNKNOWN;
	luabind::globals( g_pLuaState )["EVENT_KEYDOWN"] = Events::LUACSGO_EVENT_KEYDOWN;
	luabind::globals( g_pLuaState )["EVENT_KEYUP"] = Events::LUACSGO_EVENT_KEYUP;
	luabind::globals( g_pLuaState )["EVENT_MOUSEDOWN"] = Events::LUACSGO_EVENT_MOUSEDOWN;
	luabind::globals( g_pLuaState )["EVENT_MOUSEUP"] = Events::LUACSGO_EVENT_MOUSEUP;
	luabind::globals( g_pLuaState )["EVENT_MOUSEMOVE"] = Events::LUACSGO_EVENT_MOUSEMOVE;

	luabind::globals( g_pLuaState )["MASK_ALL"] = MASK_ALL;
	luabind::globals( g_pLuaState )["MASK_SOLID"] = MASK_SOLID;
	luabind::globals( g_pLuaState )["MASK_SHOT"] = MASK_SHOT;
	luabind::globals( g_pLuaState )["MASK_VISIBLE"] = MASK_VISIBLE;
	luabind::globals( g_pLuaState )["MASK_SHOT_PORTAL"] = MASK_SHOT_PORTAL;
	luabind::globals( g_pLuaState )["MASK_SHOT_BRUSHONLY"] = MASK_SHOT_BRUSHONLY;

	luabind::globals( g_pLuaState )["FL_ONGROUND"] = SDK::EntityFlags::FL_ONGROUND;
	luabind::globals( g_pLuaState )["FL_DUCKING"] = SDK::EntityFlags::FL_DUCKING;
	luabind::globals( g_pLuaState )["FL_WATERJUMP"] = SDK::EntityFlags::FL_WATERJUMP;
	luabind::globals( g_pLuaState )["FL_ONTRAIN"] = SDK::EntityFlags::FL_ONTRAIN;
	luabind::globals( g_pLuaState )["FL_INRAIN"] = SDK::EntityFlags::FL_INRAIN;
	luabind::globals( g_pLuaState )["FL_FROZEN"] = SDK::EntityFlags::FL_FROZEN;
	luabind::globals( g_pLuaState )["FL_ATCONTROLS"] = SDK::EntityFlags::FL_ATCONTROLS;
	luabind::globals( g_pLuaState )["FL_CLIENT"] = SDK::EntityFlags::FL_CLIENT;
	luabind::globals( g_pLuaState )["FL_FAKECLIENT"] = SDK::EntityFlags::FL_FAKECLIENT;

	luabind::globals( g_pLuaState )["Interfaces"] = &g_Interfaces;
	luabind::globals( g_pLuaState )["DrawManager"] = &g_DrawManager;
	luabind::globals( g_pLuaState )["Keys"] = &g_Keys;
	luabind::globals( g_pLuaState )["Utils"] = &g_Utils;
	luabind::globals( g_pLuaState )["Callbacks"] = &g_Callbacks;

	if(luaL_dofile( g_pLuaState, "LuaCSGO\\LuaCSGO.lua" ) != 0) {
		printf( "Error: %s\n", lua_tostring( g_pLuaState, -1 ) );
		return;
	}

	if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_SCRIPTLOAD )) {
		auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_SCRIPTLOAD );
		for(auto it = v.begin(); it != v.end(); it++)
			luabind::call_function<void>( *it );
	}
}

DWORD WINAPI InitThread( LPVOID ) {
	AllocDeveloperConsole();

	if(InitializeInterfaces())
		SetupHooks();
	else return FALSE;

	if(g_pWindowProc == NULL) {
		HWND hWindow = NULL;
		while(!( hWindow = FindWindowA( "Valve001", "Counter-Strike: Global Offensive" ) )) Sleep( 200 );
		g_pWindowProc = (WNDPROC)SetWindowLongPtr( hWindow, GWL_WNDPROC, (LONG_PTR)NewWndProc );
	}

	InitializeLua();

	return TRUE;
}

BOOL WINAPI DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved ) {
	switch(dwReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( hMod );
			CreateThread( NULL, NULL, InitThread, NULL, NULL, NULL );
			break;
		case DLL_PROCESS_DETACH:
			if(g_Callbacks.HasCallback( CallbackType::LUACSGO_CALLBACK_SCRIPTUNLOAD )) {
				auto v = g_Callbacks.GetCallbacks( CallbackType::LUACSGO_CALLBACK_SCRIPTUNLOAD );
				for(auto it = v.begin(); it != v.end(); it++)
					luabind::call_function<void>( *it );
			}
			FreeConsole();
			lua_close( g_pLuaState );
		default:
			break;
	}
	return TRUE;
}
