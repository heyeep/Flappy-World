_G.__DRAWABLE_BACKGROUND = 'drawable_background'

local drawable_background = Component.create(_G.__DRAWABLE_BACKGROUND)

function drawable_background:initialize(x, y)
  self.x = x
  self.y = y
end
