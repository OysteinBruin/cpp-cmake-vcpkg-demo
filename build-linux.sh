cmake -B build-linux -S . \
    -GNinja \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build-linux