#!/usr/bin/env bash

cd $(dirname $0)
mkdir -p build

g++ source/gridman.cpp  -o build/gridman  -std=c++11  -O3 -ffast-math  -Wall \
    && ./build/gridman
