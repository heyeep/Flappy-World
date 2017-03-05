local new = function(socket, topic, params)
  local log = require('vendor.logger.log')
  local self = {}
  local push_factory = require('vendor.websockets.phx_push')
  self.state = 'CHANNEL_CLOSED'
  self.topic = topic
  self.params = params or {}
  self.socket = socket
  self.bindings = {}
  self.socket:add_channel(self)
  self.joined_once = false

  self.on_event = function(_, event, callback)
    log.trace(string.format('on_event(), event: %s', event))
    self.bindings[#self.bindings + 1] = { event = event, callback = callback }
  end

  self.on_close = function(_, callback)
    self:on_event("phx_close", function(message, _)
                    callback(message)
    end)
  end

  self.on_error = function(_, callback)
    self:on_event("phx_error", function(err, _)
                    callback(err)
    end)
  end

  self.trigger_event = function(_, event, message, ref)
    log.trace(string.format('topic: %s, binding count is %s', self.topic, #self.bindings))
    local bindings_list = self.bindings
    for i = 1, #bindings_list do
      local bnding = bindings_list[i]
      if not bnding then
        -- This used to be a problem until switching to operating on a copy
        -- of self.bindings. Keeping it here to see if this continues happening.
        log.fatal('binding is nil.')
      else
        if (bnding.event == event) then
          local on_receive_callback = bnding.callback
          on_receive_callback(message, ref)
        end
      end
    end
  end

  self.reply_event_name = function(_, ref)
    return string.format("chan_reply_%s", ref)
  end

  self.off_event = function(_, event)
    log.trace('off_event()', event)
    local filtered_bindings = {}
    local bindings_copy = self.bindings
    for i = 1, #bindings_copy do
      local bnding = bindings_copy[i]
      if (bnding.event ~= event) then
        filtered_bindings[#filtered_bindings + 1] = bnding
      end
    end
    self.bindings = filtered_bindings
  end

  self.is_member_of_topic = function(_, p_topic)
    return self.topic == p_topic
  end

  self.join = function(_) -- PhxPush
    if (self.joined_once) then
      -- ERROR
      log.error('Already joined.')
    else
      self.joined_once = true
    end

    self:send_join()
    return self.join_push
  end

  self.rejoin = function(_)
    if (self.joined_once and self.state ~= 'CHANNEL_JOINING' and
        self.state ~= 'CHANNEL_JOINED') then
      self:send_join()
    end
  end

  self.send_join = function(_)
    log.info('send_join()')
    self.state = 'CHANNEL_JOINING'
    self.join_push.payload = self.params
    self.join_push:send()
  end

  self.leave = function(_)
    self.state = 'CHANNEL_CLOSED'
    self:push_event("phx_leave", {})
      :on_receive("ok",
                  function (_)
                    self:trigger_event("phx_close", "leave", nil)
                 end)
  end

  self.push_event = function(_, event, payload) -- PhxPush
    local push_event = push_factory.new(self, event, payload)
    push_event:send()
    return push_event
  end

  self.join_push = push_factory.new(self, "phx_join", self.params)

  self.join_push:on_receive("ok", function(_)
                              log.info('join_push: on_receive()')
                              self.state = 'CHANNEL_JOINED'
  end)

  self.socket:phx_on_open(function()
      self:rejoin()
  end)

  self:on_close(function(_)
      self.state = 'CHANNEL_CLOSED'
      self.socket:remove_channel(self)
  end)

  self:on_error(function(_)
      self.state = 'CHANNEL_ERRORED'
  end)

  self:on_event(
    "phx_reply", function(message, ref)
      log.trace(
        string.format(
          'on_event()->phx_reply, message %s ref %s', message, ref))
      self:trigger_event(self:reply_event_name(ref), message, ref)
  end)

  return self
end

-- As far as I can tell, this is the way to get this syntax to work.
-- local ws = require('vendor/websockets/phx_socket').new()
-- Copied from client.lua in websockets library.
return setmetatable({}, {__index = function(_, _)
                           return new
                   end})
