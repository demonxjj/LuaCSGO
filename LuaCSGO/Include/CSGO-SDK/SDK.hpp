#pragma once
#pragma warning(disable: 4244)
#include "Definitions.hpp"

#include "CRC.hpp"
#include "Vector.hpp"
#include "Vector2D.hpp"
#include "Vector4D.hpp"
#include "QAngle.hpp"
#include "CUserCMD.hpp"
#include "CHandle.hpp"
#include "CBaseCombatWeapon.hpp"
#include "AudioState.hpp"
#include "CBaseEntity.hpp"
#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"
#include "Color.hpp"
#include "IBaseClientDll.hpp"
#include "IClientEntity.hpp"
#include "IClientEntityList.hpp"
#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientUnknown.hpp"
#include "IPanel.hpp"
#include "ISurface.hpp"
#include "IVEngineClient.hpp"
#include "IEngineTrace.hpp"
#include "PlayerInfo.hpp"
#include "Recv.hpp"
#include "String_t.hpp"
#include "VMatrix.hpp"
#include "IClientMode.hpp"
#include "CInput.hpp"

namespace SDK {
	namespace Interfaces {
		extern IVEngineClient*		g_pEngine;
		extern IBaseClientDLL*		g_pClient;
		extern IClientEntityList*	g_pEntityList;
		extern CGlobalVarsBase*		g_pGlobals;
		extern IPanel*				g_pVGuiPanel;
		extern ISurface*			g_pVGuiSurface;
		extern CInput*				g_pInput;
		extern IEngineTrace*		g_pEngineTrace;
	}
}