#pragma once

#include "CSGO-SDK/SDK.hpp"

namespace LuaCSGO
{
	class PanelInterface {
	public:
		PanelInterface( SDK::IPanel* pPanel ) {
			m_pPanel = pPanel;
		}

		const char* GetName( unsigned int panel ) {
			return m_pPanel->GetName( panel );
		}
	private:
		SDK::IPanel* m_pPanel;
	};
}