local love = require('love')
local pipe_draw_system = class("pipe_draw_system", System)

local pipe_base_image = love.graphics.newImage('assets/sprites/pipes/pipe_green_base.png')
local pipe_top_image = love.graphics.newImage('assets/sprites/pipes/pipe_green_top.png')
local pipe_bottom_image = love.graphics.newImage('assets/sprites/pipes/pipe_green_bottom.png')

PIPE_WIDTH = 40
PIPE_GAP = 230
PIPE_MAX = 5

function pipe_draw_system:draw()
  for index, entity in pairs(self.targets) do
    two_pipe = entity:get(_G.__DRAWABLE_PIPE)
    local offset = love.math.random (-3, 3)
    local coords = {}
    coords.x = two_pipe.x
    coords.y = two_pipe.y
    local part = two_pipe.part
    if (part == "base") then
      love.graphics.draw(pipe_base_image, coords.x, coords.y, 0, 1.5, 1.5)
    end

    if (part == "top") then
      love.graphics.draw(pipe_top_image, coords.x, coords.y, 0, 1.5, 1.5)
    end

    if (part == "bottom") then
      love.graphics.draw(pipe_bottom_image, coords.x, coords.y, 0, 1.5, 1.5)
    end

  end
end

function pipe_draw_system:requires()
  return {_G.__DRAWABLE_PIPE}
end


return pipe_draw_system
