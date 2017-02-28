local BirdBehaviorSystem = class("BirdBehaviorSystem", System)

function BirdBehaviorSystem:initialize(world_height, bird_speed)
  System.initialize(self)
  self.world_height = world_height
  self.bird_speed = bird_speed
end

function BirdBehaviorSystem:draw()
  for _, entity in pairs(self.targets) do
    local body = entity:get("Physic").body
    body:setX(body:getX() + self.bird_speed)

    if body:getY() > self.world_height then
      body:setY(0)
    end
  end
end

function BirdBehaviorSystem:requires()
  return {"PlayerBird", "Physic"}
end

return BirdBehaviorSystem
