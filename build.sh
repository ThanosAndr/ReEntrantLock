#!/bin/bash

# Create a build directory
mkdir build
cd build

# Run CMake to generate build files
cmake ..

# Build the project
make