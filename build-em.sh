#!/bin/env bash
pushd /opt/emsdk
source emsdk_env.sh
popd

mkdir build-em &>/dev/null
cd build-em
       	#-DCMAKE_INSTALL_PREFIX=/usr/lib/emscripten/system
cmake .. -G Ninja \
       	-DCMAKE_INSTALL_PREFIX=dist \
       	-DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_TOOLCHAIN_FILE="../toolchains/generic/Emscripten-wasm.cmake" \
        -DCMAKE_PREFIX_PATH="/usr/lib/emscripten/system/" \
        -DCMAKE_FIND_ROOT_PATH="/usr/lib/emscripten/system/" \
       	-DEMSCRIPTEN_PREFIX="/usr/lib/emscripten"

cmake --build . --target install

[ "$1" == "run" ] && emrun ./dist/MyApplication.html &>/dev/null &
exit 0
