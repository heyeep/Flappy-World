local bird_camera_begin_system = class("bird_camera_begin_system", System)

function bird_camera_begin_system:draw()
  for _, entity in pairs(self.targets) do
    local x = entity:get(_G.__POSITION).x

    love.graphics.push()
    love.graphics.translate(-x + 200, 0)
  end
end

function bird_camera_begin_system:requires()
  return {_G.__PLAYER_BIRD, _G.__POSITION}
end

return bird_camera_begin_system
