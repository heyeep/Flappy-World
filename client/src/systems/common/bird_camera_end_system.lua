local bird_camera_end_system = class("bird_camera_end_system", System)

function bird_camera_end_system:draw()
  for _, entity in pairs(self.targets) do
    love.graphics.pop()
  end
end

function bird_camera_end_system:requires()
  return {"player_bird", "position"}
end

return bird_camera_end_system
