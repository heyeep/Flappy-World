_G.__PHYSIC = 'physic'

local physic = Component.create(_G.__PHYSIC)

physic.initialize = function(self, body, fixture, shape)
  self.body = body
  self.shape = shape
  self.fixture = fixture
end

