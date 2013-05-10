#!/bin/bash
MACHINE_TYPE=`uname -m`

mkdir -p build
set -e
cd build
rm -f tests/reports/*.xml
cmake ..
make
make test || true
make doc

if [ ${MACHINE_TYPE} == 'x86_64' ]; then
  CPPCHECK="../cmake/cppcheck_amd64"
else
  CPPCHECK="../cmake/cppcheck_x86"
fi

$CPPCHECK .. -i ../build -i ../dependencies --std=c++11 --enable=all --xml-version=2 2> style_issues.xml
