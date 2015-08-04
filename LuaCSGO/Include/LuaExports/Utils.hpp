#pragma once

#include "BaseClientEntity.hpp"

namespace LuaCSGO
{
	class Utils {
	public:
		SDK::Vector GetEntityBone( BaseClientEntity pEntity, int iBoneNumber ) {
			SDK::matrix3x4_t boneMatrix[128];

			if(!( ( SDK::CBaseEntity* )pEntity )->SetupBones( boneMatrix, 128, 0x00000100, SDK::Interfaces::g_pEngine->GetLastTimeStamp() ))
				return SDK::Vector();

			return SDK::Vector( boneMatrix[iBoneNumber][0][3], boneMatrix[iBoneNumber][1][3], boneMatrix[iBoneNumber][2][3] );
		}

		SDK::Vector WorldToScreen( const SDK::Vector &origin ) {
			SDK::Vector screen;
			if(!ScreenTransform( origin, screen )) {
				int iScreenWidth, iScreenHeight;
				SDK::Interfaces::g_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );

				float fScreenX = iScreenWidth / 2.0f;
				float fScreenY = iScreenHeight / 2.0f;

				fScreenX += 0.5f * screen.x * iScreenWidth + 0.5f;
				fScreenY -= 0.5f * screen.y * iScreenHeight + 0.5f;

				screen.x = fScreenX;
				screen.y = fScreenY;
			}
			return screen;
		}

		bool ScreenTransform( const SDK::Vector& point, SDK::Vector& screen ) {
			const SDK::VMatrix& w2sMatrix = SDK::Interfaces::g_pEngine->WorldToScreenMatrix();
			screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
			screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];

			float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];
			screen.z = 0.0f;

			bool bIsBehind = false;
			if(w < 0.001f) {
				bIsBehind = true;
				screen.Invalidate();
			} else {
				float invw = 1.0f / w;
				screen.x *= invw;
				screen.y *= invw;
			}

			return bIsBehind;
		}
	};
}