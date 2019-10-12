#!/bin/bash

rm -rf _build
mkdir _build
cd _build
cmake ..

echo "Building $1"
make -j4 $1
