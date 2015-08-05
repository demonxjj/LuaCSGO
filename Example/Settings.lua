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
	self.bActive = not self.bActive
end

function LuaCSGO:IsActive()
	return self.bActive
end

	