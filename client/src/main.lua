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
require("graphic/DrawableTwoPipe")
local DrawableTwoPipe = lovetoys.Component.load({"DrawableTwoPipe"})
require("identifier/PlayerBird")
local PlayerBird = lovetoys.Component.load({"PlayerBird"})
require("graphic/DrawableBird")
local DrawableBird = lovetoys.Component.load({"DrawableBird"})
require("common/Circle")
local Circle = lovetoys.Component.load({"Circle"})
require("common/Position")
local Position = lovetoys.Component.load({"Position"})
require("physic/Physic")
local Physic = lovetoys.Component.load({"Physic"})

-- systems
local TwoPipeDrawSystem = require("graphic/TwoPipeDrawSystem")
local BirdDrawSystem = require("graphic/BirdDrawSystem")
local PhysicsPositionSyncSystem = require("physic/PhysicsPositionSyncSystem")
local BirdBehaviorSystem = require("behavior/BirdBehaviorSystem")
local MainKeySystem = require("event/MainKeySystem")
local BirdCameraBeginSystem = require("common/BirdCameraBeginSystem")
local BirdCameraEndSystem = require("common/BirdCameraEndSystem")

-- Events
require("KeyPressed")

local M = {}

function love.load(_)
  love.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {fullscreen=false, vsync=true, resizable=false})
  love.physics.setMeter(ONE_METER_IN_PIXELS)
  M.world = love.physics.newWorld(HORIZONTAL_GRAVITY, VERTICAL_GRAVITY, true)
  M.world:setCallbacks(beginContact, endContact)

  M.engine = lovetoys.Engine()
  M.eventmanager = lovetoys.EventManager()

  local mainkeysystem = MainKeySystem()
  M.eventmanager:addListener("KeyPressed", mainkeysystem, mainkeysystem .fireEvent)

  M.engine:addSystem(BirdCameraBeginSystem())
  M.engine:addSystem(BirdDrawSystem())
  M.engine:addSystem(TwoPipeDrawSystem())
  M.engine:addSystem(BirdCameraEndSystem())
  M.engine:addSystem(PhysicsPositionSyncSystem())
  M.engine:addSystem(BirdBehaviorSystem(WINDOW_HEIGHT, BIRD_SPEED))

  --generate some pipes
  local PIPE_SPACING_BETWEEN_SETS = 200
  local x_last_pipe = 0
  for _ = 1, 20, 1 do
    local two_pipe = lovetoys.Entity()
    local x = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local y = love.math.random(0, WINDOW_HEIGHT)
    two_pipe:add(DrawableTwoPipe(x, y))
    M.engine:addEntity(two_pipe)
    x_last_pipe = x
  end

  --generate the bird
  local bird = lovetoys.Entity()
  bird:add(PlayerBird())
  bird:add(DrawableBird())
  local x_bird, y_bird = 200, WINDOW_HEIGHT / 4
  local r_bird = 40
  bird:add(Position(x_bird, y_bird))
  bird:add(Circle(r_bird))
  local body = love.physics.newBody(M.world, x_bird, y_bird, "dynamic")
  local shape = love.physics.newCircleShape(r_bird)
  local fixture = love.physics.newFixture(body, shape, 0)
  body:setMass(1)
  bird:add(Physic(body, fixture, shape))
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
  M.eventmanager:fireEvent(KeyPressed(key, isrepeat))
end
