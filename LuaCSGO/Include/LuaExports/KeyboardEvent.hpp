#pragma once

#include "Events.hpp"

namespace LuaCSGO
{
	class KeyboardEvent {
	public:
		KeyboardEvent() {
			Type = LUACSGO_EVENT_UNKNOWN;
			Key = LUACSGO_KEY_NONE;
		}
		KeyboardEvent( Events type, int key ) {
			Type = type;
			Key = key;
		}

		Events GetType() { return Type; }
		int GetKeyCode() { return Key; }
	private:
		Events Type;
		int Key;
	};
}