local love = require('love')
local bird_camera_end_system = class("bird_camera_end_system", System)

function bird_camera_end_system:draw()
  for _, entity in pairs(self.targets) do
  end

  -- This module parallels bird_camera_begin_system so we only need to pop once.
  love.graphics.pop()
end

function bird_camera_end_system:requires()
  return {_G.__PLAYER_BIRD, _G.__POSITION}
end

return bird_camera_end_system
