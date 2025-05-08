#!/usr/bin/env bash
SCRIPTS="$(realpath -s "$(dirname "$0")")"
ROOT="$(dirname "$SCRIPTS")"

# Load submodule
cd "$ROOT"
git submodule update --init --recursive

# Build uws
cd "$ROOT/capi"
make shared

# Bindgen
cd "$SCRIPTS"
nelua ./bindgen.nelua
