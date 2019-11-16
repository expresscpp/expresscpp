#!/bin/bash

echo "starting"
echo ""

rm -rf build
mkdir build
cd build
cmake .. -GNinja \
-DEXPRESSCPP_BUILD_EXAMPLES=ON \
-DEXPRESSCPP_BUILD_TESTS=ON \
-DEXPRESSCPP_USE_CONAN_DEPENDENCIES=ON \
-DEXPRESSCPP_ENABLE_COVERAGE=ON \
-DEXPRESSCPP_USE_ADDRESS_SANITIZER=ON


cmake --build . -j

echo ""
echo "done"
