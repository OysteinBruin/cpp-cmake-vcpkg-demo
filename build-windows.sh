cmake -B build-windows -S . \
    -GNinja \
    -DCMAKE_TOOLCHAIN_FILE=./mingw-toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release
cmake --build build-windows