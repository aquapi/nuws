local fs = require 'nelua.utils.fs'

local function getdir()
  return fs.dirname(debug.getinfo(1).source:sub(2, -9))
end

local dir = getdir()

return {
  dirname = getdir,
  header = function()
    return fs.abspath('../capi/libuwebsockets.h', dir)
  end,
  lib = function()
    return fs.abspath('../capi/libuwebsockets.so', dir)
  end
}
