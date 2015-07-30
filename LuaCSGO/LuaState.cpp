#include "LuaState.h"

LuaState::LuaState() {
	m_pState = luaL_newstate();
	luaL_openlibs( m_pState );
}

LuaState::~LuaState() {
	if(m_pState != NULL) {
		lua_close( m_pState );
		m_pState = NULL;
	}
}

LuaState::operator lua_State*() {
	return m_pState;
}
void LuaState::AttachDebugger( LuaDebugger *dbg ) {
	m_pDbg = dbg; 
}

bool LuaState::IsValid() {
	return m_pState != NULL;
}

bool LuaState::LoadFile( const char *strFilename ) {
	int iErr = 0;

	if(( iErr = luaL_dofile( m_pState, strFilename ) ) == 0) {
		return true;
	}

	if(m_pDbg != NULL) m_pDbg->ErrorRun( iErr );

	return false;
}