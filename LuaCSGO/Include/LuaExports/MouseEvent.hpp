#pragma once

#include "Events.hpp"

namespace LuaCSGO
{
	class MouseEvent {
	public:
		MouseEvent() {
			Type = Events::LUACSGO_EVENT_UNKNOWN;
			Button = ButtonCodes::LUACSGO_BUTTON_NONE;
			MouseX = 0;
			MouseY = 0;
		}
		MouseEvent( Events type, ButtonCodes button, int x, int y ) {
			Type = type;
			Button = button;
			MouseX = x;
			MouseY = y;
		}

		Events GetType() { return Type; }
		ButtonCodes GetButton() { return Button; }
		int GetX() { return MouseX; }
		int GetY() { return MouseY; }

	private:
		Events Type;
		ButtonCodes Button;
		int MouseX;
		int MouseY;
	};
}