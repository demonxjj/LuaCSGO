#pragma once

#include <Windows.h>

namespace SDK {
	//-----------------------------------------------------------------------------
	// Interface creation function
	//-----------------------------------------------------------------------------
	typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
	typedef void* (*InstantiateInterfaceFn)();

	typedef unsigned int VPANEL;
	typedef void( *pfnDemoCustomDataCallback )(unsigned char *pData, size_t iSize);

	//-----------------------------------------------------------------------------
	// Call a virtual function
	//-----------------------------------------------------------------------------
	template<typename FuncType>
	FuncType CallVFunction( void* ppClass, DWORD index ) {
		PDWORD pVTable = *(PDWORD*)ppClass;
		DWORD dwAddress = pVTable[index];
		return (FuncType)(dwAddress);
	}

	enum FontFeature {
		FONT_FEATURE_ANTIALIASED_FONTS = 1,
		FONT_FEATURE_DROPSHADOW_FONTS = 2,
		FONT_FEATURE_OUTLINE_FONTS = 6,
	};

	enum FontDrawType {
		FONT_DRAW_DEFAULT = 0,
		FONT_DRAW_NONADDITIVE,
		FONT_DRAW_ADDITIVE,
		FONT_DRAW_TYPE_COUNT = 2,
	};

	enum FontFlags {
		FONTFLAG_NONE,
		FONTFLAG_ITALIC = 0x001,
		FONTFLAG_UNDERLINE = 0x002,
		FONTFLAG_STRIKEOUT = 0x004,
		FONTFLAG_SYMBOL = 0x008,
		FONTFLAG_ANTIALIAS = 0x010,
		FONTFLAG_GAUSSIANBLUR = 0x020,
		FONTFLAG_ROTARY = 0x040,
		FONTFLAG_DROPSHADOW = 0x080,
		FONTFLAG_ADDITIVE = 0x100,
		FONTFLAG_OUTLINE = 0x200,
		FONTFLAG_CUSTOM = 0x400,
		FONTFLAG_BITMAP = 0x800,
	};

	enum EntityFlags {
		FL_ONGROUND		= (1 << 0),	
		FL_DUCKING		= (1 << 1),	
		FL_WATERJUMP	= (1 << 2),
		FL_ONTRAIN		= (1 << 3),
		FL_INRAIN		= (1 << 4),
		FL_FROZEN		= (1 << 5),
		FL_ATCONTROLS	= (1 << 6),
		FL_CLIENT		= (1 << 7),
		FL_FAKECLIENT	= (1 << 8)
	};

	enum LifeState {
		LIFE_ALIVE = 0,// alive
		LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
		LIFE_DEAD = 2 // dead. lying still.
	};

	struct OcclusionParams_t {
		float	m_flMaxOccludeeArea;
		float	m_flMinOccluderArea;
	};

	enum SkyboxVisibility_t {
		SKYBOX_NOT_VISIBLE = 0,
		SKYBOX_3DSKYBOX_VISIBLE,
		SKYBOX_2DSKYBOX_VISIBLE,
	};
	
	enum EngineInputContextId_t {
		ENGINE_INPUT_CONTEXT_GAME = 0,
		ENGINE_INPUT_CONTEXT_GAMEUI,
	};

	enum InitReturnVal_t {
		INIT_FAILED = 0,
		INIT_OK,

		INIT_LAST_VAL
	};

	enum AppSystemTier_t {
		APP_SYSTEM_TIER0 = 0,
		APP_SYSTEM_TIER1,
		APP_SYSTEM_TIER2,
		APP_SYSTEM_TIER3,

		APP_SYSTEM_TIER_OTHER
	};

	enum InvalidatePhysicsBits_t {
		POSITION_CHANGED = 0x1,
		ANGLES_CHANGED = 0x2,
		VELOCITY_CHANGED = 0x4,
		ANIMATION_CHANGED = 0x8,		// Means cycle has changed, or any other event which would cause render-to-texture shadows to need to be rerendeded
		BOUNDS_CHANGED = 0x10,		// Means render bounds have changed, so shadow decal projection is required, etc.
		SEQUENCE_CHANGED = 0x20		// Means sequence has changed, only interesting when surrounding bounds depends on sequence																				
	};

}