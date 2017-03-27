local love = require('love')
local pipe_draw_system = class("pipe_draw_system", System)

local pipe_images = {
  [0] = love.graphics.newImage('assets/sprites/pipes/pipe_green_base.png'),
  [1] = love.graphics.newImage('assets/sprites/pipes/pipe_green_top.png'),
  [2] = love.graphics.newImage('assets/sprites/pipes/pipe_green_bottom.png'),
}

local pipe_image
PIPE_WIDTH = 40
PIPE_GAP = 230
PIPE_MAX = 5

function pipe_draw_system:draw()
  for index, entity in pairs(self.targets) do
    pipe = entity:get(_G.__DRAWABLE_PIPE)
    love.graphics.draw(pipe_images[pipe.part], pipe.x, pipe.y, 0, 1.5, 1.5)
  end
end

function pipe_draw_system:requires()
  return {_G.__DRAWABLE_PIPE}
end


return pipe_draw_system
