-- Synchronizes the Position Component with the Position of the Body Component, if an Entity has both.
local physics_position_sync_system = class("physics_position_sync_system", System)

function physics_position_sync_system:update(dt)
  -- Syncs the Position with the Physic. Physic is the primary component.
  for k, entity in pairs(self.targets) do
    entity:get("position").x = entity:get("physic").body:getX()
    entity:get("position").y = entity:get("physic").body:getY()
  end
end

function physics_position_sync_system:requires()
  return {"physic", "position"}
end

return physics_position_sync_system
