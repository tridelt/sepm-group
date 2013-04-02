#!/bin/bash
mkdir -p build
set -e
cd build
rm -f tests/reports/*.xml
cmake ..
make test
make doc
