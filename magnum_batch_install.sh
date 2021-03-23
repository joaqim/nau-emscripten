#!/usr/bin/env bash
sudo chown $USER:wheel /opt -R

cd /opt
[ -d emsdk ] || git clone https://github.com/juj/emsdk
cd emsdk
#./emsdk update
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd /opt

[ -d corrade ] || git clone --recurse-submodules git://github.com/mosra/corrade
[ -d magnum ] || git clone --recurse-submodules git://github.com/joaqim/magnum -b emscripten-pointerlock
[ -d magnum-plugins ] || git clone --recurse-submodules git://github.com/mosra/magnum-plugins
[ -d libjpeg ] || git clone https://git.sprintf.io/emscripten-ports/libjpeg
[ -d magnum-integration ] || git clone --recurse-submodules git://github.com/mosra/magnum-integration
#[ -d magnum-extras ] || git clone --recurse-submodules git://github.com/mosra/magnum-extras

[ -d magnum-examples ] || git clone --recurse-submodules git://github.com/mosra/magnum-examples

BUILD_DIR=build-emc
mkdir corrade/$BUILD_DIR &>/dev/null
mkdir magnum/$BUILD_DIR &>/dev/null
mkdir magnum-plugins/$BUILD_DIR &>/dev/null
#mkdir magnum-extras/$BUILD_DIR &>/dev/null
mkdir magnum-examples/$BUILD_DIR &>/dev/null

# This is the path magnum example apps will be deployed to.
DEPLOY_PREFIX=$PWD"/deploy"
mkdir $DEPLOY_PREFIX &>/dev/null

EMSCRIPTEN_PREFIX="/usr/lib/emscripten"

CMAKE_CMD="cmake .. -GNinja -DCMAKE_PREFIX_PATH=$PREFIX_PATH -DCMAKE_INSTALL_PREFIX=$EMSCRIPTEN_PREFIX/system -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../toolchains/generic/Emscripten-wasm.cmake -DEMSCRIPTEN_PREFIX=$EMSCRIPTEN_PREFIX"

CMAKE_BUILD_CMD="sudo cmake --build . --target install"

cd /opt/corrade/$BUILD_DIR
$CMAKE_CMD -DWITH_TESTSUITE=ON -DBUILD_DEPRECATED=OFF
$CMAKE_BUILD_CMD

#cd /opt/
#[ -d zlib ] || git clone https://github.com/hachque-Emscripten/zlib-1.2.5 zlib
#mkdir zlib/$BUILD_DIR 2&>/dev/null
#cd zlib
#git clone https://github.com/mosra/toolchains
#cd $BUILD_DIR
#$CMAKE_CMD -D /opt/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
#$CMAKE_BUILD_CMD
#exit $?

cd /opt/magnum/$BUILD_DIR
echo \#\!/usr/bin/env sh\n$CMAKE_CMD -DWITH_SDL2APPLICATION=ON -DWITH_EMSCRIPTENAPPLICATION=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_AUDIO=ON -DWITH_WAVAUDIOIMPORTER=ON -DBUILD_DEPRECATED=OFF -DWITH_ANYIMAGEIMPORTER=ON -DWITH_PNGIMPORTER=ON \n$CMAKE_BUILD_CMD > ./build-em.sh && sudo chmod +x ./build-em.sh

$CMAKE_CMD -DWITH_SDL2APPLICATION=ON -DWITH_EMSCRIPTENAPPLICATION=ON -DWITH_MESHTOOLS=ON -DWITH_PRIMITIVES=ON -DWITH_OBJIMPORTER=ON -DWITH_TGAIMPORTER=ON -DWITH_SHADERS=ON -DWITH_TEXT=ON -DWITH_AUDIO=ON -DWITH_WAVAUDIOIMPORTER=ON -DBUILD_DEPRECATED=OFF -DWITH_ANYIMAGEIMPORTER=ON -DWITH_PNGIMPORTER=ON
$CMAKE_BUILD_CMD

#cd /opt
#wget http://www.ijg.org/files/jpegsrc.v9b.tar.gz
#tar xvzf jpegsrc.v9b.tar.gz -C ./libjpeg --strip-components=1
#cd ./libjpeg
#emconfigure ./configure --enable-shared=no --enable-static=yes --prefix="$EMSCRIPTEN_PREFIX/system"
#emmake make
#sudo make install

#cd /opt/magnum-plugins/$BUILD_DIR
#$CMAKE_CMD -DWITH_OPENGEXIMPORTER=ON -DWITH_STBTRUETYPEFONT=ON -DBUILD_DEPRECATED=OFF -DWITH_TINYGLTFIMPORTER=ON -DWITH_JPEGIMPORTER=ON
#$CMAKE_BUILD_CMD

#cd ../../magnum-extras/$BUILD_DIR
#$CMAKE_CMD -DWITH_UI=OFF -DWITH_UI_GALLERY=ON -DMAGNUM_DEPLOY_PREFIX=$DEPLOY_PREFIX -DBUILD_DEPRECATED=OFF
#$CMAKE_BUILD_CMD

#cd ../../magnum-examples/$BUILD_DIR
#$CMAKE_CMD -DWITH_AUDIO_EXAMPLE=OFF -DWITH_TEXTUREDTRIANGLE_EXAMPLE=ON -DWITH_WEBVR_EXAMPLE=OFF -DMAGNUM_DEPLOY_PREFIX=$DEPLOY_PREFIX
#$CMAKE_BUILD_CMD

