local polygon_draw_system = class("polygon_draw_system", System)

function polygon_draw_system:draw()
  love.graphics.setColor(255, 255, 255)
  for index, entity in pairs(self.targets) do
    love.graphics.polygon("fill", entity:get("drawable_polygon").body:getWorldPoints(
      entity:get("drawable_polygon").shape:getPoints()))
  end
end

function polygon_draw_system:requires()
  return {"drawable_polygon"}
end

return polygon_draw_system
