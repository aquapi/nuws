#!/usr/bin/env bash
cd "$(dirname "$0")"

# Load submodule
git submodule update --init --recursive

# Build uws
cd capi
make shared
cd ..

# Bindgen
nelua ./lib/bindgen.nelua
