local love = require('love')
local pipe_draw_system = class("pipe_draw_system", System)

local pipe_images = {
  [_G.__PIPE_PART_BASE] = love.graphics.newImage('assets/sprites/pipes/pipe_green_base.png'),
  [_G.__PIPE_PART_TOP] = love.graphics.newImage('assets/sprites/pipes/pipe_green_top.png'),
  [_G.__PIPE_PART_BOTTOM] = love.graphics.newImage('assets/sprites/pipes/pipe_green_bottom.png'),
}


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
