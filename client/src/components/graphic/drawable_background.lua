_G.__DRAWABLE_BACKGROUND = 'drawable_background'

local drawable_background = Component.create(_G.__DRAWABLE_BACKGROUND)

drawable_background.initialize = function(self, x, y)
  self.x = x
  self.y  = y
end
