require("LuaCSGO/Settings")

local ESPFont = DrawManager:CreateNewFont("Arial", 16, false, false, false, true)


local function GetHeadPosition(pEntity)
	vHead = Utils:GetEntityBone(pEntity, 10)
	
	if vHead:IsValid() then
		vHead.Z = vHead.Z + 15
	end
	
	return vHead
end

local function IsVisible(pLocalPlayer, pEntity)
	targetHead = Utils:GetEntityBone(pEntity, 10)
	
	if not targetHead:IsValid() then return false end
	
	trace = g_pEngineTrace:TraceRay(pLocalPlayer:GetEyePos(), targetHead, pLocalPlayer, MASK_SHOT)
	
	return trace:DidHitEntity(pEntity) or trace:IsVisible()
end

local function SetColor(pLocalPlayer, pEntity)
	local bIsEnemy = pLocalPlayer:GetTeam() ~= pEntity:GetTeam()
	if bIsEnemy then
		if IsVisible(pLocalPlayer, pEntity) then
			DrawManager:SetDrawColor(0xFFFFFF14)
		else
			DrawManager:SetDrawColor(0xFFFF0000)
		end
	else
		if IsVisible(pLocalPlayer, pEntity) then
			DrawManager:SetDrawColor(0xFF007F00)
		else
			DrawManager:SetDrawColor(0xFF0000FF)
		end
	end
end

function DrawNames(pLocalPlayer, pEntity, entNum)
	
	SetColor(pLocalPlayer, pEntity)
	
	local vHead 		= GetHeadPosition(pEntity)
	local vHeadScreen 	= Utils:WorldToScreen(vHead)
	
	if vHeadScreen:IsValid() then
		local name 		= g_pEngine:GetClientName(entNum)
		local textSize  = DrawManager:MeasureText(ESPFont, name)
		DrawManager:DrawText(ESPFont, vHeadScreen.X - textSize.Width / 2, vHeadScreen.Y - textSize.Height, name)
	end
end

function DrawBox(pLocalPlayer, pEntity)
	
	SetColor(pLocalPlayer, pEntity)
	
	local vHead 		= GetHeadPosition(pEntity)
	local vOrigin 		= pEntity:GetOrigin()
	
	local vHeadScreen 	= Utils:WorldToScreen(vHead)
	local vOrigScreen 	= Utils:WorldToScreen(vOrigin)
	
	if vHeadScreen:IsValid() and vOrigScreen:IsValid() then
		local iBoxHeight = math.abs(vHeadScreen.Y - vOrigScreen.Y)
		local iBoxWidth = iBoxHeight / 1.8
		
		DrawManager:DrawOutlinedRect(vHeadScreen.X - iBoxWidth / 2, vHeadScreen.Y, iBoxWidth, iBoxHeight, 0xFF000000)
	end
	
end