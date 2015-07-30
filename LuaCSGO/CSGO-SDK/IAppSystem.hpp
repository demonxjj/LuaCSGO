#pragma once

#include "Definitions.hpp"

namespace SDK
{
	struct AppSystemInfo_t {
		const char *m_pModuleName;
		const char *m_pInterfaceName;
	};

	class IAppSystem {
	public:
		// Here's where the app systems get to learn about each other 
		virtual bool Connect( CreateInterfaceFn factory ) = 0; // 0
		virtual void Disconnect() = 0; // 1
		virtual void *QueryInterface( const char *pInterfaceName ) = 0; // 2
		virtual InitReturnVal_t Init() = 0; // 3
		virtual void Shutdown() = 0; // 4
		virtual const AppSystemInfo_t* GetDependencies() = 0; // 5
		virtual AppSystemTier_t GetTier() = 0; // 6
		virtual void Reconnect( CreateInterfaceFn factory, const char *pInterfaceName ) = 0; // 7
		virtual void UnkFunc() = 0; // 8
	};
}