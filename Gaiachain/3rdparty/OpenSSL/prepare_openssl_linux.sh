#!/bin/bash

set -e

SCRIPT_DIR=$PWD
#/`dirname $0`

echo "Script dir: "$SCRIPT_DIR

# Get OpenSSL version to download
source $SCRIPT_DIR/setenv-android.sh

mkdir build
cd build
source $SCRIPT_DIR/build-openssl-linux.sh

cd ../

INCLUDE_DIR=OpenSSL/include/
LIBS_DIR=OpenSSL/lib/linux/

mkdir -p ${INCLUDE_DIR}
mkdir -p ${LIBS_DIR}

cp -r build/openssl-${OPENSSL_VERSION}/include/openssl ${INCLUDE_DIR}
cp -r build/linux ${LIBS_DIR}
