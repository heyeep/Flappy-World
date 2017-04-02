_G.__POSITION = "position"

local lovetoys = require("lovetoys.lovetoys")
local position = lovetoys.Component.create(_G.__POSITION)

position.initialize = function(self, x, y)
  self.x = x
  self.y = y
end
