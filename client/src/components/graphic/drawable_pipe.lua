_G.__DRAWABLE_PIPE = "drawable_pipe"

local lovetoys = require("lovetoys.lovetoys")
local drawable_pipe = lovetoys.Component.create(_G.__DRAWABLE_PIPE)

drawable_pipe.initialize = function(self, x, y, part)
  self.x = x
  self.y = y
  self.part = part
end

