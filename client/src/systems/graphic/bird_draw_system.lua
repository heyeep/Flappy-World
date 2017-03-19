local love = require('love')
local bird_draw_system = class("bird_draw_system", System)
local _ = require('logger.log')

local bird_image = love.graphics.newImage('assets/flappybird.png')

function bird_draw_system:draw()
  love.graphics.setColor(255, 255, 255)
  for _, entity in pairs(self.targets) do
    local r = entity:get(_G.__CIRCLE).r
    local x = entity:get(_G.__POSITION).x
    local y = entity:get(_G.__POSITION).y

    local rect = {}
    rect.x = x - r
    rect.y = y - r
    rect.w = r * 2
    rect.h = r * 2

    -- Leaving this here to look at the border around the bird.
    love.graphics.rectangle("fill", rect.x, rect.y, rect.w, rect.h)

    love.graphics.draw(bird_image, rect.x, rect.y, 0, 0.1, 0.1)
  end
end

function bird_draw_system:requires()
  return {_G.__DRAWABLE_BIRD, _G.__POSITION, _G.__CIRCLE}
end

return bird_draw_system
