#pragma once

#include "CSGO-SDK/SDK.hpp"

namespace LuaCSGO
{
	class GlobalVars {
	public:
		GlobalVars( SDK::CGlobalVarsBase* pGlobals ) {
			m_pGlobals = pGlobals;
		}

		float GetAbsFrameTime() {
			return m_pGlobals->absoluteframetime;
		}

	#ifdef GetCurrentTime
	#undef GetCurrentTime
	#endif

		float GetCurrentTime() {
			return m_pGlobals->curtime;
		}
		int GetFrameCount() {
			return m_pGlobals->framecount;
		}
		float GetFrameTime() {
			return m_pGlobals->frametime;
		}
		float GetInterpolationAmount() {
			return m_pGlobals->interpolation_amount;
		}
		float GetIntervalPerTick() {
			return m_pGlobals->interval_per_tick;
		}
		int GetMaxClients() {
			return m_pGlobals->maxClients;
		}
		bool IsRemoteClient() {
			return m_pGlobals->m_bRemoteClient;
		}
		float GetRealTime() {
			return m_pGlobals->realtime;
		}
		float GetTickCount() {
			return m_pGlobals->tickcount;
		}
	private:
		SDK::CGlobalVarsBase* m_pGlobals;
	};
}
