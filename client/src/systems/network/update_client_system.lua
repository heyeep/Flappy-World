local love = require("love")
local inspect = require("inspect.inspect")
-- local class = require("middleclass")
local log = require("logger.log")

local self = class("update_client_system", System)

-- Modules used for creating new Birds.
local lovetoys = require("lovetoys.lovetoys")
local bird_factory = require("bird_factory")

require("identifier.other_bird")
local other_bird = lovetoys.Component.load({_G.__OTHER_BIRD})

-- Hold on to table of Birds. This serves a few purposes.
-- 1. Fast access to the Birds without accessing the Engine.
-- 2. Able to figure out when to add new Birds to the engine (not in the table yet).
-- 3. Fast iteration (side effect of not having to use Engine's looping API).
self.birds = {}

local receive_update_luachan =
  love.thread.getChannel(_G.__CHANNEL_RECEIVE_UPDATE)

self.update = function(_, _)
  -- log.trace('update()')
  local update = receive_update_luachan:pop()
  if not update then
    return
  end

  if update.birds then
    self.handle_birds_update(update.birds)
  end

  if update.coordinates then
    self.handle_coordinates_update(update.coordinates)
  end

  if update.new_player_joined then
    self.handle_new_player_joined_update(update.new_player_joined)
  end

end

self.handle_birds_update = function(birds)
  local my_bird = birds.me
  local other_birds = birds.others

  for _, entity in pairs(_G.engine:getEntitiesWithComponent(_G.__MY_BIRD)) do
    if not entity.server_id then
      entity:get(_G.__PLAYER_BIRD).server_id = my_bird.server_id
    end
  end

  if other_birds then
    for i = 1, #other_birds do
      local bird_json = other_birds[i]
      self.update_or_create_bird(bird_json)
    end
  end
end

self.handle_coordinates_update = function(coords)
  -- log.trace('update.coordinates:', inspect(coords))

  -- Don't update coordinates for MyBird.
  if self.is_update_for_my_bird(coords) then
    return
  end

  self.update_or_create_bird(coords)
end

self.handle_new_player_joined_update = function(new_player)
  -- Don't handle new_player_joined when it's your own bird.
  if self.is_update_for_my_bird(new_player) then
    return
  end

  log.trace("update.new_player_joined:", inspect(new_player))

  self.update_or_create_bird(new_player)
end

-- JSON should have the following keys.
-- server_id, x, y, lv_y, lv_x
-- server_id: Server id on the server.
-- x: X coordinate of Bird.
-- y: Y coordinate of Bird.
-- lv_y: Linear velocity of Bird in Y Direction.
-- lv_x: Linear velocity of Bird in X Direction.
-- Returns the Bird that was updated or created.
self.update_or_create_bird = function(json)
  local server_id = json.server_id
  local x = json.x + _G.__DELAY_COMPENSATION_X
  local y = json.y
  local lv_y = json.lv_y or 0
  local lv_x = json.lv_x or 0

  local bird = self.birds[server_id]

  if bird then
    -- We've seen this Bird before, just update its X and Y.
    bird:get(_G.__PHYSIC).body:setX(x)
    bird:get(_G.__PHYSIC).body:setY(y)
    bird:get(_G.__PHYSIC).body:setLinearVelocity(lv_x, lv_y)
  else
    -- This is a Bird we haven't seen before.
    -- Create the Bird and add it to the Engine.
    bird = bird_factory.new(40, x, y)
    bird:get(_G.__PHYSIC).body:setLinearVelocity(lv_x, lv_y)
    bird:add(other_bird())
    _G.engine:addEntity(bird)
    -- And save it in our bird table.
    self.birds[server_id] = bird
  end

  return bird
end

-- Returns True if the Json update was for MyBird.
self.is_update_for_my_bird = function(json)
  local my_bird_server_id = nil
  for _, entity in pairs(_G.engine:getEntitiesWithComponent(_G.__MY_BIRD)) do
    if not entity.server_id then
      my_bird_server_id = entity:get(_G.__PLAYER_BIRD).server_id
    end
  end

  if json.server_id == my_bird_server_id then
    return true
  end
end

self.requires = function()
  return {_G.__PHYSIC, _G.__POSITION}
end

return self
