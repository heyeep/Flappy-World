_G.__POSITION = 'position'

local position  = Component.create(_G.__POSITION)

function position:initialize(x, y)
  self.x = x
  self.y = y
end
