#!/usr/bin/env bash
####
# Compiles the "bellman" project.
####

# Temporarily change to the directory that this script is stored in
cd $(dirname $0)

# Make a subdirectory called "build" or just use the existing one
mkdir -p build

# Compilation recipe
COMPILE_FLAGS="-std=c++11  -O3 -ffast-math  -Wall"
TARGETS="wendyhunt gridboi"

# Run compilations
for TARGET in ${TARGETS}
do
    echo "Compiling '${TARGET}'..."
    g++ source/${TARGET}.cpp  -o build/${TARGET}  ${COMPILE_FLAGS}
done
