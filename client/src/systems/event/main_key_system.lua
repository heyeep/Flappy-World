local main_key_system = class("main_key_system", System)
local log = require('logger.log')

main_key_system.fireEvent = function(self, event)
  log.trace('fireEvent', event, event.key)

  if event.key == "space" then
    for _, entity in pairs(_G.engine:getEntitiesWithComponent(_G.__MY_BIRD)) do
      entity:get(_G.__PHYSIC).body:setLinearVelocity(0, -400)
    end
  end
end

return main_key_system
