local love = require("love")
local lovetoys = require("lovetoys.lovetoys")
local middleclass = require("lovetoys.lib.middleclass")
local bird_camera_begin_system =
  middleclass.class("bird_camera_begin_system", lovetoys.System)

bird_camera_begin_system.draw = function(self)
  -- Iterate through the list of player_birds we have and grab the position of
  -- one of them. We use that position to set up the x offset of the bird
  -- relative to the left side of the screen
  -- (e.g. How far away is the bird from the left side of the screen?)
  local my_bird_x = 0
  for _, entity in pairs(self.targets) do
    my_bird_x = entity:get(_G.__POSITION).x
  end

  love.graphics.push()
  love.graphics.translate(-my_bird_x + 200, 0)
end

bird_camera_begin_system.requires = function()
  return {_G.__MY_BIRD, _G.__POSITION}
end

return bird_camera_begin_system
