local love = require('love')
-- local class = require('middleclass')
local log = require('logger.log')

local self = class('update_pop_system', System)

local receive_update_luachan = love.thread.getChannel(_G.__CHANNEL_RECEIVE_UPDATE)

self.update = function(_)
  -- log.trace('update()')
  local update = receive_update_luachan:pop()
  if not update then
    return
  end

  local x_coordinate = update.x
  local y_coordinate = update.y
  -- TODO: We hardcoded the payload in main.lua.
  -- to x = 10, y = 20.
  -- If the payload prints out here properly, than 'sync'
  -- can be considered to be working.
  log.info('on_event(coordinates): payload: x:', x_coordinate, 'y:', y_coordinate)
end

self.requires = function()
  return {_G.__PHYSIC, _G.__POSITION}
end

return self
