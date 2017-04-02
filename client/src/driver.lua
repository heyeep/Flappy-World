local ev = require('ev')
local log = require('logger.log')
local love = require('love')
local inspect = require('inspect.inspect')

require('constants')
local push_update_luachan = love.thread.getChannel(_G.__CHANNEL_PUSH_UPDATE)
local receive_update_luachan = love.thread.getChannel(_G.__CHANNEL_RECEIVE_UPDATE)

local self = {}

local ws = require('websockets.phx_socket').new(nil, nil, self)
local channel_factory = require('websockets.phx_channel')

-- The room will hit RoomChannel on our server.
-- The 1 is hardcoded for now but is intended to be the room id.
local room_phxchan = channel_factory.new(ws, "room:1", nil)

-- Websocket Callbacks
-- Kicks off bootstrapping and joining of the Phoenix Channel.

self.on_socket_open = function(_)
  log.info('on_socket_open')
  local room_push = room_phxchan:join()
  room_push
    :on_receive('ok', function(message)
                  log.info('Received OK, message:', inspect(message))
                  receive_update_luachan:push({birds = message})
               end)
    :on_receive('error', function(reason)
                  log.error('Couldn\'t join channel, reason:', inspect(reason))
               end)

  room_phxchan:on_event(
    'coordinates', function(message, _)
      log.trace('phxchan on_event (coordinates)', message)
      receive_update_luachan:push({coordinates = message})
  end)

  room_phxchan:on_event(
    'new_player_joined', function(message, _)
      log.trace('phxchan on_event(new_player_joined)', message)
      receive_update_luachan:push({new_player_joined = message})
  end)
end

self.on_socket_close = function(_, event)
  log.info('on_socket_close event:', event)
end

self.on_socket_error = function(_, error)
  log.info('on_socket_error error:', error)
end

-- Sets up communication layer between Game and Websockets.
ev.Idle.new(function(loop, idol_event)
    -- We have "bootstapped" into the event loop and therefore do not
    -- care about having this function execute when the event loop is
    -- idol:
    idol_event:stop(loop)

    local update_callback = function(_, _)
      local update = push_update_luachan:pop()
      if not update then
        return
      end
      log.info('Pushing event', update.type)
      room_phxchan:push_event(update.type, update.payload)
    end

    local update_timer = ev.Timer.new(update_callback,
                                      _G.__TIME_TILL_NEXT_UPDATE,
                                      _G.__TIME_TILL_NEXT_UPDATE)
    update_timer:start(loop)
end):start(ev.Loop.default)

ws:connect()
