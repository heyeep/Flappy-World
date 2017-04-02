local lovetoys = require("lovetoys.lovetoys")
local middleclass = require("lovetoys.lib.middleclass")
local bird_behavior_system =
  middleclass.class("bird_behavior_system", lovetoys.System)

bird_behavior_system.initialize = function(self, world_height, bird_speed)
  lovetoys.System.initialize(self)
  self.world_height = world_height
  self.bird_speed = bird_speed
end

bird_behavior_system.draw = function(self)
  for _, entity in pairs(self.targets) do
    local body = entity:get(_G.__PHYSIC).body
    body:setX(body:getX() + self.bird_speed)

    if body:getY() > self.world_height then
      body:setY(0)
    end
  end
end

bird_behavior_system.requires = function()
  return {_G.__PLAYER_BIRD, _G.__PHYSIC}
end

return bird_behavior_system
