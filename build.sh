#!/bin/bash

export CC=/usr/bin/clang-9
export CXX=/usr/bin/clang++-9
rm -rf _build
mkdir _build
cd _build
cmake ..
make -j4

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci -f docker/ci/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci