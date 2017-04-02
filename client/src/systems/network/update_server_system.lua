local love = require("love")
-- local class = require("lovetoys.middleclass")
local log = require("logger.log")

local self = class("update_server_system", System)

local push_update_luachan = love.thread.getChannel(_G.__CHANNEL_PUSH_UPDATE)

self.time = 0

self.update = function(_, dt)
  self.time = self.time + dt

  -- Block updates until appropriate amount of time has passed.
  if self.time < _G.__TIME_TILL_NEXT_UPDATE then
    return
  end

  self.time = self.time - _G.__TIME_TILL_NEXT_UPDATE

  -- log.trace("update: elapsed:", self.elapsed_time)

  for _, entity in pairs(_G.engine:getEntitiesWithComponent(_G.__MY_BIRD)) do
    local my_bird = entity

    local server_id = entity:get(_G.__PLAYER_BIRD).server_id
    local x = my_bird:get(_G.__PHYSIC).body:getX()
    local y = my_bird:get(_G.__PHYSIC).body:getY()
    local lv_x, lv_y =
      my_bird:get(_G.__PHYSIC).body:getLinearVelocity()

    -- Only push coordinate updates if bird has server_id set.
    if server_id then
      push_update_luachan:push(
        {type = "coordinates",
         payload = {server_id = server_id,
                    x = x,
                    y = y,
                    lv_x = lv_x,
                    lv_y = lv_y
      }})
    end
  end
end

self.requires = function()
  return {_G.__PHYSIC, _G.__POSITION}
end

return self
