_G.__PLAYER_BIRD = 'player_bird'

local player_bird = Component.create(_G.__PLAYER_BIRD)

player_bird.initialize = function(self)
  self.server_id = nil
end
