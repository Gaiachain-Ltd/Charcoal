#!/bin/bash

############################################
#
# MILO @ 2016
#
# Milo Barcode compiler - script for building Milo Barcode shared library
#
############################################
#
# USAGE
#
# Set following optional environment variables:
# ANDROID_NDK_TOOLCHAIN_VERSION - toolchain version. e.g.: export ANDROID_NDK_TOOLCHAIN_VERSION=4.9
#
# EXAMPLE USAGE
# ./scripts/build_barcode_libraries.sh -p /home/USER/workspace/ZainTablet/sandbox/MiloBarcode/ -n MiloBarcode -d ../build-MiloBarcode-Desktop -a ../build-MiloBarcode-Android -l /opt/Qt5.6.0_android/5.6/gcc_64/bin -m /opt/Qt5.6.0_android/5.6/android_armv7/bin/
############################################


#on error script execution breaks
set -e

# help
function print_help() {
    echo "Help for parameters of scripts which builds APK from qt project sources";
    echo
    echo "Parameters:"
    echo "  -p path     Path to project root folder"
    echo "  -n name     pro file name without suffix e.g. for test.pro it would be -n test"
    echo "  -d path     Path to desktop build folder (will be created if not exists)"
    echo "  -a path     Path to android build folder (will be created if not exists)"
    echo "  -t path     Target folder to copy apk (If no set then copies to: $PROJECT_RELEASE_BUILD/bin/)"
    echo "  -c options  qmake additional config options e.g. CONFIG+=abc"
    echo "  -r          Build application in release mode"
    echo "  -s path     Android SDK path"
    echo "  -k path     Android NDK path"
    echo "  -l path    Qt desktop bin path (the one with qmake)"
    echo "  -m path    Qt android bin path (the one with qmake)"
    echo
}

# if any files and folder from parameters or variables does not exist then say about it
# and set $ERROR as true
function test_for_files_and_paths() {
    ERROR=0;
    if [ ! -f "$QT_BIN_DESKTOP/qmake" ]; then
        echo
        echo "Error:"
        echo "Qmake file not found: $QT_BIN_DESKTOP/qmake";
        ERROR=1
    fi
    if [ ! -f "$PRO_FILE" ]; then
        echo
        echo "Error:"
        echo "Qt project not found: $PRO_FILE";
        ERROR=1
    fi
    if [ ! -d "$ANDROID_SDK_ROOT" ]; then
        echo
        echo "Error:"
        echo "Android SDK root folder not found: $ANDROID_SDK_ROOT";
        ERROR=1
    fi
    if [ ! -d "$ANDROID_NDK_ROOT" ]; then
        echo
        echo "Error:"
        echo "Android NDK root folder not found: $ANDROID_NDK_ROOT";
        ERROR=1
    fi
}

# core function
function build_desktop() {
	echo "Building Desktop libraries"
	echo "Project build dir: "$PROJECT_DESKTOP_RELEASE_BUILD
    rm -rf $PROJECT_DESKTOP_RELEASE_BUILD
    mkdir -p $PROJECT_DESKTOP_RELEASE_BUILD
    cd $PROJECT_DESKTOP_RELEASE_BUILD
    $QT_BIN_DESKTOP/qmake $PRO_FILE -r -spec linux-g++ $CONFIG_OPTIONS
    make -j4
    cd ..
}

function build_android() {
	echo "Building Android libraries"
    echo "Project build dir: "$PROJECT_ANDROID_RELEASE_BUILD
    rm -rf $PROJECT_ANDROID_RELEASE_BUILD
    mkdir -p $PROJECT_ANDROID_RELEASE_BUILD
    cd $PROJECT_ANDROID_RELEASE_BUILD
    $QT_BIN_ANDROID/qmake $PRO_FILE -r -spec android-g++ $CONFIG_OPTIONS
    make -j4
    cd ..
}
# reading parameters from command line
while getopts ":hp:n:d:a:c:r:k:s:m:l:" opt ;
do
    case $opt in
	h) print_help;
        exit 0;
        ;;
        p) PROJECT_ROOT=$OPTARG
        ;;
        n) PROJECT_NAME=$OPTARG
	   PRO_FILE=$PROJECT_ROOT/$PROJECT_NAME.pro
	    ;;
        d) PROJECT_DESKTOP_RELEASE_BUILD=$OPTARG
        ;;
        a) PROJECT_ANDROID_RELEASE_BUILD=$OPTARG
        ;;
        c) CONFIG_OPTIONS=$OPTARG
	    ;;
        r) DEPLOY_OPTIONS="--release"
	    ;;
        s) ANDROID_SDK_ROOT=$OPTARG
	    ;;
        k) ANDROID_NDK_ROOT=$OPTARG
	    ;;
        m) QT_BIN_ANDROID=$OPTARG
	    ;;
        l) QT_BIN_DESKTOP=$OPTARG
	    ;;
        *) echo "Wrong parameter";
           echo "For help run $0 -h";
           exit 1
            ;;
    esac
done

# Execution

test_for_files_and_paths;
if [ $ERROR -eq 1 ]; then
  echo
  print_help;
  exit 1;
fi

export ANDROID_NDK_ROOT=$ANDROID_NDK_ROOT

build_desktop;
build_android;
