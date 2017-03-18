local bird_camera_begin_system = class("bird_camera_begin_system", System)

function bird_camera_begin_system:draw()
  for _, entity in pairs(self.targets) do
    local x = entity:get("position").x

    love.graphics.push()
    love.graphics.translate(-x + 200, 0)
  end
end

function bird_camera_begin_system:requires()
  return {"player_bird", "position"}
end

return bird_camera_begin_system
