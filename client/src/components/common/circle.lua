_G.__CIRCLE = 'circle'

local circle  = Component.create(_G.__CIRCLE)

circle.initialize = function(self, r)
  self.r = r
end
