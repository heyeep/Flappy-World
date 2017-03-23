local love = require('love')
local bird_camera_begin_system = class("bird_camera_begin_system", System)

function bird_camera_begin_system:draw()
  -- Iterate through the list of player_birds we have and grab the position of
  -- one of them. We use that position to set up the x offset of the bird
  -- relative to the left side of the screen
  -- (e.g. How far away is the bird from the left side of the screen?)
  local player_bird_x = 0
  for _, entity in pairs(self.targets) do
    player_bird_x = entity:get(_G.__POSITION).x
  end

  love.graphics.push()
  love.graphics.translate(-player_bird_x + 200, 0)
end

function bird_camera_begin_system:requires()
  return {_G.__PLAYER_BIRD, _G.__POSITION}
end

return bird_camera_begin_system
