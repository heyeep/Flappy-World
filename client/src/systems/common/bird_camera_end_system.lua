local love = require("love")
local lovetoys = require("lovetoys.lovetoys")
local class = require("lovetoys.lib.middleclass")
local bird_camera_end_system =
  class("bird_camera_end_system", lovetoys.System)

bird_camera_end_system.draw = function(self)
  for _, _ in pairs(self.targets) do
  end

  -- This module parallels bird_camera_begin_system so we only need to pop once.
  love.graphics.pop()
end

bird_camera_end_system.requires = function()
  return {_G.__PLAYER_BIRD, _G.__POSITION}
end

return bird_camera_end_system
