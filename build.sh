#!/bin/bash

export CC=/usr/bin/clang-9
export CXX=/usr/bin/clang++-9
rm -rf _build
mkdir _build
cd _build
cmake .. \
-DEXPRESSCPP_BUILD_EXAMPLE=ON \
-DEXPRESSCPP_BUILD_TESTS=ON \
-DEXPRESSCPP_USE_ADDRESS_SANITIZER=ON

make -j4

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci -f docker/ci/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci