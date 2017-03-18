local main_key_system = class("main_key_system", System)

function main_key_system:fireEvent(event)
  if event.key == "space" then
    for _, entity in pairs(engine:getEntitiesWithComponent("player_bird")) do
      entity:get("physic").body:setLinearVelocity(0, -400)
    end
  end
end

return main_key_system
