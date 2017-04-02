local lovetoys = require("lovetoys.lovetoys")
local middleclass = require("lovetoys.lib.middleclass")
local main_key_system =
  middleclass.class("main_key_system", lovetoys.System)
local log = require("logger.log")

main_key_system.fireEvent = function(_, event)
  log.trace("fireEvent", event, event.key)

  if event.key == "space" then
    for _, entity in pairs(_G.engine:getEntitiesWithComponent(_G.__MY_BIRD)) do
      entity:get(_G.__PHYSIC).body:setLinearVelocity(0, -400)
    end
  end
end

return main_key_system
