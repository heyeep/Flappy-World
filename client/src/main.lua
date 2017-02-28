lovetoys = require("lib.lovetoys.lovetoys")
lovetoys.initialize({
  globals = true,
  debug = true
})

WINDOW_WIDTH = 1000
WINDOW_HEIGHT = 600

ONE_METER_IN_PIXELS = 64
HORIZONTAL_GRAVITY = 0
VERTICAL_GRAVITY = 9.81 * ONE_METER_IN_PIXELS

BIRD_SPEED = 2

-- components
require("components/graphic/DrawableTwoPipe")
local DrawableTwoPipe = Component.load({"DrawableTwoPipe"})
require("components/identifier/PlayerBird")
local PlayerBird = Component.load({"PlayerBird"})
require("components/graphic/DrawableBird")
local DrawableBird = Component.load({"DrawableBird"})
require("components/common/Circle")
local Circle = Component.load({"Circle"})
require("components/common/Position")
local Position = Component.load({"Position"})
require("components/physic/Physic")
local Physic = Component.load({"Physic"})

-- systems
local TwoPipeDrawSystem = require("systems/graphic/TwoPipeDrawSystem")
local BirdDrawSystem = require("systems/graphic/BirdDrawSystem")
local PhysicsPositionSyncSystem = require("systems/physic/PhysicsPositionSyncSystem")
local BirdBehaviorSystem = require("systems/behavior/BirdBehaviorSystem")
local MainKeySystem = require("systems/event/MainKeySystem")
local BirdCameraBeginSystem = require("systems/common/BirdCameraBeginSystem")
local BirdCameraEndSystem = require("systems/common/BirdCameraEndSystem")

-- Events
require("events/KeyPressed")

function love.load(arg)
  love.window.setMode(WINDOW_WIDTH, WINDOW_HEIGHT, {fullscreen=false, vsync=true, resizable=false})
  love.physics.setMeter(ONE_METER_IN_PIXELS)
  world = love.physics.newWorld(HORIZONTAL_GRAVITY, VERTICAL_GRAVITY, true)
  world:setCallbacks(beginContact, endContact)

  engine = Engine()
  eventmanager = EventManager()

  mainkeysystem = MainKeySystem()
  eventmanager:addListener("KeyPressed", mainkeysystem, mainkeysystem .fireEvent)

  engine:addSystem(BirdCameraBeginSystem())
  engine:addSystem(BirdDrawSystem())
  engine:addSystem(TwoPipeDrawSystem())
  engine:addSystem(BirdCameraEndSystem())
  engine:addSystem(PhysicsPositionSyncSystem())
  engine:addSystem(BirdBehaviorSystem(WINDOW_HEIGHT, BIRD_SPEED))

  --generate some pipes
  local PIPE_SPACING_BETWEEN_SETS = 200
  local x_last_pipe = 0
  for i = 1, 20, 1 do
    local two_pipe = Entity()
    local x = x_last_pipe + PIPE_SPACING_BETWEEN_SETS
    local y = love.math.random(0, WINDOW_HEIGHT)
    two_pipe:add(DrawableTwoPipe(x, y))
    engine:addEntity(two_pipe)
    x_last_pipe = x
  end

  --generate the bird
  local bird = Entity()
  bird:add(PlayerBird())
  bird:add(DrawableBird())
  local x_bird, y_bird = 200, WINDOW_HEIGHT / 4
  local r_bird = 40
  bird:add(Position(x_bird, y_bird))
  bird:add(Circle(r_bird))
  local body = love.physics.newBody(world, x_bird, y_bird, "dynamic")
  local shape = love.physics.newCircleShape(r_bird)
  local fixture = love.physics.newFixture(body, shape, 0)
  body:setMass(1)
  bird:add(Physic(body, fixture, shape))
  engine:addEntity(bird)
end

function love.draw()
  engine:draw()
end

function love.update(dt) 
  engine:update(dt)
  world:update(dt)
end

function love.keypressed(key, isrepeat)
  eventmanager:fireEvent(KeyPressed(key, isrepeat))
end
