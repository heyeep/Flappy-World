local BirdCameraBeginSystem = class("BirdCameraBeginSystem", System)

function BirdCameraBeginSystem:draw()
  for _, entity in pairs(self.targets) do
    local x = entity:get("Position").x

    love.graphics.push()
    love.graphics.translate(-x + 200, 0)
  end
end

function BirdCameraBeginSystem:requires()
  return {"PlayerBird", "Position"}
end

return BirdCameraBeginSystem
