#!/bin/bash
mkdir -p build
cd build
rm -f tests/reports/*.xml
cmake ..
make test
make doc
