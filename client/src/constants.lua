-- Threads do not share the same _G namespace.
-- As a quick hack, putting constants here that would be used by different threads.

-- Usage: require('constants')

-- Channel used by system to receive updates from web socket layer.
_G.__CHANNEL_RECEIVE_UPDATE = 'receive_update'

-- Channel used by system to push updates to the web socket layer.
_G.__CHANNEL_PUSH_UPDATE = 'push_update'
