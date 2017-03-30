local new = function(ws_url, hb_interval)
  local log = require('logger.log')
  local ev = require('ev')
  local self = {}
  local json = require('cjson')
  
  -- Instance Variables / Begin

  self.url = ws_url or 'ws://localhost:4000/socket/websocket'
  self.heart_beat_interval = hb_interval or 5
  self.params = {}
  self.channels = {}
  self.open_callbacks = {}
  self.close_callbacks = {}
  self.error_callbacks = {}
  self.message_callbacks = {}
  self.reconnect_on_error = true
  self.queue = {}

  self.reconnect_interval = 5
  self.send_buffer_timer = nil
  self.reconnect_timer = nil
  self.heartbeat_timer = nil

  math.randomseed(os.time())
  self.ref = math.random()

  -- Instance Variables / End

  self.connect = function(_, parameters)
    log.trace(string.format('connect(), parameters : %s', parameters))
    local url = self.url
    local params = parameters or self.parameters
    if (params) then
      -- TODO: Put parameters into Url string.
      self.url = url
    end

    log.info('Connecting to websocket at', self.url)
    self.ws_client = require('websocket.client').ev()

    -- Websocket Callbacks / Begin

    self.ws_client:on_open(function(_, on_open_arg)
        log.info('on_open()', 'on_open_arg:', on_open_arg)
        self:on_conn_open()
    end)

    self.ws_client:on_message(function(_, on_message_arg)
        log.info('on_message()', 'on_message_arg:', on_message_arg)
        self:on_conn_message(on_message_arg)
    end)

    self.ws_client:on_close(function(_, on_close_arg)
        self:on_conn_close(on_close_arg)
    end)

    self.ws_client:on_error(function(_, on_error_arg)
        self:on_conn_error(on_error_arg)
    end)

    -- Websocket Callbacks / End

    self.ws_client:connect(self.url, 'echo')
    ev.Loop.default:loop()
  end

  self.disconnect = function(_)
    log.trace('disconnect()')
    self:discard_heartbeat_timer()
    self:discard_reconnect_timer()
    self:disconnect_socket()
  end

  self.reconnect = function(_)
    log.trace('reconnect()')
    self:disconnect_socket()
    self:connect(self.params)
  end

  self.is_connected = function(_)
    log.trace('is_connected()')
    return self.ws_client.state == 'OPEN'
  end

  self.socket_state = function(_)
    -- Meh, we probably don't need this.
    -- It's easy to pull it out of the client.
    -- OPEN | CLOSED | CLOSING | CONNECTING
    return self.ws_client.state
  end

  self.add_channel = function(_, phx_channel)
    log.trace(string.format('add_channel(), phx_channel: %s', phx_channel))
    -- Remember, indices start at 1.
    self.channels[#self.channels + 1] = phx_channel
  end

  self.remove_channel = function(_, phx_channel)
    log.trace(string.format('remove_channel(), phx_channel: %s', phx_channel))

    local channels = self.channels
    local index_of_channel = 0
    -- Just iterating through and finding the index of the channel.
    for i = 1, #channels do
      if (channels[i] == phx_channel) then
        index_of_channel = i
        break
      end
    end

    -- If index is greater than 0, we found a channel existing in the list.
    if (index_of_channel > 0) then
      -- Maybe have to think about reordering the list.
      -- Probably not.
      self.channels[index_of_channel] = nil
    end
  end

  -- Phoenix Callbacks / Begin
  -- These sets of functions will get called by a phx_channel object.
  -- You can think of of these functions as the listener interface to the channel.
  -- The objc callbacks here typedef the callback so it probably returns more
  -- values than just a simple callback function.
  self.phx_on_open = function(_, callback)
    self.open_callbacks[#self.open_callbacks + 1] = callback
  end

  self.phx_on_close = function(_, callback)
    self.close_callbacks[#self.close_callbacks + 1] = callback
  end

  self.phx_on_error = function(_, callback)
    self.error_callbacks[#self.error_callbacks + 1] = callback
  end

  self.phx_on_message = function(_, callback)
    self.message_callbacks[#self.message_callbacks + 1] = callback
  end

  -- Phoenix Callbacks / End

  self.push = function(_, data)
    -- The magic 1 is an opcode. I think 1 means it's text.
    -- The iOS client pushes this call into another thread
    -- using NSOperationQueue. We might want to do that too.
    local json_data = json.encode(data)
    log.trace('phx_socket: push():', json_data)
    self.ws_client:send(json_data, 1)
  end

  -- Private Methods / Begin

  self.trigger_channel_error = function(_, error)
    log.trace(string.format('trigger_channel_error(), error: %s', error))
    for i = 1, #self.channels do
      local phx_channel = self.channels[i]
      phx_channel:trigger_event('phx_error', error, nil)

    end
  end

  self.start_heartbeat_timer_with_interval = function(_, interval)
    self.heart_beat_interval = interval

    local heartbeat_timer_cb = function(_, _)
      self:send_heartbeat()
    end

    self.heartbeat_timer = ev.Timer.new(heartbeat_timer_cb, interval, interval)
    self.heartbeat_timer:start(ev.Loop.default)
  end

  self.disconnect_socket = function(_)
    log.info('disconnect_socket()')
    if (self.ws_client.state == 'OPEN') then
      self.ws_client.on_open(nil)
      self.ws_client.on_close(nil)
      self.ws_client.on_error(nil)
      self.ws_client.on_message(nil)
      self.ws_client:close(nil, nil, nil)
      self.ws_client = nil
    end
  end

  self.discard_heartbeat_timer = function(_)
    log.trace('discard_heartbeat_timer()')
    if (self.heartbeat_timer) then
      self.heartbeat_timer:stop(ev.Loop.default)
      self.heartbeat_timer = nil
    end
  end

  self.discard_reconnect_timer = function(_)
    log.trace('discard_reconnect_timer()')
    if (self.reconnect_timer) then
      self.reconnect_timer:stop(ev.Loop.default)
      self.reconnect_timer = nil
    end
  end

  self.on_conn_open = function(_)
    log.info('on_conn_open()')
    --     [self.queue setSuspended:NO];

    self:discard_reconnect_timer()

    if (self.heart_beat_interval > 0) then
      self:start_heartbeat_timer_with_interval(self.heart_beat_interval)
    end

    for i = 1, #self.open_callbacks do
      local on_open_callback = self.open_callbacks[i]
      on_open_callback()
    end

    --     if ([self.delegate respondsToSelector:@selector(phxSocketDidOpen)]) {
    --         [self.delegate phxSocketDidOpen];
    --     }
  end

  self.on_conn_close = function(_, event)
    log.info('on_conn_close()')
    --     [self.queue setSuspended:YES];

    self:trigger_channel_error(event)

    if (self.reconnect_on_error) then
      self:discard_reconnect_timer()

      local reconnect_cb = function(_, _)
        self:reconnect()
      end

      local interval = self.reconnect_interval
      self.reconnect_timer = ev.Timer.new(reconnect_cb, interval, interval)
      self.reconnect_timer:start(ev.Loop.default)
    end

    self:discard_heartbeat_timer()

    for i = 1, #self.close_callbacks do
      log.info('Calling on_close_callback.')
      local on_close_callback = self.close_callbacks[i]
      on_close_callback(event)
    end

    --     if ([self.delegate respondsToSelector:@selector(phxSocketDidClose:)]) {
    --         [self.delegate phxSocketDidClose:event];
    --     }
  end

  self.on_conn_error = function(_, error)
    log.error('on_conn_error(): ', error)
    --     [self.queue setSuspended:YES];
    if (self.heartbeat_timer) then
      self:discard_heartbeat_timer()
      self.heartbeat_timer = nil
    end

    for i = 1, #self.error_callbacks do
      local on_error_callback = self.error_callbacks[i]
      on_error_callback(error)
    end

    --     if ([self.delegate respondsToSelector:@selector(phxSocketDidReceiveError:)]) {
    --         [self.delegate phxSocketDidReceiveError:error];
    --     }

    self:on_conn_close(error)
  end

  self.on_conn_message = function(_, message)
    log.trace('phx_socket: on_conn_message():', message)
    local json_data = json.decode(message)
    local topic = json_data.topic
    local event = json_data.event
    local payload = json_data.payload
    local ref = json_data.ref

    for i = 1, #self.channels do
      local chan = self.channels[i]
      if (chan.topic == topic) then
        chan:trigger_event(event, payload, ref)
      end
    end

    for i = 1, #self.message_callbacks do
      local on_message_callback = self.message_callbacks[i]
      log.info('on_message_callback()')
      on_message_callback(message)
    end
  end

  self.send_heartbeat = function(_)
    log.trace('send_heartbeat()')
    local json_map = {}
    json_map.topic = 'phoenix'
    json_map.event = 'heartbeat'
    json_map.payload = {}
    json_map.ref = self:make_ref()
    self:push(json_map)
  end

  self.make_ref = function(_)
    self.ref = self.ref + 1
    log.trace('make_ref()', self.ref)
    return self.ref
  end

  -- Private Methods / End
  return self
end

-- As far as I can tell, this is the way to get this syntax to work.
-- local ws = require('vendor/websockets/phx_socket').new()
-- Copied from client.lua in websockets library.
return setmetatable({},{__index = function(_, _)
                          return new
                   end})
