#pragma once

#include <memory>
#include <Windows.h>
#include "CSGO-SDK\SDK.hpp"
#include "LuaState.h"
#include "Offsets.hpp"
#include "Utils.hpp"
#include "VTableHook.hpp"

#define INTERFACE_IVENGINECLIENT "VEngineClient013"
#define INTERFACE_CLIENT_DLL "VClient017"
#define INTERFACE_ENTITY_LIST "VClientEntityList003"
#define INTERFACE_VGUI_SURFACE "VGUI_Surface031"
#define INTERFACE_VGUI_PANEL "VGUI_Panel009"
#define INTERFACE_ENGINETRACE "EngineTraceClient004"
#define CREATEINTERFACE_PROCNAME "CreateInterface"