local ev = require('ev')
local log = require('vendor.logger.log')

log.info('Running driver.lua')
local ws = require('vendor.websockets.phx_socket').new()
local channel_factory = require('vendor.websockets.phx_channel')

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

      -- The room will hit RoomChannel on our server.
      -- The 1 is hardcoded for now but is intended to be the room id.
      local room_channel = channel_factory.new(ws, "room:1", nil)

      -- If something prints out here, that is enough to vet that
      -- websockets + phoenix channels works.
      -- Barring any bugs.
      room_channel:on_event(
        'hi', function(_, _)
          log.trace('Received hi from server through channels.')
      end)

      local room_push = room_channel:join()
      room_push:on_receive(
        "ok", function(id, message)
          log.info('Received OK, id:', id, 'message:', message)
          room_channel:push_event('hi', {})
      end)

      room_push:on_receive(
        'error', function(id, reason)
          log.error('Couldn\'t join channel id:', id, 'reason:', reason)
      end)
    end

    local timer = ev.Timer.new(callback, interval, interval)
    timer:start(loop)
end):start(ev.Loop.default)

ws:connect()
