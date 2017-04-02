_G.__PLAYER_BIRD = "player_bird"

local lovetoys = require("lovetoys.lovetoys")
local player_bird = lovetoys.Component.create(_G.__PLAYER_BIRD)

player_bird.initialize = function(self)
  self.server_id = nil
end
