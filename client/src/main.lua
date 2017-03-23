local love = require('love')

love.filesystem.setRequirePath(
  require('loadpath') .. love.filesystem.getRequirePath())

local log = require('logger.log')

local lovetoys = require("lovetoys.lovetoys")
lovetoys.initialize({
  globals = true,
  debug = true
})

-- Constants
local WINDOW_WIDTH = 1000
local WINDOW_HEIGHT = 600
local ONE_METER_IN_PIXELS = 64
local HORIZONTAL_GRAVITY = 0
local VERTICAL_GRAVITY = 9.81 * ONE_METER_IN_PIXELS
local BIRD_SPEED = 2

-- components
require("graphic.drawable_two_pipe")
local drawable_two_pipe = lovetoys.Component.load({_G.__DRAWABLE_TWO_PIPE})
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

-- systems
local two_pipe_draw_system = require("graphic.two_pipe_draw_system")
local bird_draw_system = require("graphic.bird_draw_system")
local physics_position_sync_system = require("physic.physics_position_sync_system")
local bird_behavior_system = require("behavior.bird_behavior_system")
local main_key_system = require("event.main_key_system")
local bird_camera_begin_system = require("common.bird_camera_begin_system")
local bird_camera_end_system = require("common.bird_camera_end_system")

-- Events
require("key_pressed")

function love.load(_)
  love.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {fullscreen=false, vsync=true, resizable=false})
  love.physics.setMeter(ONE_METER_IN_PIXELS)
  _G.world = love.physics.newWorld(HORIZONTAL_GRAVITY, VERTICAL_GRAVITY, true)
  _G.world:setCallbacks(beginContact, endContact)

  _G.engine = lovetoys.Engine()
  _G.eventmanager = lovetoys.EventManager()

  local mainkeysystem = main_key_system()
  _G.eventmanager:addListener("key_pressed", mainkeysystem, mainkeysystem .fireEvent)

  _G.engine:addSystem(bird_camera_begin_system())
  _G.engine:addSystem(bird_draw_system())
  _G.engine:addSystem(two_pipe_draw_system())
  _G.engine:addSystem(bird_camera_end_system())
  _G.engine:addSystem(physics_position_sync_system())
  _G.engine:addSystem(bird_behavior_system(WINDOW_HEIGHT, BIRD_SPEED))

  --generate some pipes
  local PIPE_SPACING_BETWEEN_SETS = 200
  local x_last_pipe = 0
  for _ = 1, 20, 1 do
    local two_pipe = lovetoys.Entity()
    local x = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local y = love.math.random(0, WINDOW_HEIGHT)
    two_pipe:add(drawable_two_pipe(x, y))
    _G.engine:addEntity(two_pipe)
    x_last_pipe = x
  end

  --generate the bird
  local bird = lovetoys.Entity()
  bird:add(player_bird())
  bird:add(drawable_bird())
  local x_bird, y_bird = 200, WINDOW_HEIGHT / 4
  local r_bird = 40
  bird:add(position(x_bird, y_bird))
  bird:add(circle(r_bird))
  local body = love.physics.newBody(_G.world, x_bird, y_bird, "dynamic")
  local shape = love.physics.newCircleShape(r_bird)
  local fixture = love.physics.newFixture(body, shape, 0)
  body:setMass(1)
  bird:add(physic(body, fixture, shape))
  _G.engine:addEntity(bird)

  -- This is intended to be a copy pasta of the above so that it's easy to
  -- refactor into a bird factory class later.
  local npc_bird = lovetoys.Entity()
  npc_bird:add(player_bird())
  npc_bird:add(drawable_bird())
  local x_npc_bird, y_npc_bird = 100, WINDOW_HEIGHT / 3
  local r_npc_bird = 40
  npc_bird:add(position(x_npc_bird, y_npc_bird))
  npc_bird:add(circle(r_npc_bird))
  local npc_body = love.physics.newBody(_G.world, x_npc_bird, y_npc_bird, "dynamic")
  local npc_shape = love.physics.newCircleShape(r_npc_bird)
  local npc_fixture = love.physics.newFixture(npc_body, npc_shape, 0)
  body:setMass(1)
  npc_bird:add(physic(npc_body, npc_fixture, npc_shape))
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

  -- TODO: Figure out how to get the coordinates out.
  local update_luachan = love.thread.getChannel('update')
  update_luachan:push({type = 'coordinates', payload = {x = 10, y = 20}})
end

function love.keypressed(key, isrepeat)
  _G.eventmanager:fireEvent(key_pressed(key, isrepeat))
end
