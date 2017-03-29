-- Synchronizes the Position Component with the Position of the Body Component, if an Entity has both.
local physics_position_sync_system = class("physics_position_sync_system", System)

physics_position_sync_system.update = function(self, dt)
  -- Syncs the Position with the Physic. Physic is the primary component.
  for k, entity in pairs(self.targets) do
    entity:get(_G.__POSITION).x = entity:get(_G.__PHYSIC).body:getX()
    entity:get(_G.__POSITION).y = entity:get(_G.__PHYSIC).body:getY()
  end
end

physics_position_sync_system.requires = function()
  return {_G.__PHYSIC, _G.__POSITION}
end

return physics_position_sync_system
