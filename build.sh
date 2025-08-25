#! /bin/bash

if [[ -d "build" ]]; then
    rm -rf build
fi

meson setup build

pushd build
meson compile
popd
