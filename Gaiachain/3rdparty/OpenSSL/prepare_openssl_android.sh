#!/bin/bash

# include common part
. `dirname $0`/prepare_openssl_common.sh

GIT_TAG="OpenSSL_1_0_2n"

check_openssl_sources

cd $OPENSSL_SRC_DIR
git checkout $GIT_TAG

source $SCRIPT_DIR/setenv-android.sh
OUTPUT_DIR=$SCRIPT_DIR/OpenSSL/android/armeabi-v7a

./Configure --prefix=$OUTPUT_DIR shared android-armv7
make clean
# The "weird" lines below are necessary to produce .so files that are not
# versioned. Android requires that.
make CALC_VERSIONS="SHLIB_COMPAT=; SHLIB_SOVER=" MAKE="make -e" all
mkdir -p $OUTPUT_DIR/lib
echo "place-holder make target for avoiding symlinks" >> $OUTPUT_DIR/lib/link-shared
make SHLIB_EXT=.so install_sw
rm $OUTPUT_DIR/lib/link-shared
