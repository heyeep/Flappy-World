Y_TOP_OF_WINDOW = 0

platform = {}
player = {}

GRAVITY = -1000

MAX_PIPES_ON_SCREEN = 6
X_PIPE_SPAWN_OFFSET = 10
Y_PIPE_GAP = 150
X_PIPE_MIN_SPACING = 300
PIPE_WIDTH = 30
PIPE_HEIGHT = 500
PIPE_SPEED = 5
pipes = {}
y_pipe_top = 0
y_pipe_bottom = 0
x_pipe_spawn = 0

function love.load(arg)
  if arg[#arg] == "-debug" then require("mobdebug").start() end

  y_pipe_top = 0
  y_pipe_bottom = love.graphics.getWidth()
  x_pipe_spawn = love.graphics.getWidth() + X_PIPE_SPAWN_OFFSET

  platform.width = love.graphics.getWidth()
  platform.height = love.graphics.getHeight()
  platform.x = 0
  platform.y = platform.height / 2

  player.x = love.graphics.getWidth() / 2
  player.y = love.graphics.getHeight() / 2
  player.img = love.graphics.newImage('purple.png')
  player.ground = love.graphics.getHeight()
  player.y_velocity = 0
  player.y_terminal_velocity = 500
  player.jump_height = -400
  player.gravity = GRAVITY
  player.speed = 200

  -- pipes
  for i = 1, MAX_PIPES_ON_SCREEN do
    x = i * X_PIPE_MIN_SPACING + x_pipe_spawn
    pipes[i] = spawnPipe(x)
  end
end


function love.draw()
  -- draw player given player properties
  love.graphics.draw(player.img, player.x, player.y, 0, 1, 1, 0, 32)
  -- draw all pipes
  love.graphics.setColor(0, 255, 255)
  for i, p in ipairs(pipes) do
    love.graphics.rectangle("fill", p.top.x, p.top.y, p.top.w, p.top.h)
    love.graphics.rectangle("fill", p.bot.x, p.bot.y, p.bot.w, p.bot.h)
  end
end

function love.update(dt) 
  -- player controls
  if love.keyboard.isDown('space') then
    player.y_velocity = player.jump_height
  end

  -- player physics
  if player.y_velocity ~= 0 then
      player.y = player.y + player.y_velocity * dt
    if player.y_velocity >= player.y_terminal_velocity then
      player.y_velocity  = player.y_terminal_velocity
    else
      player.y_velocity = player.y_velocity - player.gravity * dt
    end
  end

  if player.y > player.ground then
    player.y = Y_TOP_OF_WINDOW
  end

  -- pipes
  x_rightmost_pipe = 0
  for i, p in ipairs(pipes) do
    if p.x > x_rightmost_pipe then
      x_rightmost_pipe = p.x
    end
  end

  for i, p in ipairs(pipes) do
    p.x = p.x - p.speed
    p.top.x = p.top.x - p.speed
    p.bot.x = p.bot.x - p.speed

    if p.x <= 0 then
      pipes[i] = spawnPipe(x_rightmost_pipe + X_PIPE_MIN_SPACING)
    end
  end

end

function spawnPipe(x)
  upper_bound = y_pipe_top + (Y_PIPE_GAP / 2)
  lower_bound = y_pipe_bottom - (Y_PIPE_GAP / 2)
  y = math.random(upper_bound, lower_bound)

  pipe = {}

  pipe.x = x
  pipe.speed = PIPE_SPEED
  pipe.w = PIPE_WIDTH

  pipe.top = {}

  pipe.top.x = x
  pipe.top.y = 0
  pipe.top.h = y - (Y_PIPE_GAP / 2)
  pipe.top.w = PIPE_WIDTH

  pipe.bot = {}

  pipe.bot.x = x
  pipe.bot.y = y + (Y_PIPE_GAP / 2)
  pipe.bot.h = y_pipe_bottom - y
  pipe.bot.w = PIPE_WIDTH

  return pipe
end
