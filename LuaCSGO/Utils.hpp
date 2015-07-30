#pragma once

#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "CSGO-SDK\SDK.hpp"

class Utils {
public:
	static SDK::Vector GetEntityBone( SDK::IClientEntity* pEntity, int Bone ) {
		SDK::matrix3x4_t boneMatrix[128];

		if( !pEntity->SetupBones( boneMatrix, 128, 0x00000100, SDK::Interfaces::g_pEngine->GetLastTimeStamp() ) )
			return SDK::Vector();

		SDK::matrix3x4_t hitbox = boneMatrix[Bone];

		return SDK::Vector( hitbox[0][3], hitbox[1][3], hitbox[2][3] );
	}

	static bool ScreenTransform( const SDK::Vector& point, SDK::Vector& screen ) {
		const SDK::VMatrix& w2sMatrix = SDK::Interfaces::g_pEngine->WorldToScreenMatrix();
		screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];
			
		float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];
		screen.z = 0.0f;

		bool bIsBehind = false;
		if( w < 0.001f ) {
			bIsBehind = true;
			screen.x *= 100000;
			screen.y *= 100000;
		} else {
			float invw = 1.0f / w;
			screen.x *= invw;
			screen.y *= invw;
		}

		return bIsBehind;
	}

	static bool WorldToScreen( const SDK::Vector &origin, SDK::Vector &screen ) {
		bool bSuccess = false;

		if( !ScreenTransform( origin, screen ) ) {
			int iScreenWidth, iScreenHeight;
			SDK::Interfaces::g_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );

			float fScreenX = iScreenWidth / 2.0f;
			float fScreenY = iScreenHeight / 2.0f;

			fScreenX += 0.5f * screen.x * iScreenWidth + 0.5f;
			fScreenY -= 0.5f * screen.y * iScreenHeight + 0.5f;

			screen.x = fScreenX;
			screen.y = fScreenY;

			bSuccess = true;
		}

		return bSuccess;
	}

	static DWORD GetGameClass( std::string className ) {
		DWORD dwClass;
		for( int i = 0; i < SDK::Interfaces::g_pEntityList->GetHighestEntityIndex(); i++ ) {
			SDK::IClientEntity* pEntity = SDK::Interfaces::g_pEntityList->GetClientEntity( i );

			if( pEntity != 0 ) {
				if( className.compare(pEntity->GetClientClass()->GetName()) == 0 ) {
					dwClass = (DWORD)pEntity;
					break;
				}
			}
		}
		return dwClass;
	}

	static SDK::Vector RotateVector( const SDK::Vector& origin, SDK::Vector point, const SDK::QAngle& angle ) {

		float s = sin( angle.y * 3.141592653589 / 180 );
		float c = cos( angle.y * 3.141592653589 / 180 );

		// translate point back to origin:
		point.x -= origin.x;
		point.y -= origin.y;

		// rotate point
		float xnew = point.x * c + point.y * s;
		float ynew = point.x * s - point.y * c;

		// translate point back:
		point.x = xnew + origin.x;
		point.y = ynew + origin.y;
		return point;
	}

	static SDK::Vector RotateVectorAroundX( const SDK::Vector& origin, SDK::Vector point, const SDK::QAngle& angle ) {
			
		float s = sin( angle.x * 3.141592653589 / 180 );
		float c = cos( angle.x * 3.141592653589 / 180 );

		// translate point back to origin:
		point.x -= origin.x;
		point.z -= origin.z;

		// rotate point
		float xnew = point.x * c + point.z * s;
		float znew = -point.x * s + point.z * c;

		// translate point back:
		point.x = xnew + origin.x;
		point.z = znew + origin.z;
		return point;
	}

	static SDK::CBaseEntity* GetBestAimbotTarget( SDK::CBaseEntity* pLocalEntity, bool targetTeam ) {
		float fClosestDistance = 99999.99f;
		SDK::CBaseEntity* vBestTarget = nullptr;

		if( pLocalEntity && pLocalEntity->IsAlive() ) {
			SDK::Vector vLocalHead = pLocalEntity->GetEyePos();
			for( int i = 1; i < 65; i++ ) {
				SDK::CBaseEntity* pEntity = (SDK::CBaseEntity*)SDK::Interfaces::g_pEntityList->GetClientEntity( i );

				if( !pEntity )
					continue;
				if( !pEntity->IsAlive() )
					continue;
				if( pEntity->IsDormant() )
					continue;
				if( i == SDK::Interfaces::g_pEngine->GetLocalPlayer() )
					continue;
				if( !targetTeam && pEntity->GetTeamNum() == pLocalEntity->GetTeamNum() )
					continue;

				if( IsVisible( pEntity ) ) {
					float dist = pEntity->GetOrigin().DistTo( pLocalEntity->GetOrigin() );
					if( dist < fClosestDistance ) {
						fClosestDistance = dist;
						vBestTarget = pEntity;
					}
				}
			}
		}
		return vBestTarget;
	}

	static SDK::CBaseEntity* GetEntityClosestToXHair( SDK::CBaseEntity* pLocalEntity, bool targetTeam, float maxFoV ) {
		float fClosestDistance = 99999.99f;
		SDK::Vector vHeadScreen;
		int iWidth, iHeight;
		SDK::CBaseEntity* vBestTarget = nullptr;

		if( pLocalEntity && pLocalEntity->IsAlive() ) {
			SDK::Vector vLocalHead = pLocalEntity->GetEyePos();
			for( int i = 1; i < 65; i++ ) {
				SDK::CBaseEntity* pEntity = (SDK::CBaseEntity*)SDK::Interfaces::g_pEntityList->GetClientEntity( i );

				if( !pEntity )
					continue;
				if( !pEntity->IsAlive() )
					continue;
				if( pEntity->IsDormant() )
					continue;
				if( i == SDK::Interfaces::g_pEngine->GetLocalPlayer() )
					continue;
				if( !targetTeam && pEntity->GetTeamNum() == pLocalEntity->GetTeamNum() )
					continue;

				SDK::Vector vHead = GetEntityBone( pEntity, 10 );
				SDK::Interfaces::g_pEngine->GetScreenSize( iWidth, iHeight );
				if( WorldToScreen( vHead, vHeadScreen ) ) {
					if( IsVisible( pEntity ) ) {
						float xDiff = vHeadScreen.x - iWidth / 2;
						float yDiff = vHeadScreen.y - iHeight / 2;

						float dist = sqrt( yDiff*yDiff + xDiff*xDiff );
						if( dist < maxFoV && dist < fClosestDistance ) {
							fClosestDistance = dist;
							vBestTarget = pEntity;
						}
					}
				}
			}
		}
		return vBestTarget;
	}

	static bool GetAimbotAngles( const SDK::Vector& src, const SDK::Vector& dst, SDK::QAngle& res ) {
		if( !src.IsValid() || !dst.IsValid() ) return false;

		SDK::Vector delta = src - dst;
		float length = delta.Length2D(); //sqrt(x*x + y*y)
		if( length == 0 || delta.x == 0 ) return false;

		float pitch = (float)(atan( delta.z / length ) * 180 / 3.141592653590);
		float yaw = (float)(atan( delta.y / delta.x ) * 180 / 3.141592653590);
		float roll = 0.0f;

		if( delta.x >= 0.0 ) {
			yaw += 180.0f;
		}

		res = SDK::QAngle( pitch, yaw, roll );
		return true;
	}

	static bool IsVisibleEx( SDK::Vector start, SDK::Vector end, SDK::CBaseEntity* target, int skip ) {
			
		SDK::CGameTrace tr;
		SDK::Ray_t ray;
		SDK::CTraceFilter filter;
		filter.pSkip = (SDK::CBaseEntity*)SDK::Interfaces::g_pEntityList->GetClientEntity( skip );

		ray.Init( start, end );
		SDK::Interfaces::g_pEngineTrace->TraceRay( ray, 0x46004003, &filter, &tr );
			
		return (tr.m_pEnt == target || tr.fraction > 0.97f);
	}

	static bool IsVisible( SDK::CBaseEntity* target ) {
		SDK::CBaseEntity* pLocal = (SDK::CBaseEntity*)SDK::Interfaces::g_pEntityList->GetClientEntity( SDK::Interfaces::g_pEngine->GetLocalPlayer() );
		if( !pLocal ) return false;
		return IsVisibleEx( pLocal->GetEyePos(), GetEntityBone( target, 10 ), target, SDK::Interfaces::g_pEngine->GetLocalPlayer() );
	}

	static SDK::QAngle ClampAngles( const SDK::QAngle &angles ) {
		SDK::QAngle res = angles;
		if( res.y > 179.99f )
			res.y -= 360;

		if( res.y < -179.99f )
			res.y += 360;

		if( res.x > 89.0f )
			res.x = 89.0f;

		if( res.x < -89.0f )
			res.x = -89.0f;

		res.z = 0;
		return res;
	}

	static void Clamp( SDK::QAngle &angles ) {
		angles = ClampAngles( angles );
	}

	static DWORD SigScan( std::string module, BYTE *bSig, const char* szMask ) {
		_MODULEINFO modInfo;
		GetModuleInformation( GetCurrentProcess(), GetModuleHandleA( module.c_str() ), &modInfo, sizeof( _MODULEINFO ) );
		return FindPattern( (DWORD)modInfo.lpBaseOfDll, modInfo.SizeOfImage, bSig, szMask );
	}

private:
	static bool Mask( const BYTE* pData, const BYTE* bMask, const char* szMask ) {
		for( ; *szMask; ++szMask, ++pData, ++bMask )
			if( *szMask == 'x' && *pData != *bMask )
				return false;
		return (*szMask) == NULL;
	}

	static DWORD FindPattern( DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char* szMask ) {
		for( DWORD i = 0; i<dwLen; i++ )
			if( Mask( (BYTE*)(dwAddress + i), bMask, szMask ) )
				return (DWORD)(dwAddress + i);
		return 0;
	}

};