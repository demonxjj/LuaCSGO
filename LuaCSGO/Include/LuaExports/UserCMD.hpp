#pragma once

#include "CSGO-SDK/SDK.hpp"

namespace LuaCSGO
{
	class UserCMD {
		friend class VerifiedCMD;
	public:
		UserCMD( SDK::CUserCmd* cmd ) {
			pCmd = cmd;
		}

		int GetButtons() {
			return pCmd->buttons;
		}
		SDK::Vector GetViewAngles() {
			return pCmd->viewangles;
		}
		float GetForwardMove() {
			return pCmd->forwardmove;
		}
		float GetSideMove() {
			return pCmd->sidemove;
		}
		float GetUpMove() {
			return pCmd->upmove;
		}
		unsigned long GetChecksum() {
			return pCmd->GetChecksum();
		}

		void SetButtons(int buttons) {
			pCmd->buttons = buttons;
		}
		void SetViewAngles( SDK::Vector va) {
			pCmd->viewangles = va;
		}
		void SetForwardMove(float f) {
			pCmd->forwardmove = f;
		}
		void SetSideMove( float f ) {
			pCmd->sidemove = f;
		}
		void SetUpMove( float f ) {
			pCmd->upmove = f;
		}
	private:
		SDK::CUserCmd* pCmd;
	};
}
