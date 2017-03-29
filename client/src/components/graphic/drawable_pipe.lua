_G.__DRAWABLE_PIPE = 'drawable_pipe'

local drawable_pipe = Component.create(_G.__DRAWABLE_PIPE)

drawable_pipe.initialize = function(self, x, y, part)
  self.x = x
  self.y = y
  self.part = part
end

