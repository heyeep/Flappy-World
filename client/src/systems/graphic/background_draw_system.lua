local love = require('love')
local background_draw_system = class("background_draw_system", System)

local background_image = love.graphics.newImage('assets/sprites/backgrounds/basic_day.png')

function background_draw_system:draw()
  for index, entity in pairs(self.targets) do
    local bg = entity:get(_G.__DRAWABLE_BACKGROUND)
    local coords = {}
    coords.x = bg.x
    coords.y = bg.y
    
    love.graphics.draw(background_image, coords.x, coords.y, 0, 3, 3)
  end
end

function background_draw_system:requires()
  return {_G.__DRAWABLE_BACKGROUND}
end

return background_draw_system
