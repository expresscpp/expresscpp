#!/bin/bash


export CC=/usr/bin/clang-8
export CXX=/usr/bin/clang++-8
rm -rf _build
mkdir _build
cd _build
cmake .. -GNinja \
-DEXPRESSCPP_BUILD_EXAMPLES=ON \
-DEXPRESSCPP_BUILD_TESTS=ON \
-DEXPRESSCPP_USE_CONAN_DEPENDENCIES=ON 
-DEXPRESSCPP_RUN_CLANG_TIDY=ON

cmake --build . -j

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_gcc8 -f docker/ci_gcc8/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_gcc8


# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_gcc9 -f docker/ci_gcc9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_gcc9


# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_clang9 -f docker/ci_clang9/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_clang9


# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci_clang8 -f docker/ci_clang8/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci_clang8
