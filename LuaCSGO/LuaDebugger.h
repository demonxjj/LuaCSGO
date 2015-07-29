#pragma once

#include "../LuaLib/lua.hpp"

enum {
	DBG_MASK_CALL = LUA_MASKCALL,
	DBG_MASK_RET = LUA_MASKRET,
	DBG_MASK_LINE = LUA_MASKLINE,
	DBG_MASK_COUNT = LUA_MASKCOUNT
};

class LuaState;

class LuaDebugger {
public:
	LuaDebugger( LuaState& vm );
	virtual ~LuaDebugger( void );

	void SetHooksFlag( int iMask );
	void SetCount( int iCount ) { m_iCountMask = iCount; }

	void ErrorRun( int iErrorCode );

protected:
	int m_iCountMask;
	LuaState& m_vm;
};