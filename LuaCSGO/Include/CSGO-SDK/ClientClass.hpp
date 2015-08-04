#pragma once

#include "Definitions.hpp"

#include "Recv.hpp"
#include "IClientNetworkable.hpp"

namespace SDK {

	class ClientClass;
	// Linked list of all known client classes
	extern ClientClass *g_pClientClassHead;

	// The serial number that gets passed in is used for ehandles.
	typedef IClientNetworkable*	(*CreateClientClassFn)(int entnum, int serialNum);
	typedef IClientNetworkable*	(*CreateEventFn)();

	//-----------------------------------------------------------------------------
	// Purpose: Client side class definition
	//-----------------------------------------------------------------------------
	class ClientClass {
	public:
		ClientClass( char *pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable *pRecvTable ) {
			m_pNetworkName = pNetworkName;
			m_pCreateFn = createFn;
			m_pCreateEventFn = createEventFn;
			m_pRecvTable = pRecvTable;

			// Link it in
			m_pNext = g_pClientClassHead;
			g_pClientClassHead = this;
		}

		const char* GetName() { return m_pNetworkName; }

	public:
		CreateClientClassFn		m_pCreateFn;
		CreateEventFn			m_pCreateEventFn;	// Only called for event objects.
		char					*m_pNetworkName;
		RecvTable				*m_pRecvTable;
		ClientClass				*m_pNext;
		int						m_ClassID;	// Managed by the engine.
	};
}