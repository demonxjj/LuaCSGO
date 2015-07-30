#pragma once

#include "Definitions.hpp"

namespace SDK {
	class IPanel {
	public:
		const char *GetName( unsigned int vguiPanel ) {
			typedef const char *(__thiscall* tGetName)(PVOID, unsigned int);
			return CallVFunction<tGetName>( this, 36 )(this, vguiPanel);
		}
#ifdef GetClassName
#undef GetClassName
#endif
		const char *GetClassName( unsigned int vguiPanel ) {
			typedef const char *(__thiscall* tGetClassName)(PVOID, unsigned int);
			return CallVFunction<tGetClassName>( this, 37 )(this, vguiPanel);
		}
		void PaintTraverse( unsigned int vguiPanel, bool forceRepaint, bool allowForce = true ) {
			typedef void( __thiscall* tPaintTraverse )(PVOID, unsigned int, bool, bool);
			return CallVFunction<tPaintTraverse>( this, 41 )(this, vguiPanel, forceRepaint, allowForce);
		}
	};
}