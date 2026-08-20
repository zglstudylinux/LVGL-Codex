#!/usr/bin/env sh
set -eu

app="${1:-device-dashboard}"
build_dir="build/linux-${app}"

cmake -S . -B "$build_dir" -G Ninja \
  -DLVGL_APP="$app" \
  -DLVGL_TARGET=linux-fbdev-evdev \
  -DLVGL_SERIES=9
cmake --build "$build_dir"
ctest --test-dir "$build_dir" --output-on-failure
