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

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_gcc9 -f docker/ci_gcc9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_gcc9

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_clang9 -f docker/ci_clang9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_clang9