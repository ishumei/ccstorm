#!/bin/bash

set -e

_root=$(which $0)
_root=$(dirname $_root)
_root=$(cd $_root; pwd)
cd $_root
_build=$_root/build

# rebuild with coverage info
rm -rf $_build
mkdir $_build
cd $_build
cmake -DCMAKE_BUILD_TYPE=Debug -DSTORM_ENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER=g++ ..     
make -j5
cd $_root

lcov -d $_build -z
lcov -d $_build -b . --no-external -i -c -o $_build/ccstorm_coverage_init.info
lcov -r $_build/ccstorm_coverage_init.info \
        $_root'/thirdparty/*' \
        $_root'/include/rapidjson/*' \
        $_root'/test/*' \
        'storm/gen-cpp/*' \
     -o $_build/ccstorm_coverage_init.info
$_build/ccstorm_unit_tests
lcov -d $_build -b . --no-external -c -o $_build/ccstorm_coverage_report.info
lcov -r $_build/ccstorm_coverage_report.info \
        $_root'/thirdparty/*' \
        $_root'/include/rapidjson/*' \
        $_root'/test/*' \
        'storm/gen-cpp/*' \
     -o $_build/ccstorm_coverage_report.info
genhtml -o $_build/coverage --prefix=$_build $_build/ccstorm_coverage_init.info $_build/ccstorm_coverage_report.info

echo "report generated in $_build/coverage"

