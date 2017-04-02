_G.__DRAWABLE_BACKGROUND = "drawable_background"

local lovetoys = require("lovetoys.lovetoys")
local drawable_background =
  lovetoys.Component.create(_G.__DRAWABLE_BACKGROUND)

drawable_background.initialize = function(self, x, y)
  self.x = x
  self.y = y
end
