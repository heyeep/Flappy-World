-- Usage: Plain Lua
-- local require_path = require('loadpath')
-- package.path = require_path .. package.path

-- Usage: Plain Lua, Nested Directory
-- package.path = '../?.lua;' .. package.path
-- package.path = require('loadpath') .. package.path

-- Usage: Love
-- local require_path = require('loadpath')
-- love.filesystem.setRequirePath(require_path .. love.filesystem.getRequirePath())

-- Set up directories 'require' looks in for modules.
-- It looks for directories starting in the 'src' directory.
-- https://love2d.org/wiki/love.filesystem.setRequirePath
local require_path = ''
require_path = 'vendor/?.lua;' .. require_path
require_path = 'tests/?.lua;' .. require_path
require_path = 'components/?.lua;' .. require_path
require_path = 'events/?.lua;' .. require_path
require_path = 'systems/?.lua;' .. require_path
require_path = 'vendor/?.lua;' .. require_path
require_path = 'util/?.lua;' .. require_path
return require_path
