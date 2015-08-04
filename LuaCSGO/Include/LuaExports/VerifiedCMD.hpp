#pragma once

#include "UserCMD.hpp"

namespace LuaCSGO
{
	class VerifiedCMD {
	public:
		VerifiedCMD( SDK::CVerifiedUserCmd* pVerified ) {
			_pVerified = pVerified;
		}

		UserCMD GetUserCMD() {
			return UserCMD( &_pVerified->m_cmd );
		}

		unsigned long GetCRC() {
			return _pVerified->m_crc;
		}

		void SetUserCMD( UserCMD cmd ) {
			_pVerified->m_cmd = *( cmd.pCmd );
		}

		void SetCRC( unsigned long v) {
			_pVerified->m_crc = v;
		}

		void Update( UserCMD cmd ) {
			SDK::Vector newAngles = cmd.GetViewAngles();
			if(newAngles.x > 89.0f || newAngles.x < -89.0f ||
				newAngles.y > 180.0f || newAngles.y < -180.0f ||
				newAngles.z != 0.0f ||
				!std::isfinite( newAngles.x ) ||
				!std::isfinite( newAngles.y ) ||
				!std::isfinite( newAngles.z )) {

				printf(
					"WARNING: VerifiedCMD:Update received UserCMD with invalid angles! This will cause Untrusted.\n"
					"         Angles: (%f, %f, %f)\n", newAngles.x, newAngles.y, newAngles.z );
				printf( "WARNING: Invalid angles were discarded!\n" );
			} else {
				SetUserCMD( cmd );
				SetCRC( cmd.GetChecksum() );
			}
		}
	private:
		SDK::CVerifiedUserCmd* _pVerified;
	};
}
