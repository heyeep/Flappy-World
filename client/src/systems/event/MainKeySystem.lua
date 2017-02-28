local MainKeySystem = class("MainKeySystem", System)

function MainKeySystem:fireEvent(event)
  if event.key == "space" then
    for _, entity in pairs(engine:getEntitiesWithComponent("PlayerBird")) do
      entity:get("Physic").body:setLinearVelocity(0, -400)
    end
  end
end

return MainKeySystem
