#!/bin/bash

# Find sources
TEST_DIR=`dirname $0`
BUILD_DIR="$TEST_DIR"/../build
SOURCE_DIR="$TEST_DIR"/../src
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

/opt/llvm/llvm/tools/clang/tools/extra/clang-tidy/tool/run-clang-tidy.py \
    -p "$BUILD_DIR" -header-filter=".*" | egrep "\..*:[0-9]+:" \
    | grep -F -x -v -f "$TMP_FILENAME"

RETURN_CODE=$?

rm "$TMP_FILENAME"

if [ $RETURN_CODE -ne 0 ]; then
    RETURN_CODE=0
else
    RETURN_CODE=1
fi

exit $RETURN_CODE
