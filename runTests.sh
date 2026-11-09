#!/bin/bash
set -euo pipefail

# Run all registered tests via CTest
cd build
ctest --output-on-failure
