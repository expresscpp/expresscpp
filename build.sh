#!/bin/bash

export CC=/usr/bin/gcc-9
export CXX=/usr/bin/g++-9
rm -rf _build
mkdir _build
cd _build
cmake ..
make -j4

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci -f docker/ci/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci