#!/bin/bash

# Get the directory of the script
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

cd $SCRIPT_DIR/..

# Construct the paths
PROTO_FILE="$PWD/src/exchange/proto/ExchangeFormat.proto"
INCLUDE_DIR="$PWD/src/exchange/proto"
OUTPUT_DIR="$PWD/scripts"

# Invoke protoc
protoc "$PROTO_FILE" -I="$INCLUDE_DIR" --python_out="$OUTPUT_DIR"

