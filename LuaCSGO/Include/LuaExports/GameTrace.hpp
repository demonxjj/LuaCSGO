#pragma once

#include "BaseClientEntity.hpp"

namespace LuaCSGO
{
	class GameTrace {
	public:
		GameTrace( SDK::CGameTrace trace ) {
			_trace = trace;
		}

		bool DidHit() {
			return _trace.DidHit();
		}

		bool IsVisible() {
			return _trace.fraction > 0.97f;
		}

		bool DidHitEntity( BaseClientEntity pEnt ) {
			return _trace.m_pEnt == ( (SDK::CBaseEntity*)pEnt );
		}

		int GetHitbox() {
			return _trace.hitgroup;
		}

		SDK::Vector GetEndPos() {
			return _trace.endpos;
		}

	private:
		SDK::CGameTrace _trace;
	};
}
