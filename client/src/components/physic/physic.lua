local physic = Component.create("physic")

function physic:initialize(body, fixture, shape)
  self.body = body
  self.shape = shape
  self.fixture = fixture
end

