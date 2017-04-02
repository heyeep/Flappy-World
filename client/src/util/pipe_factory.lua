local love = require("love")
local lovetoys = require("lovetoys.lovetoys")

require("graphic.drawable_pipe")
local drawable_pipe = lovetoys.Component.load({_G.__DRAWABLE_PIPE})

local PIPE_SPACING_BETWEEN_SETS = 300
local PIPE_MAX = 9

local new = function ()
  local pipe_max = PIPE_MAX
  local x_last_pipe = 0
  local y_last_pipe = 0
  local part = _G.__PIPE_PART_BASE
  local time = PIPE_SPACING_BETWEEN_SETS
  while time > 0 do
    local x_pipe = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local PIPE_GAP = love.math.random(-8, 8)

    -- Generate Top
    for numOfPipes = PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = _G.__PIPE_PART_TOP
        y_pipe = y_pipe + 13
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe + 26
    end
    part = _G.__PIPE_PART_BASE

    -- Generate Bottom
    y_last_pipe = _G.__WINDOW_HEIGHT
    for numOfPipes = -PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = _G.__PIPE_PART_BOTTOM
        y_pipe = y_pipe + 7
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe - 26
    end
    part = _G.__PIPE_PART_BASE
    y_last_pipe = 0
    x_last_pipe = x_pipe
    time = time - 1
  end
end

return setmetatable({},{__index = function(_, _)
                          return new
                   end})
