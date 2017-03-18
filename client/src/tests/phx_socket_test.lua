package.path = '../?.lua;' .. package.path
package.path = require('loadpath') .. package.path

local T = require('/vendor.luaunit.luaunit')

test_phx_socket = {}

function test_phx_socket:setUp()
  self.socket = require('vendor/websockets/phx_socket').new()
end

function test_phx_socket:test_make_ref__different_values()
  local a = self.socket:make_ref()
  local b = self.socket:make_ref()
  T.assertNotEquals(a, b)
end

function test_phx_socket:test_initial_ref_is_different()
  local my_ref = self.socket.ref

  -- This is a ghetto way to ensure the ref is seeded with a different time.
  local os_time = os.time()
  while (os_time == os.time()) do
    -- Do nothing
  end

  local other_ref = require('vendor/websockets/phx_socket').new().ref
  T.assertNotEquals(my_ref, other_ref)
end

function test_phx_socket:test_make_ref__returns_number()
  T.assertEquals(type(self.socket:make_ref()), type(1))
end

function test_phx_socket:tearDown()
  self.socket = nil
end

T.LuaUnit:run()
