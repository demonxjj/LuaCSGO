#include "SDK.hpp"

namespace SDK {
	namespace Interfaces {
		IVEngineClient* g_pEngine = nullptr;
		IBaseClientDLL* g_pClient = nullptr;
		IClientEntityList* g_pEntityList = nullptr;
		CGlobalVarsBase* g_pGlobals = nullptr;
		IPanel *g_pVGuiPanel = nullptr;
		ISurface *g_pVGuiSurface = nullptr;
		CInput *g_pInput = nullptr;
		IEngineTrace *g_pEngineTrace = nullptr;
	}
}