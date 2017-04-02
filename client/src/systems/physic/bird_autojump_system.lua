local lovetoys = require("lovetoys.lovetoys")
local middleclass = require("lovetoys.lib.middleclass")
-- Quick system to make sure birds always "bounce" back up instead of falling.
local sync_system =
  middleclass.class("bird_autojump_system", lovetoys.System)

sync_system.update = function(self, _)
  for _, entity in pairs(self.targets) do
    if entity:get(_G.__PHYSIC).body:getY() > 500 then
      entity:get(_G.__PHYSIC).body:setLinearVelocity(0, -700)
    end
  end
end

sync_system.requires = function()
  return {_G.__PHYSIC, _G.__POSITION, _G.__PLAYER_BIRD}
end

return sync_system
