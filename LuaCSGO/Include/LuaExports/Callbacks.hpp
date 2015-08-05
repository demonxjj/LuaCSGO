#pragma once

#include "luabind\luabind.hpp"

#include <tuple>
#include <unordered_map>
#include <vector>

namespace LuaCSGO
{
	enum CallbackType {
		LUACSGO_CALLBACK_SCRIPTLOAD,
		LUACSGO_CALLBACK_SCRIPTUNLOAD,
		LUACSGO_CALLBACK_MOUSE,
		LUACSGO_CALLBACK_KEYBOARD,
		LUACSGO_CALLBACK_PAINTTRAVERSE,
		LUACSGO_CALLBACK_CREATEMOVE,
		LUACSGO_CALLBACK_FRAMESTAGENOTIFY_BEGIN,
		LUACSGO_CALLBACK_FRAMESTAGENOTIFY_END
	};

	class Callbacks {
	public:

		bool RegisterCallback( CallbackType type, luabind::object f ) {
			if(HasCallback( type ) && !AllowsMultipleCallbacks( type )) {
				printf( "Only a single callback is allowed for this callback type.\n" );
				return false;
			}

			//if(!lua_isfunction(f.interpreter(), 0))

			std::get<0>( _callbacks[(CallbackType)type] ) = true;
			std::get<1>( _callbacks[(CallbackType)type] ).push_back( f );
			return true;
		}
		
		void DisableAllExcept( CallbackType type ) {
			for(auto it = _callbacks.begin(); it != _callbacks.end(); it++) {
				if(it->first != type)
					std::get<0>( it->second ) = false;
			}
		}

		void EnableAllCallbacks() {
			for(auto it = _callbacks.begin(); it != _callbacks.end(); it++)
				std::get<0>( it->second ) = true;
		}

		bool AllowsMultipleCallbacks( CallbackType type ) {
			return type != LUACSGO_CALLBACK_MOUSE && type != LUACSGO_CALLBACK_KEYBOARD;
		}

		bool HasCallback( CallbackType type ) {
			auto it = _callbacks.find( type );
			return it != _callbacks.end() && std::get<0>(*it);
		}
		std::vector<luabind::object> GetCallbacks( CallbackType type ) {
			if(!HasCallback( type )) throw "No callback registered for this event.";
			if(std::get<0>( _callbacks[type] ))
				return std::get<1>( _callbacks[type] );
			else return std::vector<luabind::object>();
		}
		void Reset() {
			_callbacks.clear();
		}
	private:
		std::unordered_map<CallbackType, std::tuple<bool, std::vector<luabind::object>>> _callbacks;
	};
}