#pragma once

#include "Definitions.hpp"

namespace SDK {
	class IClientNetworkable;
	class IClientUnknown;
	class IClientEntity;

	class IClientEntityList {
	public:
		// Get IClientNetworkable interface for specified entity
		virtual IClientNetworkable*	GetClientNetworkable( int entnum ) = 0;

		virtual void*				vtablepad0x1( void ) = 0;
		virtual void*				vtablepad0x2( void ) = 0;

		virtual IClientEntity*		GetClientEntity( int entNum ) = 0;
		virtual IClientEntity*		GetClientEntityFromHandle( CBaseHandle hEnt ) = 0;

		virtual int					NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
		virtual int					GetHighestEntityIndex( void ) = 0;
		virtual void				SetMaxEntities( int maxEnts ) = 0;
		virtual int					GetMaxEntities() = 0;
	};
}