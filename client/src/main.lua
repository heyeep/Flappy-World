local love = require('love')

love.filesystem.setRequirePath(
  require('loadpath') .. love.filesystem.getRequirePath())

local log = require('logger.log')
require('constants')

local lovetoys = require("lovetoys.lovetoys")
lovetoys.initialize({
    globals = true,
    debug = true
})

-- Constants
_G.__WINDOW_WIDTH = 1000
_G.__WINDOW_HEIGHT = 600
_G.__ONE_METER_IN_PIXELS = 64
_G.__HORIZONTAL_GRAVITY = 0
_G.__VERTICAL_GRAVITY = 9.81 * _G.__ONE_METER_IN_PIXELS
_G.__BIRD_SPEED = 2

-- Factory
local bird_factory = require('bird_factory')
local pipe_factory = require('pipe_factory')

-- Components
--require("graphic.drawable_pipe")
--local drawable_pipe = lovetoys.Component.load({_G.__DRAWABLE_PIPE})

require("graphic.drawable_background")
local drawable_background = lovetoys.Component.load({_G.__DRAWABLE_BACKGROUND})

-- Systems
local background_draw_system = require("graphic.background_draw_system")
local pipe_draw_system = require("graphic.pipe_draw_system")
local bird_draw_system = require("graphic.bird_draw_system")
local physics_position_sync_system = require("physic.physics_position_sync_system")
local bird_behavior_system = require("behavior.bird_behavior_system")
local main_key_system = require("event.main_key_system")
local bird_camera_begin_system = require("common.bird_camera_begin_system")
local bird_camera_end_system = require("common.bird_camera_end_system")

local update_push_system = require('network.update_push_system')
local update_pop_system = require('network.update_pop_system')

-- Events
require("key_pressed")

function love.load(_)
  love.window.setMode(_G.__WINDOW_WIDTH,
                      _G.__WINDOW_HEIGHT,
                      {fullscreen=false, vsync=true, resizable=false})

  love.physics.setMeter(_G.__ONE_METER_IN_PIXELS)
  _G.world = love.physics.newWorld(_G.__HORIZONTAL_GRAVITY,
                                   _G.__VERTICAL_GRAVITY,
                                   true)

  _G.world:setCallbacks(beginContact, endContact)

  _G.engine = lovetoys.Engine()
  _G.eventmanager = lovetoys.EventManager()

  local mainkeysystem = main_key_system()
  _G.eventmanager:addListener("key_pressed", mainkeysystem, mainkeysystem .fireEvent)

  _G.engine:addSystem(bird_camera_begin_system())
  _G.engine:addSystem(background_draw_system())
  _G.engine:addSystem(pipe_draw_system())
  _G.engine:addSystem(bird_draw_system())
  _G.engine:addSystem(bird_camera_end_system())
  _G.engine:addSystem(physics_position_sync_system())
  _G.engine:addSystem(bird_behavior_system(_G.__WINDOW_HEIGHT, _G.__BIRD_SPEED))

  _G.engine:addSystem(update_push_system())
  _G.engine:addSystem(update_pop_system())

  -- Generate background
  -- All background images are 144 x 256
  local BACKGROUND_SPACING = 144
  -- Creates two background images before the game "begins", covers the black empty space.
  local x_last_bg = -288
  for _ = 1, 1000, 1 do
    local background = lovetoys.Entity()
    local x = x_last_bg + BACKGROUND_SPACING
    background:add(drawable_background(x, 0))
    _G.engine:addEntity(background)
    x_last_bg = x
  end
  
  -- Generate pipes
  pipe_factory.new(100)
 -- pipe_factory.update(dt)
  
  -- Generate the bird.
  local bird = bird_factory.new(40, 200, _G.__WINDOW_HEIGHT / 4)
  _G.engine:addEntity(bird)

  local npc_bird = bird_factory.new(40, 100, _G.__WINDOW_HEIGHT / 3)
  _G.engine:addEntity(npc_bird)

  local thread = love.thread.newThread("driver.lua")
  thread:start()
end

function love.draw()
  _G.engine:draw()
end

function love.update(dt)
  _G.engine:update(dt)
  _G.world:update(dt)

end

function love.keypressed(key, isrepeat)
  _G.eventmanager:fireEvent(key_pressed(key, isrepeat))
end
