local love = require("love")
local lovetoys = require("lovetoys.lovetoys")
local class = require("lovetoys.lib.middleclass")
local background_draw_system =
  class("background_draw_system", lovetoys.System)

local background_image = love.graphics.newImage("assets/sprites/backgrounds/basic_day.png")

background_draw_system.draw = function(self)
  for _, entity in pairs(self.targets) do
    local bg = entity:get(_G.__DRAWABLE_BACKGROUND)
    local coords = {}
    coords.x = bg.x
    coords.y = bg.y

    love.graphics.draw(background_image, coords.x, coords.y, 0, 3, 3)
  end
end

background_draw_system.requires = function()
  return {_G.__DRAWABLE_BACKGROUND}
end

return background_draw_system
