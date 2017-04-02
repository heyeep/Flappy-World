_G.__CIRCLE = "circle"

local lovetoys = require("lovetoys.lovetoys")
local circle = lovetoys.Component.create(_G.__CIRCLE)

circle.initialize = function(self, r)
  self.r = r
end
