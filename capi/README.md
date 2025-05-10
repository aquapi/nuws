C-API for Nelua.

```sh
# Build to a static library
make
```

To use the static library in C, link `stdc++`, `zlib` and `libuv`
```sh
$(CC) ... -lstdc++ -lz -luv ...
```
