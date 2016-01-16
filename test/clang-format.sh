#!/bin/bash

INPLACE=0
while getopts i option
do
    case $option in 
    i)  INPLACE=1
        ;;
    *)  echo "Usage $0 [-i]"
        echo "OPTIONS:"
        echo       -i: Format files according to coding standards in place
        exit 1
        ;;
    esac
done

# Find sources
TEST_DIR=`dirname $0`
BUILD_DIR="$TEST_DIR"/../build
SOURCE_DIR="$TEST_DIR"/../src
TMP_FILENAME=`cat /dev/urandom | tr -dc [:alnum:] | fold -w 10 | head -n 1`
TMP_FILENAME="$BUILD_DIR/$TMP_FILENAME"

RETURN_CODE=0

for SOURCE_FILE in `find "$SOURCE_DIR" "$TEST_DIR" -name "*.cpp" -o -name "*.h"` 
do
    # In-place formatting
    if [ $INPLACE -eq 1 ]; then
        clang-format -style=file -i "$SOURCE_FILE"
        if [ $? -ne 0 ]; then
            RETURN_CODE=1
        fi
        continue
    fi

    clang-format -style=file "$SOURCE_FILE" > "$TMP_FILENAME"
    diff "$SOURCE_FILE" "$TMP_FILENAME"
    if [ $? -ne 0 ]; then
        FILEPATH=`echo "$SOURCE_FILE" | sed "s/.*\.\.\///g"`
      echo "$FILEPATH" 1>&2
      RETURN_CODE=1
    fi
    rm $TMP_FILENAME
done

if [ $RETURN_CODE -ne 0 ]; then
    echo "The above files do not meet the coding standards."
    echo "Please run clang-format -style=file -i [FILE] to remediate."
fi

exit $RETURN_CODE
