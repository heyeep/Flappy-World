local bird_draw_system = class("bird_draw_system", System)

function bird_draw_system:draw()
  love.graphics.setColor(0, 255, 0)
  for index, entity in pairs(self.targets) do
    local r = entity:get("circle").r
    local x = entity:get("position").x
    local y = entity:get("position").y

    local rect = {}
    rect.x = x - r
    rect.y = y - r
    rect.w = r * 2
    rect.h = r * 2

    -- print("bird position", x, y, r, rect.y, rect.h)
    love.graphics.setColor(0, 255, 255)
    love.graphics.rectangle("fill", rect.x, rect.y, rect.w, rect.h)
  end
end

function bird_draw_system:requires()
  return {"drawable_bird", "position", "circle"}
end

return bird_draw_system
