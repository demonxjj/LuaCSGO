#pragma once

#include <vector>

#include "../LuaLib/lua.hpp"
#include "LuaDebugger.h"


struct any {
	enum type { Int, Float, String };
	any( int   e ) { m_data.INT = e; m_type = Int; }
	any( float e ) { m_data.FLOAT = e; m_type = Float; }
	any( const char* e ) { m_data.STRING = e; m_type = String; }
	type get_type() const { return m_type; }
	int get_int() const { return m_data.INT; }
	float get_float() const { return m_data.FLOAT; }
	const char* get_string() const { return m_data.STRING; }

private:
	type m_type;
	union {
		int   INT;
		float FLOAT;
		const char *STRING;
	} m_data;
};

class LuaState {
public:
	LuaState();
	virtual ~LuaState();

	bool LoadFile( const char *strFileName );

	template<class ...Args>
	bool CallFunction( const char* strFunctionName, int nReturns, Args... args ) {
		return CallFunction_imp( strFunctionName, nReturns, any( args )... );
	}

	operator lua_State *( void ) { return m_pState; }

	void AttachDebugger( LuaDebugger *dbg ) { m_pDbg = dbg; }
	bool IsValid() { return m_pState != NULL; }

private:
	template<class ...Args>
	bool CallFunction_imp( const char* strFunctionName, int nReturns, const Args&... args ) {
		m_nArgs = 0;
		lua_settop( m_pState, 0 );
		lua_getglobal( m_pState, strFunctionName );

		if(lua_isfunction( m_pState, -m_nArgs - 1 )) {
			std::vector<any> vec = { args... };
			for(unsigned i = 0; i < vec.size(); ++i) {
				m_nArgs++;
				switch(vec[i].get_type()) {
					case any::Int: 
						lua_pushnumber( m_pState, vec[i].get_int() );
						break;
					case any::Float:
						lua_pushnumber( m_pState, vec[i].get_float() );
						break;
					case any::String:
						lua_pushstring( m_pState, vec[i].get_string() );
						break;
				}
			}

			int iErr = lua_pcall( m_pState, m_nArgs, nReturns, 0 );

			if(iErr != 0) {
				if(m_pDbg != NULL) m_pDbg->ErrorRun( iErr );
				return false;
			}
			return true;
		}
		return false;
	}
protected:
	lua_State *m_pState;
	LuaDebugger *m_pDbg;
	int m_nArgs;
};