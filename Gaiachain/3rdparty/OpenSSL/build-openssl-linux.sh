#!/bin/bash

VERSION=${OPENSSL_VERSION}

if [ ! -f "openssl-$VERSION.tar.gz" ]; then
    wget https://www.openssl.org/source/openssl-$VERSION.tar.gz
fi

rm -fr linux
mkdir linux
rm -fr openssl-$VERSION
tar xfa openssl-$VERSION.tar.gz
cd openssl-$VERSION

./Configure shared linux-x86_64 || exit 1
make depend
# SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so
make -j$(nproc) build_libs || exit 1
# -strip-all
strip --strip-all libcrypto.so
strip --strip-all libssl.so
cp libcrypto.so ../linux
cp libssl.so ../linux
cd ..

