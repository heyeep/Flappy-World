local BirdCameraEndSystem = class("BirdCameraEndSystem", System)

function BirdCameraEndSystem:draw()
  for _, entity in pairs(self.targets) do
    love.graphics.pop()
  end
end

function BirdCameraEndSystem:requires()
  return {"PlayerBird", "Position"}
end

return BirdCameraEndSystem
