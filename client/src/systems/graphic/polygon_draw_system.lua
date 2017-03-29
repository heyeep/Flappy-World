local polygon_draw_system = class("polygon_draw_system", System)

polygon_draw_system.draw = function()
  love.graphics.setColor(255, 255, 255)
  for index, entity in pairs(self.targets) do
    love.graphics.polygon("fill", entity:get("drawable_polygon").body:getWorldPoints(
      entity:get("drawable_polygon").shape:getPoints()))
  end
end

polygon_draw_system.requires = function()
  return {"drawable_polygon"}
end

return polygon_draw_system
