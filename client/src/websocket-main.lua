--------------------------------------------------------------------------------
-- Replace contents of this file into main.lua to test out websocket connection.
-- https://love2d.org/forums/viewtopic.php?f=4&t=76670
-- TODO: Remove this file and integrate websockets into app.
--------------------------------------------------------------------------------
-- At this point, all this program does is run the websocket client on another
-- thread and prints out a message once the client is connected.
--------------------------------------------------------------------------------

function love.load(arg)
  if arg[#arg] == "-debug" then require("mobdebug").start() end
  setupWebsocket()
end

function love.update(dt)
  pollWebsocket()
end

function love.draw()
  love.graphics.setColor(20,255,0,255)
  love.graphics.print("Hello", 100, 100)
  printWebsocketMessages();
end

function setupWebsocket()
  thread = love.thread.newThread("pd-websocket.lua")
  channel = love.thread.getChannel("test")
  thread:start()
  i = {}
end

function pollWebsocket()
  v = channel:pop()
  if v then
    table.insert(i, v)
  end
end

function printWebsocketMessages()
  love.graphics.print(tostring(i[1]), 10, 10)
end
