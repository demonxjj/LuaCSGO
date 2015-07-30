#pragma once

#include <vector>

#include "../LuaLib/lua.hpp"
#include "LuaDebugger.h"

struct any {
	enum type { Number, String };
	any( double e ) { m_data.NUMBER = e; m_type = Number; }
	any( const char* e ) { m_data.STRING = e; m_type = String; }
	type get_type() const { return m_type; }
	double get_number() const { return m_data.NUMBER; }
	const char* get_string() const { return m_data.STRING; }

private:
	type m_type;
	union {
		double NUMBER;
		const char *STRING;
	} m_data;
};

class LuaState {
public:
	LuaState();
	~LuaState();

	bool LoadFile( const char *strFileName );
	void AttachDebugger( LuaDebugger *dbg );
	bool IsValid();
	operator lua_State *();

	template<class ...Args>
	bool CallFunction( const char* strFunctionName, Args... args ) {
		return CallFunction_imp( strFunctionName, any( args )... );
	}

private:
	template<class ...Args>
	bool CallFunction_imp( const char* strFunctionName, const Args&... args ) {
		m_nArgs = 0;
		lua_settop( m_pState, 0 );
		lua_getglobal( m_pState, strFunctionName );

		if(lua_isfunction( m_pState, -m_nArgs - 1 )) {
			std::vector<any> vec = { args... };
			for(unsigned i = 0; i < vec.size(); ++i) {
				m_nArgs++;
				switch(vec[i].get_type()) {
					case any::Number:
						lua_pushnumber( m_pState, vec[i].get_number() );
						break;
					case any::String:
						lua_pushstring( m_pState, vec[i].get_string() );
						break;
				}
			}

			int iErr = lua_pcall( m_pState, m_nArgs, 0, 0 );

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
