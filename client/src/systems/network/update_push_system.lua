local love = require('love')
-- local class = require('lovetoys.middleclass')
local log = require('logger.log')

local self = class('update_push_system', System)

local push_update_luachan = love.thread.getChannel(_G.__CHANNEL_PUSH_UPDATE)

self.update = function(_)
  -- log.trace('update()')
  push_update_luachan:push(
    {type = 'coordinates', payload = {x = 10, y = 20}})
end

self.requires = function()
  return {_G.__PHYSIC, _G.__POSITION}
end

return self
