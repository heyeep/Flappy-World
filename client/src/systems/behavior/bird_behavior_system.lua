local bird_behavior_system = class("bird_behavior_system", System)

function bird_behavior_system:initialize(world_height, bird_speed)
  System.initialize(self)
  self.world_height = world_height
  self.bird_speed = bird_speed
end

function bird_behavior_system:draw()
  for _, entity in pairs(self.targets) do
    local body = entity:get("physic").body
    body:setX(body:getX() + self.bird_speed)

    if body:getY() > self.world_height then
      body:setY(0)
    end
  end
end

function bird_behavior_system:requires()
  return {"player_bird", "physic"}
end

return bird_behavior_system
