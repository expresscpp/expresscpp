#!/bin/bash

export CC=/usr/bin/gcc-9
export CXX=/usr/bin/g++-9
rm -rf _build
mkdir _build
cd _build
cmake .. \
-DEXPRESSCPP_BUILD_EXAMPLES=ON \
-DEXPRESSCPP_BUILD_TESTS=ON

make -j4

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_gcc8 -f docker/ci_gcc8/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_gcc8


# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_gcc9 -f docker/ci_gcc9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_gcc9


# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_clang9 -f docker/ci_clang9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_clang9
