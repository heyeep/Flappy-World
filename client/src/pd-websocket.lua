local ev = require'ev'
local ws_client = require('websocket.client').ev()

ws_client:on_open(function()
    c = love.thread.getChannel("test")
    c:push("hi im connected")
    print('connected')
end)

-- ws_client:connect('ws://echo.websocket.org','echo')
ws_client:connect('ws://localhost:4000/socket/websocket', 'echo')

ws_client:on_message(function(ws, msg)
    c = love.thread.getChannel("test")
    c:push("hi on message")
    print('received', msg)
end)

local i = 0

-- ev.Timer.new(function()
--     i = i + 1
--     ws_client:send('hello '..i)
--              end,1,1):start(ev.Loop.default)

ev.Loop.default:loop()
