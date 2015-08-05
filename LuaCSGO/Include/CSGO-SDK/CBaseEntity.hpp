#pragma once

#include "Definitions.hpp"

#include "IClientEntity.hpp"
#include "QAngle.hpp"

namespace SDK {
	class CMouthInfo;

	class CBaseEntity : public IClientEntity {

	public:
		const float GetServerTime() const {
			return *reinterpret_cast<float*>((DWORD)this + (DWORD)0x000017CC);
		}
		const bool IsDormant() const {
			return *reinterpret_cast<bool*>((DWORD)this + (DWORD)0x000000E9);
		}
		const int GetTeamNum() const {
			return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x000000F0);
		}
		const int GetHealth() const {
			return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x000000FC);
		}
		const int GetFlags() const {
			return *reinterpret_cast<int*>((DWORD)this + (DWORD)0x00000100);
		}
		const Vector GetViewOffset() const {
			return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x00000104);
		}
		const Vector GetVelocity() const {
			return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x00000110);
		}
		const CHandle<IClientEntity> GetActiveWeapon() const {
			return *reinterpret_cast<CHandle<IClientEntity>*>((DWORD)this + (DWORD)0x000012C0);
		}
		const Vector GetOrigin() const {
			return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x00000134);
		}
		const BYTE GetLifeState() const {
			return *reinterpret_cast<BYTE*>((DWORD)this + (DWORD)0x0000025B);
		}
		const bool GetSpotted() const {
			return *reinterpret_cast<bool*>( ( DWORD )this + (DWORD)0x00000935 );
		}
		void SetSpotted( bool b ) {
			*reinterpret_cast<bool*>( ( DWORD )this + (DWORD)0x00000935 ) = b;
		}
		const QAngle GetLocalViewPunch() const {
			return *reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x000013DC);
		}
		const QAngle GetLocalAimPunch() const {
			return *reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x000013E8);
		}
		const float GetFlashAlpha() const {
			return *reinterpret_cast<float*>((DWORD)this + (DWORD)0x00001DB0);
		}
		const Vector GetRotation() const {
			return *reinterpret_cast<Vector*>((DWORD)this + (DWORD)0x000023B4);
		}
		const Vector GetEyePos() const {
			return GetOrigin() + GetViewOffset();
		}
		const bool IsAlive() const {
			return GetLifeState() == LIFE_ALIVE;
		}

		void SetLocalViewPunch( const QAngle& newAngle ) {
			*reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x000013DC) = newAngle;
		}
		void SetLocalAimPunch( const QAngle& newAngle ) {
			*reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x000013E8) = newAngle;
		}
	};
}