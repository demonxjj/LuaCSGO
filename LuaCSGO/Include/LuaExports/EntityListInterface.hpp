#pragma once
#include "CSGO-SDK/SDK.hpp"

#include "BaseClientEntity.hpp"

namespace LuaCSGO
{
	class EntityListInterface {
	public:
		EntityListInterface( SDK::IClientEntityList* pEntityList ) {
			m_pEntityList = pEntityList;
		}

		BaseClientEntity GetEntityFromIndex( int index ) {
			return BaseClientEntity( ( SDK::CBaseEntity* )m_pEntityList->GetClientEntity( index ) );
		}

		int GetHighestEntityIndex() {
			return m_pEntityList->GetHighestEntityIndex();
		}

	private:
		SDK::IClientEntityList* m_pEntityList;
	};
}