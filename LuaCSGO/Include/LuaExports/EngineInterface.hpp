#pragma once

#include "CSGO-SDK/SDK.hpp"

namespace LuaCSGO
{
	class EngineInterface {
	public:
		EngineInterface( SDK::IVEngineClient* pEngine ) {
			m_pEngine = pEngine;
		}

		bool IsInGame() {
			return m_pEngine->IsInGame();
		}

		bool IsConnected() {
			return m_pEngine->IsConnected();
		}

		bool IsPlayingDemo() {
			return m_pEngine->IsPlayingDemo();
		}

		bool IsRecordingDemo() {
			return m_pEngine->IsRecordingDemo();
		}

		bool IsTakingScreenshot() {
			return m_pEngine->IsTakingScreenshot();
		}

		void ExecuteCommand( const char* szCommand ) {
			m_pEngine->ExecuteClientCmd( szCommand );
		}

		int GetLocalPlayer() {
			return m_pEngine->GetLocalPlayer();
		}

		int GetMaxClients() {
			return m_pEngine->GetMaxClients();
		}

		const char* GetClientName( int entNum ) {
			static SDK::PlayerInfo pInfo;
			m_pEngine->GetPlayerInfo( entNum, &pInfo );
			return pInfo.szName;
		}

		const char* GetLevelName() {
			return m_pEngine->GetLevelName();
		}
		SDK::Vector2D GetScreenSize() {
			int w, h;
			m_pEngine->GetScreenSize( w, h );
			return SDK::Vector2D( w, h );
		}

		SDK::Vector GetViewAngles() {
			SDK::Vector va;
			m_pEngine->GetViewAngles( va );
			return va;
		}

		void SetViewAngles( SDK::Vector v ) {
			m_pEngine->SetViewAngles( v );
		}
	private:
		SDK::IVEngineClient* m_pEngine;
	};
}