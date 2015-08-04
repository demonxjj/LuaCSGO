#pragma once

#include "CSGO-SDK/SDK.hpp"

namespace LuaCSGO
{
	class BaseClientEntity {
	public:
		BaseClientEntity( SDK::CBaseEntity* pBaseEntity ) {
			_pBaseEntity = pBaseEntity;
		}

		bool IsValid() {
			return _pBaseEntity != nullptr;
		}
		
		SDK::Vector GetOrigin() {
			return _pBaseEntity->GetOrigin();
		}
		
		int GetClassID() {
			return _pBaseEntity->GetClientClass()->m_ClassID;
		}
		
		const char* GetClassName() {
			return _pBaseEntity->GetClientClass()->GetName();
		}
		
		int GetHealth() {
			return _pBaseEntity->GetHealth();
		}
		
		int GetFlags() {
			return _pBaseEntity->GetFlags();
		}
		
		SDK::Vector GetEyePos() {
			return _pBaseEntity->GetEyePos();
		}
		
		bool IsDormant() {
			return _pBaseEntity->IsDormant();
		}
		
		bool IsAlive() {
			return _pBaseEntity->IsAlive();
		}

		int GetTeam() {
			return _pBaseEntity->GetTeamNum();
		}

		SDK::Vector GetAimPunch() {
			return _pBaseEntity->GetLocalAimPunch();
		}
		SDK::Vector GetViewPunch() {
			return _pBaseEntity->GetLocalViewPunch();
		}
		void SetAimPunch( SDK::Vector v ) {
			_pBaseEntity->SetLocalAimPunch( v );
		}
		void SetViewPunch( SDK::Vector v ) {
			_pBaseEntity->SetLocalViewPunch( v );
		}
		//cast operator
		operator SDK::CBaseEntity*( ) {
			return _pBaseEntity;
		}
	private:
		SDK::CBaseEntity* _pBaseEntity;
	};
}