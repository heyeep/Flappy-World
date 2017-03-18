package.path = '../?.lua;' .. package.path
package.path = require('loadpath') .. package.path

require('phx_socket_test')
