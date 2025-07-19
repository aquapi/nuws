local utils = require('lib.utils')
local fs = require('nelua.utils.fs')

print('Generate bindings...')

-- generate nelua bindings file
require('nelua.plugins.nldecl').generate_bindings_file({
  output_file = utils.bindings,
  parse_includes = { utils.header },
  include_names = { '^uws_' }
})

print('Patching bindings...')

fs.writefile(
  utils.bindings,
  fs.readfile(utils.bindings):gsub(
    "global uws_c_string_view_t: type <cimport,nodecl,forwarddecl> = @record{}",
    "global uws_c_string_view_t = string"
  )
)

print('Binding generated at "lib/bindings.nelua"!')
