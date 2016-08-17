#!/bin/bash

if [ $# -ne 1 ];
then
    echo Usage: clang-tidy.sh BUILD_DIRECTORY
    exit 1
fi

# Find sources
TEST_DIR=`dirname $0`
BUILD_DIR=$1
PROJECT_DIR=`readlink -f $TEST_DIR/..`/
TMP_FILENAME=`cat /dev/urandom | tr -dc [:alnum:] | fold -w 10 | head -n 1`
TMP_FILENAME="$BUILD_DIR/$TMP_FILENAME"

RETURN_CODE=0

IGNORE_FILE="$TEST_DIR/clang-tidy.ignore"

touch "$TMP_FILENAME"

if [ -f "$IGNORE_FILE" ];
then
    cp "$IGNORE_FILE" "$TMP_FILENAME"
else
    echo "Create $IGNORE_FILE with contents of errors you wish to supress"
fi

/usr/lib/llvm-3.8/share/clang/run-clang-tidy.py \
    -p "$BUILD_DIR" -header-filter=".*" -checks=*,-llvm-header-guard 2>/dev/null \
    | egrep -o "\/.*" \
    | egrep "\..*:[0-9]+:" \
    | grep -v "note: " \
    | grep -v "googletest-src" \
    | sed "s|$PROJECT_DIR||g" \
    | grep -F -x -v -f "$TMP_FILENAME"

RETURN_CODE=$?

rm "$TMP_FILENAME"

if [ $RETURN_CODE -ne 0 ]; then
    RETURN_CODE=0
else
    RETURN_CODE=1
fi

exit $RETURN_CODE
