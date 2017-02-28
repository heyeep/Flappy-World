local TwoPipeDrawSystem = class("TwoPipeDrawSystem", System)

PIPE_WIDTH = 40
PIPE_GAP = 230

function TwoPipeDrawSystem:draw()
  local world_height = 600
  for index, entity in pairs(self.targets) do
    two_pipe = entity:get("DrawableTwoPipe")

    top = {
      x = two_pipe.x - (PIPE_WIDTH / 2),
      y = 0,
      w = PIPE_WIDTH,
      h = two_pipe.y - PIPE_GAP / 2
    }

    bot = {
      x = two_pipe.x - (PIPE_WIDTH / 2),
      y = top.h + PIPE_GAP / 2,
      w = PIPE_WIDTH,
      h = 1000
    }

    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("fill", top.x, top.y, top.w, top.h)
    love.graphics.rectangle("fill", bot.x, bot.y, bot.w, bot.h)
  end
end

function TwoPipeDrawSystem:requires()
  return {"DrawableTwoPipe"}
end


return TwoPipeDrawSystem
