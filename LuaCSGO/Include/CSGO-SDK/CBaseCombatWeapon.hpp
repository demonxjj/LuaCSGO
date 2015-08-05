#pragma once

#include "Definitions.hpp"

namespace SDK {
	
	class WeaponData_t {
	public:
		char pad_0x0000[0x4]; //0x0000
		unsigned char bParsedScript; //0x0004 
		unsigned char bLoadedHudElements; //0x0005 
		char szClassName[80]; //0x0006 
		char szDisplayName[80]; //0x0056 
		char szViewModel[80]; //0x00A6 
		char szWorldModel[80]; //0x00F6 
		char szAnimationPrefix[16]; //0x0146 
		char pad_0x0156[0x2A]; //0x0156
		__int32 iMaxClip; //0x0180 
		char pad_0x0184[0x14]; //0x0184
		unsigned char bAutoSwitchTo; //0x0198 
		unsigned char bAutoSwitchFrom; //0x0199 
		char pad_0x019A[0x5B6]; //0x019A
		__int32 iWeaponID; //0x0750 
		char pad_0x0754[0x30]; //0x0754
		unsigned char bIsFullAlto; //0x0784 
		char pad_0x0785[0x4B7]; //0x0785

	};//Size=0x0C3C

	class CBaseCombatWeapon {
	public:
		const int GetWeaponID() const {
			return *reinterpret_cast<int*>((unsigned long)this + (unsigned long)0x00001690);
		}

		const int NextPrimaryAttack() const {
			return *reinterpret_cast<int*>((unsigned long)this + (unsigned long)0x0000159C);
		}

		const char* GetName() {
			typedef const char* (__thiscall* tGetName)(void*);
			return CallVFunction<tGetName>( this, 371 )(this);
		}

		const WeaponData_t* GetWpnData() {
			typedef const WeaponData_t* (__thiscall* tWpnData)(void*);
			return CallVFunction<tWpnData>( this, 450 )(this);
		}
	};
}