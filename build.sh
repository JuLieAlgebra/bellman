#!/usr/bin/env bash
####
# Compiles the "bellman" project.
####

# Temporarily change to the directory that this script is stored in
cd $(dirname $0)

# Make a subdirectory called "build" or just use the existing one
mkdir -p build

# Compile the "wendyhunt" executable into the "build" directory
g++ source/wendyhunt.cpp  -o build/wendyhunt  -std=c++11  -O3 -ffast-math  -Wall
