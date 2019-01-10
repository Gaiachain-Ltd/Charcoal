#!/bin/bash

SDK_VERSION="10.2"
IOS_BUILD_TYPE="all"

usage ()
{
    echo "
    usage: $0 -b -v -h
        -b iOS build type:
            device      - produces library suitable for device deployement (arm7 arm64)
            simulator   - produces library suitable for simulator deployement (i386 x86_64)
            all         - all targets in one library, produced library has large size (default)
        -v SDK version (default 10.2)
        -h shows this help message
    "
    exit 0
}

while getopts hv:b: option
do
 case "${option}"
 in
 h) usage;;
 v) SDK_VERSION=${OPTARG};;
 b) IOS_BUILD_TYPE=${OPTARG};;
 esac
done

if [ "$IOS_BUILD_TYPE" != "device" ] && [ "$IOS_BUILD_TYPE" != "simulator" ] && [ "$IOS_BUILD_TYPE" != "all" ]; then
    echo "Incorect build type:" $IOS_BUILD_TYPE
    usage
fi

IOS_SDK=$SDK_VERSION;

# check SDK
DEVELOPER=`xcode-select -print-path`

if [ ! -d $DEVELOPER ]; then
    echo "Please set up Xcode correctly. '$DEVELOPER_DIR' is not a valid developer tools folder."
    exit 1
fi
# for now not supporting macOS builds
#if [ ! -d "$DEVELOPER/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$OSX_SDK.sdk" ]; then
#    echo "The macOS SDK $OSX_SDK was not found."
#    exit 1
#fi
if [ ! -d "$DEVELOPER/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_SDK.sdk" ]; then
    echo "The iOS SDK $IOS_SDK was not found."
    exit 1
fi

# get openssl from git

# include common part
. `dirname $0`/prepare_openssl_common.sh

GIT_TAG="OpenSSL_1_0_2n"

check_openssl_sources

cd $OPENSSL_SRC_DIR
git checkout $GIT_TAG

# enable fall back on error
set -e

buildMacOs() {
    :
}

buildiOS() {

# NOTE: it's ok to build all arch and generate one lib, will be bigger thought
# NOTE: for release use armv7/s and arm64

# Device
#armv7 arm64
    ARCHS="armv7 armv7s arm64"

# Simulator
#i386 x86_64
if [ "$IOS_BUILD_TYPE" == "simulator" ]; then
    ARCHS="i386 x86_64"
fi

# build all libraries (default)
if [ "$IOS_BUILD_TYPE" == "all" ]; then
    ARCHS="i386 x86_64 armv7 armv7s arm64"
fi

# Build each architecture to separate directory, each build will be placed
# in current directory under arch name, i.e. iOS-i386
# NOTE: pathing is skipped, builds fine without it

BUILD_DIR="$SCRIPT_DIR/OpenSSL"

for ARCH in $ARCHS; do
    if [[ "${ARCH}" == "i386" || "${ARCH}" == "x86_64" ]]; then
        PLATFORM="iPhoneSimulator"
    else
        PLATFORM="iPhoneOS"
#        sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
    fi

    if [ "$ARCH" == "i386" ]; then
        MIN_SDK_VERSION="4.2"
    elif [ "$ARCH" == "x86_64" ]; then
        MIN_SDK_VERSION="6.0"
    elif [ "$ARCH" == "arm64" ]; then
        MIN_SDK_VERSION="6.0"
        ADDITIONAL_OPTIONS="enable-ec_nistp_64_gcc_128"
    else
        MIN_SDK_VERSION="6.0"
    fi

    export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
    export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"
    export BUILD_TOOLS="${DEVELOPER}"
    #export CC="${BUILD_TOOLS}/usr/bin/gcc -fembed-bitcode -mios-version-min=${MIN_SDK_VERSION} -arch ${ARCH}"
    export CC="clang -fembed-bitcode -mios-version-min=${MIN_SDK_VERSION} -arch ${ARCH}"

    echo "Building ${GIT_TAG} for platform: ${PLATFORM} SDK: ${SDK_VERSION} minSDK: ${MIN_SDK_VERSION} ARCH: ${ARCH}"
    echo "STD Log available in $OPENSSL_SRC_DIR/${GIT_TAG}-${ARCH}.log"
    echo "ERROR Log available in $OPENSSL_SRC_DIR/${GIT_TAG}-${ARCH}.error.log"

    echo "Configure..."

#    if [[ "${ARCH}" == "x86_64" ]]; then
# for macos
#        ./Configure darwin64-x86_64-cc --openssldir=$BUILD_DIR/iOS-${ARCH} -no-apps     1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"
#    else
        ./Configure iphoneos-cross ${ADDITIONAL_OPTIONS} --openssldir=$BUILD_DIR/iOS-${ARCH} -no-apps         1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"
#    fi

    echo "Building..."

    # add -isysroot to CC=
#    sed -ie "s!^CFLAG=!CFLAG=-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -miphoneos-version-min=${MIN_SDK_VERSION} !" "$SCRIPT_DIR/src/openssl/Makefile"

    # Patch Makefile
    if [ "$ARCH" == "x86_64" ]; then
            sed -ie "s/^CFLAG= -/CFLAG=  -miphoneos-version-min=$MIN_SDK_VERSION -DOPENSSL_NO_ASM -/" "$SCRIPT_DIR/src/openssl/Makefile"
    fi
#            sed -ie "s/^CFLAG= -/CFLAG=  -miphoneos-version-min=$MIN_SDK_VERSION -/" "$SCRIPT_DIR/Makefile"
#    fi

    # Patch versions
#    sed -ie "s/^# define OPENSSL_VERSION_NUMBER.*$/# define OPENSSL_VERSION_NUMBER  $FAKE_NIBBLE/" "$SCRIPT_DIR/src/openssl/crypto/opensslv.h"
#    sed -ie "s/^#  define OPENSSL_VERSION_TEXT.*$/#  define OPENSSL_VERSION_TEXT  \"$FAKE_TEXT\"/" "$SCRIPT_DIR/src/openssl/crypto/opensslv.h"


    make clean      1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"
    make -j$JOBS    1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"

    echo "Build completed. Installing to:" $BUILD_DIR/iOS-${ARCH}

    make install    1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"
    make clean      1>> "${GIT_TAG}-${ARCH}.log" 2>> "${GIT_TAG}-${ARCH}.error.log"
done

}

deployMacOS() {
    :
}

# deploying FAT build, these builds contains both 32 and 64 bit builds
deployiOS() {

    echo "Building iOS libraries..."

    FILES="libcrypto.a libssl.a"

    # Create output directory, based on first available arch so armv7 or i386
    # root dir is $BUILD_DIR/iOS-${ARCH}
    # NOTE: Issue with symlinks, using cp -r will fail with to many symlinks

    mkdir "$BUILD_DIR/iOS"

    if [ ! -d "$BUILD_DIR/iOS/lib" ]; then
        if [ -d "$BUILD_DIR/iOS-i386" ]; then
            mkdir "$BUILD_DIR/iOS/lib"
            cp -r "$BUILD_DIR/iOS-i386/lib" "$BUILD_DIR/iOS";
            mkdir "$BUILD_DIR/iOS/include"
            cp -r "$BUILD_DIR/iOS-i386/include" "$BUILD_DIR/iOS";
        fi
    fi
    if [ ! -d "$BUILD_DIR/iOS/lib" ]; then
        if [ -d "$BUILD_DIR/iOS-armv7" ]; then
            mkdir "$BUILD_DIR/iOS/lib"
            cp -r "$BUILD_DIR/iOS-armv7/lib" "$BUILD_DIR/iOS";
            mkdir "$BUILD_DIR/iOS/include"
            cp -r "$BUILD_DIR/iOS-armv7/include" "$BUILD_DIR/iOS";
        fi
    fi

    for f in $FILES; do

        if [ "$IOS_BUILD_TYPE" == "device" ]; then
            lipo -create \
                    "$BUILD_DIR/iOS-arm64/lib/$f" \
                    "$BUILD_DIR/iOS-armv7/lib/$f" \
                    "$BUILD_DIR/iOS-armv7s/lib/$f" \
                    -output "$BUILD_DIR/iOS/lib/$f"
        elif [ "$IOS_BUILD_TYPE" == "simulator" ]; then
            lipo -create \
                    "$BUILD_DIR/iOS-i386/lib/$f" \
                    "$BUILD_DIR/iOS-x86_64/lib/$f" \
                    -output "$BUILD_DIR/iOS/lib/$f"
        elif [ "$IOS_BUILD_TYPE" == "all" ]; then
            lipo -create \
                    "$BUILD_DIR/iOS-i386/lib/$f" \
                    "$BUILD_DIR/iOS-x86_64/lib/$f" \
                    "$BUILD_DIR/iOS-arm64/lib/$f" \
                    "$BUILD_DIR/iOS-armv7/lib/$f" \
                    "$BUILD_DIR/iOS-armv7s/lib/$f" \
                    -output "$BUILD_DIR/iOS/lib/$f"
        else
            echo "Error invalid build type"
            exit 1
        fi

    done

    if [ -d "$BUILD_DIR/iOS-i386" ]; then rm -Rf $BUILD_DIR/iOS-i386; fi
    if [ -d "$BUILD_DIR/iOS-x86_64" ]; then rm -Rf $BUILD_DIR/iOS-x86_64; fi
    if [ -d "$BUILD_DIR/iOS-arm64" ]; then rm -Rf $BUILD_DIR/iOS-arm64; fi
    if [ -d "$BUILD_DIR/iOS-armv7" ]; then rm -Rf $BUILD_DIR/iOS-armv7; fi
    if [ -d "$BUILD_DIR/iOS-armv7s" ]; then rm -Rf $BUILD_DIR/iOS-armv7s; fi

}

# Build part

buildiOS
deployiOS

echo "OpenSSL library created. Files placed into:" $BUILD_DIR/iOS
