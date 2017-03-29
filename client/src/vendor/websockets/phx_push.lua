local new = function(channel, event, payload)
  local log = require('logger.log')
  local ev = require('ev')
  local self = {}
  log.outfile = 'log.txt'

  self.channel = channel
  self.event = event
  self.payload = payload or {}

  self.received_resp = nil
  self.after_hook = nil
  self.rec_hooks = {}
  self.sent = false

  self.ref_event= ''
  self.after_interval = 0
  self.after_timer = nil

  self.send = function(_)
    log.trace('send()')
    local socket_ref = self.channel.socket:make_ref()
    self.ref_event = self.channel:reply_event_name(socket_ref)
    self.received_resp = nil
    self.sent = false

    self.channel:on_event(self.ref_event, function(message, _)
                            log.trace('on_event()', self.ref_event)
                            self.received_resp = message
                            self:match_receive(message)
                            self:cancel_ref_event()
                            self:cancel_after()
    end)

    self:start_after()
    self.sent = true

    local json_map = {}
    json_map.topic = self.channel.topic
    json_map.event = self.event
    json_map.payload = self.payload
    json_map.ref = socket_ref
    self.channel.socket:push(json_map)
  end

  self.on_receive = function(_, status, callback)
    log.trace(string.format('on_receive(), status: %s', status))
    if (self.received_resp and self.received_resp.status == status) then
      callback(self.received_resp)
    end

    self.rec_hooks[#self.rec_hooks + 1] = {status = status, callback = callback}
    return self
  end

  self.after = function(_, ms, callback)
    log.trace(string.format('after(), ms: %s', ms))
    if (self.after_hook) then
      log.error("self.after_hook is non null.")
    end

    self.after_timer = ev.Timer.new(
      function(_, _)
        log.trace('after_timer()')
      end, ms, ms)
    self.after_timer:start(ev.Loop.default)

    self.after_interval = ms
    self.after_hook = callback
    return self
  end

  self.cancel_ref_event = function(_)
    log.trace('cancel_ref_event()')
    self.channel:off_event(self.ref_event)
  end

  self.cancel_after = function(_)
    log.trace('cancel_after()')
    if (self.after_hook) then
      return
    end

    if (self.after_timer) then
      self.after_timer:stop(ev.Loop.default)
      self.after_timer = nil
    end
  end

  self.start_after = function(_)
    log.trace('start_after()')
    if not self.after_hook then
      return
    end

    local callback = function(loop, timer_event)
      timer_event:stop(loop)
      self:cancel_ref_event()
      self:after_hook()
    end

    local interval = self.after_interval
    self.after_timer = ev.Timer.new(callback, interval, interval)
    self.after_timer:start(ev.Loop.default)
  end

  self.match_receive = function(_, p_payload)
    log.trace(string.format('match_receive(), p_payload: %s', p_payload))

    for i = 1, #self.rec_hooks do
      local rc_hook = self.rec_hooks[i]
      if (rc_hook.status == p_payload.status) then
        local on_message_callback = rc_hook.callback
        on_message_callback(p_payload.response)
      end
    end
  end

  return self
end

-- As far as I can tell, this is the way to get this syntax to work.
-- local ws = require('vendor/websockets/phx_socket').new()
-- Copied from client.lua in websockets library.
return setmetatable({},{__index = function(_, _)
                          return new
                   end})
