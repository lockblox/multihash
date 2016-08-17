#!/bin/bash

SOURCE_DIR=`dirname $0`
SOURCE_DIR=`readlink -f $SOURCE_DIR`
CPUS=`lscpu | egrep "^CPU\(s\): *[0-9]" | tr -s " " | cut -d " " -f 2`

echo Starting build \
 && cmake -DCMAKE_CXX_FLAGS="-O1 -fsanitize=address,undefined \
    -fsanitize-blacklist=$SOURCE_DIR/test/sanitizer-blacklist.txt \
    -fno-omit-frame-pointer" -DCMAKE_BUILD_TYPE=Debug $SOURCE_DIR \
 && echo Building with address and undefined behaviour sanitizers \
 && echo Running scan-build && time scan-build make -j $CPUS \
 && echo Running tests with address and undefined behaviour sanitizers \
 && time make test CTEST_OUTPUT_ON_FAILURE=TRUE \
 && cmake -DCMAKE_CXX_FLAGS="-O1 -fsanitize=thread -fno-omit-frame-pointer" \
    -fsanitize-blacklist=$SOURCE_DIR/test/sanitizer-blacklist.txt \
    -DCMAKE_BUILD_TYPE=Debug $SOURCE_DIR \
 && make -j $CPUS clean \
 && echo Building with thread sanitizer \
 && time make -j $CPUS \
 && echo Running tests with thread sanitizer \
 && time make test CTEST_OUTPUT_ON_FAILURE=TRUE \
 && echo Running clang-format && $SOURCE_DIR/test/clang-format.sh \
 && echo Running clang-tidy && $SOURCE_DIR/test/clang-tidy.sh . \
 && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="--coverage" \
    $SOURCE_DIR \
 && echo Running production build && time make -j $CPUS
