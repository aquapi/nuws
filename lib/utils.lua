local fs = require 'nelua.utils.fs'

local function getdir()
  return fs.dirname(debug.getinfo(1).source:sub(2, -9))
end

local dir = getdir()

return {
  dirname = getdir,
  header = fs.abspath('../capi/libuwebsockets.h', dir),
  lib = fs.abspath('../capi/libuwebsockets.so', dir)
}
