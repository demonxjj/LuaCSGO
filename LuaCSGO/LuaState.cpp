#include "LuaState.h"

LuaState::LuaState( void ) {
	m_pState = luaL_newstate();
	luaL_openlibs( m_pState );
}

LuaState::~LuaState( void ) {
	if(m_pState != NULL) {
		lua_close( m_pState );
		m_pState = NULL;
	}
}

bool LuaState::LoadFile( const char *strFilename ) {
	bool fSuccess = false;
	int iErr = 0;

	if(( iErr = luaL_dofile( m_pState, strFilename ) ) == 0) {
		fSuccess = true;
	}

	if(fSuccess == false) {
		if(m_pDbg != NULL) m_pDbg->ErrorRun( iErr );
	}

	return fSuccess;
}

bool LuaState::SelectFunction( const char* strFunctionName ) {
	m_nArgs = 0;
	lua_settop( m_pState, 0 );
	lua_getglobal( m_pState, strFunctionName );
	if(lua_isfunction( m_pState, -m_nArgs - 1 )) {
		return true;
	}
	return false;
}
