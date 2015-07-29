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

bool LuaState::CallFunction( int nArgs, int nReturns /* = 0 */ ) {
	bool fSuccess = false;

	if(lua_isfunction( m_pState, -nArgs - 1 )) {
		int iErr = 0;
		iErr = lua_pcall( m_pState, nArgs, nReturns, 0 );

		if(iErr == 0) {
			fSuccess = true;
		}
		else {
			if(m_pDbg != NULL) m_pDbg->ErrorRun( iErr );
		}
	}

	return fSuccess;
}

