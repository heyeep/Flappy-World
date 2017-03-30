local ev = require('ev')
local log = require('logger.log')
log.outfile = 'log.txt'
local love = require('love')
local inspect = require('inspect.inspect')

local ws = require('websockets.phx_socket').new()
local channel_factory = require('websockets.phx_channel')

require('constants')
local push_update_luachan = love.thread.getChannel(_G.__CHANNEL_PUSH_UPDATE)
local receive_update_luachan = love.thread.getChannel(_G.__CHANNEL_RECEIVE_UPDATE)

-- The room will hit RoomChannel on our server.
-- The 1 is hardcoded for now but is intended to be the room id.
local room_phxchan = channel_factory.new(ws, "room:1", nil)

ev.Idle.new(function(loop, idol_event)
    -- We have "bootstapped" into the event loop and therefore do not
    -- care about having this function execute when the event loop is
    -- idol:
    idol_event:stop(loop)

    -- Pretty ghetto colde.
    -- Using a timer to defer execution of the phoenix join to give websockets
    -- some time to connect.
    -- This should be fixed as soon as possible.
    local interval = 2
    local function callback(same_loop, timer_event)
      timer_event:stop(same_loop)

      -- If something prints out here, that is enough to vet that
      -- websockets + phoenix channels works.
      -- Barring any bugs.
      room_phxchan:on_event(
        'hi', function(_, _)
          log.trace('Received hi from server through channels.')
      end)

      local room_push = room_phxchan:join()
      room_push:on_receive(
        'ok', function(message)
          log.info('Received OK, message:', inspect(message))
          room_phxchan:push_event('hi', {})
      end)

      room_push:on_receive(
        'error', function(reason)
          log.error('Couldn\'t join channel, reason:', inspect(reason))
      end)
    end

    local timer = ev.Timer.new(callback, interval, interval)
    timer:start(loop)

    local update_callback = function(_, _)
      local update = push_update_luachan:pop()
      if not update then
        return
      end
      log.info('Pushing event', update.type)
      room_phxchan:push_event(update.type, update.payload)
    end

    room_phxchan:on_event(
      'coordinates', function(message, _)
        log.trace('phxchan on_event (coordinates)', message)
        receive_update_luachan:push(message)
    end)

    -- TODO: This controls how often the update_callback triggers.
    -- As of right now, it's a 3 second poll which is too long.
    local update_timer = ev.Timer.new(update_callback, 3, 3)
    update_timer:start(loop)
end):start(ev.Loop.default)

ws:connect()
