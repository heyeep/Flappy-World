local love = require('love')
local lovetoys = require("lovetoys.lovetoys")

require("graphic.drawable_pipe")
local drawable_pipe = lovetoys.Component.load({_G.__DRAWABLE_PIPE})

local pipe_draw_system = require("graphic.pipe_draw_system")

local PIPE_SPACING_BETWEEN_SETS = 300
local PIPE_MAX = 9

local new = function (count)
  local pipe_max = PIPE_MAX
  local x_last_pipe = 0
  local y_last_pipe = 0
  local part = "base"
  local time = PIPE_SPACING_BETWEEN_SETS
  --for amount = 1, count, 1 do
  while  time > 0 do
    local x_pipe = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local PIPE_GAP = love.math.random(-8, 8)
    
    -- Generate Top
    for numOfPipes = PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = "top"
        y_pipe = y_pipe + 13
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe + 26
    end
    part = "base"
    
    -- Generate Bottom
    y_last_pipe = _G.__WINDOW_HEIGHT
    for numOfPipes = -PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = "bottom"
        y_pipe = y_pipe + 7
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe - 26
    end
    part = "base"
    y_last_pipe = 0
    x_last_pipe = x_pipe
    
    time = time - 1
  end
end

--[[
local new = function (count)

  local pipe_max = PIPE_MAX
  local x_last_pipe = 0
  local y_last_pipe = 0
  local part = "base"
  local time = PIPE_SPACING_BETWEEN_SETS
  --for amount = 1, count, 1 do
  while  time > 0 do
    local x_pipe = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local PIPE_GAP = love.math.random(-8, 8)
    
    -- Generate Top
    for numOfPipes = PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = "top"
        y_pipe = y_pipe + 13
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe + 26
    end
    part = "base"
    
    -- Generate Bottom
    y_last_pipe = _G.__WINDOW_HEIGHT
    for numOfPipes = -PIPE_GAP, pipe_max, 1 do
      local pipe = lovetoys.Entity()
      local y_pipe = y_last_pipe
      if numOfPipes == PIPE_MAX then
        part = "bottom"
        y_pipe = y_pipe + 7
      end
      pipe:add(drawable_pipe(x_pipe, y_pipe, part))
      _G.engine:addEntity(pipe)
    y_last_pipe = y_pipe - 26
    end
    part = "base"
    y_last_pipe = 0
    x_last_pipe = x_pipe
    
    time = time - 1
  end
end
--]]
return setmetatable({},{__index = function(_, _)
                          return new
                   end})
