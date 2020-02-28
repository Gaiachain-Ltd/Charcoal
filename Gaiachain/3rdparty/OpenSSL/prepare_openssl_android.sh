#!/bin/bash

set -e

SCRIPT_DIR=$PWD
#/`dirname $0`

echo "Script dir: "$SCRIPT_DIR

source $SCRIPT_DIR/setenv-android.sh

mkdir build
cd build
source $SCRIPT_DIR/build-openssl-android.sh

cd ../

INCLUDE_DIR=OpenSSL/include/
LIBS_DIR=OpenSSL/lib/android/

mkdir -p ${INCLUDE_DIR}
mkdir -p ${LIBS_DIR}

cp -r build/openssl-${OPENSSL_VERSION}/include/openssl ${INCLUDE_DIR}

for arch in ${ANDROID_ARCHS}; do
    cp -r build/${arch} ${LIBS_DIR}
done
