#pragma once

#include "DllMain.h"

#include "EngineInterface.hpp"
#include "EngineTraceInterface.hpp"
#include "EntityListInterface.hpp"
#include "PanelInterface.hpp"
#include "GlobalVars.hpp"
namespace LuaCSGO
{
	class Interfaces {
	public:
		EngineInterface GetEngineInterface() {
			static EngineInterface engine( SDK::Interfaces::g_pEngine );
			return engine;
		}
		EngineTraceInterface GetEngineTraceInterface() {
			static EngineTraceInterface trace( SDK::Interfaces::g_pEngineTrace );
			return trace;
		}

		EntityListInterface GetEntityListInterface() {
			static EntityListInterface entityList( SDK::Interfaces::g_pEntityList );
			return entityList;
		}

		PanelInterface GetPanelInterface() {
			static PanelInterface ipanel( SDK::Interfaces::g_pVGuiPanel );
			return ipanel;
		}

		GlobalVars GetGlobalVars() {
			static GlobalVars globals( SDK::Interfaces::g_pGlobals );
			return globals;
		}
	};
}
