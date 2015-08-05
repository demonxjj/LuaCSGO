--[[
	Settings file.
--]]

require("LuaCSGO/class")

LuaCSGO = class(function(instance,active)
	instance.bActive = active
end)

function LuaCSGO:Disable()
	self.bActive = false
end

function LuaCSGO:Enable()
	self.bActive = true
end

function LuaCSGO:Toggle()
	if self.IsActive() then self.Disable() else self.Enable() end
end

function LuaCSGO:IsActive()
	return self.bActive
end

	