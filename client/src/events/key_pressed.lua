local class = require("lovetoys.lib.middleclass")
local key_pressed = class("key_pressed")

key_pressed.initialize = function(self, key, isrepeat)
  self.key = key
  self.isrepeat = isrepeat
end

return key_pressed
