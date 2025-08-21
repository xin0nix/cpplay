#!/bin/bash

# URL=$1
# PROBLEM=$(echo $URL | awk -F/ '{print $5}')
PROBLEM=$1

DIR_NAME=$(echo $PROBLEM | tr ' ' _)
TEST_NAME=test_${DIR_NAME}.py

echo $DIR_NAME
echo $TEST_NAME

mkdir $DIR_NAME
touch $DIR_NAME/$TEST_NAME