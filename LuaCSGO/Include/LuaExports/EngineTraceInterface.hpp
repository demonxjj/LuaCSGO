#pragma once

#include "GameTrace.hpp"

namespace LuaCSGO
{
	class EngineTraceInterface {
	public:
		EngineTraceInterface( SDK::IEngineTrace* engineTrace ) {
			_engineTrace = engineTrace;
		}

		GameTrace TraceRay( SDK::Vector start, SDK::Vector end, BaseClientEntity skipEnt, unsigned int mask) {
			SDK::CGameTrace tr;
			SDK::Ray_t ray;
			SDK::CTraceFilter filter;
			filter.pSkip = ( SDK::CBaseEntity* )skipEnt;
			
			ray.Init( start, end );
			
			_engineTrace->TraceRay( ray, mask, &filter, &tr );
			return GameTrace( tr );
		}
	private:
		SDK::IEngineTrace* _engineTrace;
	};
}
