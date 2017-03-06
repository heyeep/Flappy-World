local ev = require('ev')
local log = require('vendor.logger.log')
local love = require('love')
local json = require('cjson')

log.info('Running driver.lua')
local ws = require('vendor.websockets.phx_socket').new()
local channel_factory = require('vendor.websockets.phx_channel')

-- The room will hit RoomChannel on our server.
-- The 1 is hardcoded for now but is intended to be the room id.
local room_channel = channel_factory.new(ws, "room:1", nil)

local update_thread = love.thread.getChannel('update')

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
      room_channel:on_event(
        'hi', function(_, _)
          log.trace('Received hi from server through channels.')
      end)

      local room_push = room_channel:join()
      room_push:on_receive(
        'ok', function(id, message)
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

    local update_callback = function(_, _)
      local update = update_thread:pop()
      if not update then
        return
      end
      log.info('Pushing event', update.type)
      room_channel:push_event(update.type, update.payload)
    end

    room_channel:on_event(
      'coordinates', function(message, _)
        local x_coordinate = message.x
        local y_coordinate = message.y
        -- TODO: We hardcoded the payload in main.lua.
        -- to x = 10, y = 20.
        -- If the payload prints out here properly, than 'sync'
        -- can be considered to be working.
        log.info('on_event(coordinates): payload: x:', x_coordinate, 'y:', y_coordinate)
    end)

    local update_timer = ev.Timer.new(update_callback, 3, 3)
    update_timer:start(loop)
end):start(ev.Loop.default)

ws:connect()
