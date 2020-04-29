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

./Configure shared || exit 1
make depend
make -j$(nproc) SHLIB_VERSION_NUMBER= SHLIB_EXT=_1_1.so build_libs || exit 1
# -strip-all
strip --strip-all libcrypto_1_1.so
strip --strip-all libssl_1_1.so
cp libcrypto_1_1.so ../linux
cp libssl_1_1.so ../linux
 cd ..

