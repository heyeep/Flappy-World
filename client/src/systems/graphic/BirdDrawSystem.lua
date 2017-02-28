local BirdDrawSystem = class("BirdDrawSystem", System)

function BirdDrawSystem:draw()
  love.graphics.setColor(0, 255, 0)
  for index, entity in pairs(self.targets) do
    local r = entity:get("Circle").r
    local x = entity:get("Position").x
    local y = entity:get("Position").y

    local rect = {}
    rect.x = x - r
    rect.y = y - r
    rect.w = r * 2
    rect.h = r * 2

    print("bird position", x, y, r, rect.y, rect.h)
    love.graphics.setColor(0, 255, 255)
    love.graphics.rectangle("fill", rect.x, rect.y, rect.w, rect.h)
  end
end

function BirdDrawSystem:requires()
  return {"DrawableBird", "Position", "Circle"}
end

return BirdDrawSystem
