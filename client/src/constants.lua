-- Threads do not share the same _G namespace.
-- As a quick hack, putting constants here that would be used by different threads.

-- Usage: require("constants")

-- Channel used by system to receive updates from web socket layer.
_G.__CHANNEL_RECEIVE_UPDATE = "receive_update"

-- Channel used by system to push updates to the web socket layer.
_G.__CHANNEL_PUSH_UPDATE = "push_update"

-- The time to wait until handling or posting anotehr update.
_G.__TIME_TILL_NEXT_UPDATE = 0.1

-- When testing, it can be good to tweak this value to get the birds to show up at once.
-- Or have really fast hands and get two instances of the client up at once.
-- This will be used to offset other birds by around X amount of pixels.
-- I set it to 200 myself when testing.
_G.__DELAY_COMPENSATION_X = 0
