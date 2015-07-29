#pragma once

#include "../LuaLib/lua.hpp"
#include "LuaDebugger.h"

class LuaState {
public:
	LuaState();
	virtual ~LuaState();

	bool LoadFile( const char *strFilename );
	bool CallFunction( int nArgs, int nReturns = 0 );
	operator lua_State *( void ) { return m_pState; }

	void AttachDebugger( LuaDebugger *dbg ) { m_pDbg = dbg; }

	bool IsValid() { return m_pState != NULL; }

protected:
	lua_State *m_pState;
	LuaDebugger *m_pDbg;
};