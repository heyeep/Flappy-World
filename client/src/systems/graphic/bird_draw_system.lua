local love = require("love")
local bird_draw_system = class("bird_draw_system", System)
local _ = require("logger.log")

local bird_image = love.graphics.newImage("assets/sprites/birds/bird_blue.png")

bird_draw_system.draw = function(self)
 -- love.graphics.setColor(255, 255, 255)
  for _, entity in pairs(self.targets) do
    local r = entity:get(_G.__CIRCLE).r
    local x = entity:get(_G.__POSITION).x
    local y = entity:get(_G.__POSITION).y

    local rect = {}
    rect.x = x - r
    rect.y = y - r
    rect.w = r * 2
    rect.h = r * 2

    local imageSize = {}
    imageSize.width = -(r - (bird_image:getWidth() / 2))
    imageSize.height = -(r - (bird_image:getHeight() / 2))
    
    -- Leaving this here to look at the border around the bird.
    --love.graphics.rectangle("fill", rect.x, rect.y, rect.w, rect.h)

    love.graphics.draw(bird_image, rect.x, rect.y, 0, 1.5, 1.5, imageSize.width, imageSize.height)
  end
end

bird_draw_system.requires = function()
  return {_G.__DRAWABLE_BIRD, _G.__POSITION, _G.__CIRCLE}
end

return bird_draw_system
