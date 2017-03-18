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
require("graphic/drawable_two_pipe")
local drawable_two_pipe = lovetoys.Component.load({"drawable_two_pipe"})
require("identifier/player_bird")
local player_bird = lovetoys.Component.load({"player_bird"})
require("graphic/drawable_bird")
local drawable_bird = lovetoys.Component.load({"drawable_bird"})
require("common/circle")
local circle = lovetoys.Component.load({"circle"})
require("common/position")
local position = lovetoys.Component.load({"position"})
require("physic/physic")
local physic = lovetoys.Component.load({"physic"})

-- systems
local two_pipe_draw_system = require("graphic/two_pipe_draw_system")
local bird_draw_system = require("graphic/bird_draw_system")
local physics_position_sync_system = require("physic/physics_position_sync_system")
local bird_behavior_system = require("behavior/bird_behavior_system")
local main_key_system = require("event/main_key_system")
local bird_camera_begin_system = require("common/bird_camera_begin_system")
local bird_camera_end_system = require("common/bird_camera_end_system")

-- Events
require("key_pressed")

local M = {}

function love.load(_)
  love.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {fullscreen=false, vsync=true, resizable=false})
  love.physics.setMeter(ONE_METER_IN_PIXELS)
  M.world = love.physics.newWorld(HORIZONTAL_GRAVITY, VERTICAL_GRAVITY, true)
  M.world:setCallbacks(beginContact, endContact)

  M.engine = lovetoys.Engine()
  M.eventmanager = lovetoys.EventManager()

  local mainkeysystem = main_key_system()
  M.eventmanager:addListener("key_pressed", mainkeysystem, mainkeysystem .fireEvent)

  M.engine:addSystem(bird_camera_begin_system())
  M.engine:addSystem(bird_draw_system())
  M.engine:addSystem(two_pipe_draw_system())
  M.engine:addSystem(bird_camera_end_system())
  M.engine:addSystem(physics_position_sync_system())
  M.engine:addSystem(bird_behavior_system(WINDOW_HEIGHT, BIRD_SPEED))

  --generate some pipes
  local PIPE_SPACING_BETWEEN_SETS = 200
  local x_last_pipe = 0
  for _ = 1, 20, 1 do
    local two_pipe = lovetoys.Entity()
    local x = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local y = love.math.random(0, WINDOW_HEIGHT)
    two_pipe:add(drawable_two_pipe(x, y))
    M.engine:addEntity(two_pipe)
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
  local body = love.physics.newBody(M.world, x_bird, y_bird, "dynamic")
  local shape = love.physics.newCircleShape(r_bird)
  local fixture = love.physics.newFixture(body, shape, 0)
  body:setMass(1)
  bird:add(physic(body, fixture, shape))
  M.engine:addEntity(bird)

  local thread = love.thread.newThread("driver.lua")
  thread:start()
end

function love.draw()
  M.engine:draw()
end

function love.update(dt)
  M.engine:update(dt)
  M.world:update(dt)

  -- TODO: Figure out how to get the coordinates out.
  local update_luachan = love.thread.getChannel('update')
  -- update_luachan:push({type = 'coordinates', payload = {x = 10, y = 20}})
end

function love.keypressed(key, isrepeat)
  M.eventmanager:fireEvent(key_pressed(key, isrepeat))
end
