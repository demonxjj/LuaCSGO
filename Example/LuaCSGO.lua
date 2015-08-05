require("LuaCSGO/Settings")
require("LuaCSGO/LuaESP")

--These are used by other scripts, so find them before loading the scripts.
g_pEngine 		= Interfaces:GetEngineInterface()
g_pEngineTrace	= Interfaces:GetEngineTraceInterface()
g_pEntityList 	= Interfaces:GetEntityListInterface() 
g_pVGUIPanel	= Interfaces:GetPanelInterface()
g_pGlobals 		= Interfaces:GetGlobalVars()

local g_pMenuFont 	= nil
local iTopPanel		= nil

--[[
	- LuaCSGO interacts with the Lua scripts mostly by callbacks.
	- All callbacks are optional. Use only the ones you need.
	- You need to register callbacks that are gonna be used. Callbacks are registered using 'Callbacks:Register(type, name)'
	- Some callback types are allowed to have multiple callbacks.
	- See the documentation to know all the availables callbacks.
	]]

--[[
	Keyboard callback.
		- Will be called by the LuaCSGO dll when there's a pending keyboard event on the game window
		- Parameter: KeyboardEvent keyEvent
			- Contains information about the event. See documentation.
		- Returns: Whether the event was consumed or not. 
			- A consumed event is not passed down to the game.
	]]
function OnKeyEvent(keyEvent)
	if keyEvent.Type == EVENT_KEYDOWN then
		if keyEvent.KeyCode == KEY_HOME then
			LuaCSGO:Toggle()
			if not LuaCSGO:IsActive() then 
				--Not active, disable all callbacks except the keyboard
				Callbacks:DisableAllExcept(CALLBACK_KEYBOARD)
			else 
				--Enable all of the registered callbacks
				Callbacks:EnableAllCallbacks()
			end
		elseif keyEvent.KeyCode == KEY_INSERT then
			--This will force a reload of all the scripts
			RELOAD()
		end
		
	end
	
	return false
end

--[[
	Callback for a enabled PaintTraverse hook.
		- Will be called by the LuaCSGO dll each time a panel is painted
		- Parameter: unsigned int vguiPanel
			- ID of the panel that's being painted
	]]
function OnPaintTraverse(vguiPanel)
	if iTopPanel == nil then
		szPanelName = g_pVGUIPanel:GetName(vguiPanel)
		print(szPanelName)
		--Is this panel the top panel?
		if szPanelName == "MatSystemTopPanel" then
		
			--If it is, then save its number so we dont have to compare strings anymore
			iTopPanel = vguiPanel
			print("MatSystemTopPanel found. ID = " .. iTopPanel)
			
			--Create a font for drawing
			g_pMenuFont = DrawManager:CreateNewFont("Courier New", 16, false, false, true, true)
		end
	elseif vguiPanel == iTopPanel then
		
		--Is the local player in-game?
		if g_pEngine:IsInGame() then
			
			--Taking a screenshot. Don't draw anything, let's look legit! :-)
			if g_pEngine:IsTakingScreenshot() then return end
		
			local pLocalPlayer = g_pEntityList:GetEntityFromIndex(g_pEngine:GetLocalPlayer())
			
			--If local player is not valid then we return immediately.
			--This should never happen, but it is good to check anyways.
			if not pLocalPlayer:IsValid() then 
				return 
			end
			
			--For each entity...
			for i = 1, g_pEntityList:GetHighestEntityIndex() do
				--Grab the entity from the entity list
				local pEntity = g_pEntityList:GetEntityFromIndex(i)
				
				--Is it a valid entity?
				if pEntity:IsValid() then
					
					iClassID = pEntity:GetClassID()
					--Is it a player? (This check is not a must, but it is useful when you want to do something with entities that are not players)
					--	You could also compare by name, if you dont know the ID. like so: if pEntity:GetClassName() == "CCSPlayer"
					if iClassID == 34 --[[CCSPlayer]] then
						
						--Is it alive, not dormant and is not the local player?
						if pEntity:IsAlive() and not pEntity:IsDormant() and i ~= g_pEngine:GetLocalPlayer() then
							
							DrawBox(pLocalPlayer, pEntity)
							DrawNames(pLocalPlayer, pEntity, i)
						end	
					elseif iClassID == 103 then
						--Do something with CCSPlantedC4
					end
				end
			end
		end
	end
end

function Clamp(vec)
	if vec.Y > 180.0 then
		vec.Y = vec.Y - 360.0
	elseif vec.Y < -180.0 then
		vec.Y = vec.Y + 360.0
	end

	if vec.X > 89.0 then
		vec.X = 89.0;
	elseif vec.X < -89.0 then
		vec.X = -89.0
	end
	
	vec.Z = 0.0
	
	return vec
end

--[[
	Callback for a enabled CreateMove hook.
		- Will be called by the LuaCSGO dll once at every game tick
		- Parameters: 
			UserCMD userCmd
			VerifiedCMD verifiedCmd
	]]
function OnCreateMove(userCmd,verifiedCmd)
	--Local player will always be valid on CreateMove (LuaCSGO does the check before calling the callback)
	local pLocalPlayer = g_pEntityList:GetEntityFromIndex(g_pEngine:GetLocalPlayer())
	local oldAngles = userCmd.ViewAngles
	local punchAngles = pLocalPlayer:GetAimPunch()
	
	local newAngles = oldAngles - punchAngles * 2
	
	userCmd.ViewAngles = Clamp(newAngles)
	
	--Apply new UserCMD and updates CRC
	verifiedCmd:Update(userCmd)
end

local oldAimPunch = Vector()
local oldViewPunch = Vector()
local bRestore = false

function OnFrameStageNotifyBegin(stage)
	if stage == FRAME_RENDER_START then
		local pLocalPlayer = g_pEntityList:GetEntityFromIndex(g_pEngine:GetLocalPlayer())
		if pLocalPlayer:IsValid() then
			--Save the old angles
			oldViewPunch = pLocalPlayer:GetViewPunch()
			oldAimPunch = pLocalPlayer:GetAimPunch()
			
			--Reset view and aim punch
			pLocalPlayer:SetViewPunch(Vector(0,0,0))
			pLocalPlayer:SetAimPunch(Vector(0,0,0))
			
			--Set the restore flag
			bRestore = true
		end
	end
end

function OnFrameStageNotifyEnd(stage)
	--If the flag is set, then we have angles to restore
	if bRestore then
		local pLocalPlayer = g_pEntityList:GetEntityFromIndex(g_pEngine:GetLocalPlayer())
		if pLocalPlayer:IsValid() then
			pLocalPlayer:SetViewPunch(oldViewPunch)
			pLocalPlayer:SetAimPunch(oldAimPunch)
			bRestore = false
		end
	end
end

--Register used callbacks
--Callbacks:Register(CALLBACK_SCRIPTLOAD, 				OnScriptLoad)
--Callbacks:Register(CALLBACK_SCRIPTUNLOAD, 			OnScriptUnload)
--Callbacks:Register(CALLBACK_MOUSE, 					OnMouseEvent)
Callbacks:Register(CALLBACK_KEYBOARD, 					OnKeyEvent)
Callbacks:Register(CALLBACK_PAINTTRAVERSE, 				OnPaintTraverse)
Callbacks:Register(CALLBACK_CREATEMOVE, 				OnCreateMove)
Callbacks:Register(CALLBACK_FRAMESTAGENOTIFY_BEGIN, 	OnFrameStageNotifyBegin)
Callbacks:Register(CALLBACK_FRAMESTAGENOTIFY_END, 		OnFrameStageNotifyEnd)