local love = require('love')
local lovetoys = require("lovetoys.lovetoys")
local log = require('logger.log')

require("identifier.player_bird")
local player_bird = lovetoys.Component.load({_G.__PLAYER_BIRD})
require("graphic.drawable_bird")
local drawable_bird = lovetoys.Component.load({_G.__DRAWABLE_BIRD})
require("common.circle")
local circle = lovetoys.Component.load({_G.__CIRCLE})
require("common.position")
local position = lovetoys.Component.load({_G.__POSITION})
require("physic.physic")
local physic = lovetoys.Component.load({_G.__PHYSIC})

local default_x = 200
local default_y = _G.__WINDOW_HEIGHT / 4
local default_size = 40

local new = function(size, x, y)
  log.info(
    'Creating a bird with', '[size:', size, ']', '[x:', x, ']', '[y:', y, ']')
  local bird = lovetoys.Entity()
  bird:add(player_bird())
  bird:add(drawable_bird())
  local x_bird = x or default_x
  local y_bird = y or default_y
  local r_bird = size or default_size
  bird:add(position(x_bird, y_bird))
  bird:add(circle(r_bird))
  local body = love.physics.newBody(_G.world, x_bird, y_bird, "dynamic")
  local shape = love.physics.newCircleShape(r_bird)
  local fixture = love.physics.newFixture(body, shape, 0)
  body:setMass(1)
  bird:add(physic(body, fixture, shape))
  return bird
end

return setmetatable({},{__index = function(_, _)
                          return new
                   end})
