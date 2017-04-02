local love = require("love")
local lovetoys = require("lovetoys.lovetoys")
local middleclass = require("lovetoys.lib.middleclass")
local polygon_draw_system =
  middleclass.class("polygon_draw_system", lovetoys.System)

polygon_draw_system.draw = function(self)
  love.graphics.setColor(255, 255, 255)
  for _, entity in pairs(self.targets) do
    love.graphics.polygon(
      "fill", entity:get("drawable_polygon").body:getWorldPoints(
        entity:get("drawable_polygon").shape:getPoints()))
  end
end

polygon_draw_system.requires = function()
  return {"drawable_polygon"}
end

return polygon_draw_system
