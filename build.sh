#!/bin/bash

rm -rf _build
mkdir _build
cd _build
cmake ..
make -j4

# docker build -t registry.gitlab.com/expresscpp/expresscpp/ci -f docker/ci/Dockerfile .
# docker push registry.gitlab.com/expresscpp/expresscpp/ci