package.path = "../?.lua;" .. package.path
package.path = require("loadpath") .. package.path

local T = require("luaunit.luaunit")

test_phx_socket = {}

test_phx_socket.setUp = function(self)
  self.socket = require("vendor/websockets/phx_socket").new()
end

test_phx_socket.test_make_ref__different_values = function(self)
  local a = self.socket:make_ref()
  local b = self.socket:make_ref()
  T.assertNotEquals(a, b)
end

test_phx_socket.test_initial_ref_is_different = function(self)
  local my_ref = self.socket.ref

  -- This is a ghetto way to ensure the ref is seeded with a different time.
  local os_time = os.time()
  while (os_time == os.time()) do
    -- Do nothing
  end

  local other_ref = require("vendor/websockets/phx_socket").new().ref
  T.assertNotEquals(my_ref, other_ref)
end

test_phx_socket.test_make_ref__returns_number = function(self)
  T.assertEquals(type(self.socket:make_ref()), type(1))
end

test_phx_socket.tearDown = function(self)
  self.socket = nil
end

T.LuaUnit:run()
