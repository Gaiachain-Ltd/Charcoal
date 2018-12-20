#!/bin/bash

############################################
#
# MILO @ 2016
#
# Android Package Builder
#
############################################
#
# USAGE
#
# Set following environment variables:
# QT_BIN - full path to qt binaries
# JAVA_HOME -Java implemetation main dir path
# ANDROID_SDK_ROOT - Android SDK path
# ANDROID_NDK_ROOT - Android NDK path
# ANDROID_NDK_HOST - host of cross build e.g. linux-x86_64
# ANDROID_NDK_PLATFORM - NDK platform  e.g. android-14
# ANDROID_TARGET_ARCH - target architecure e.g. armeabi-v7a
# ANDROID_NDK_TOOLCHAIN_VERSION - toolchain version e.g. 4.8
# ANDROID_API_VERSION - api version e.g. android-23
# ANDROID_TOOLS_VERSION - version of android build tools e.g. 23.0.1 (see ANDROID_SDK_ROOT/build-tools)
############################################


#on error script execution breaks
set -e

# help
function print_help() {
    echo "Help for parameters of scripts which builds APK from qt project sources";
    echo
    echo "Parameters:"
    echo "  -p path    Path to project root folder"
    echo "  -n name    pro file name without suffix e.g. for test.pro it would be -n test"
    echo "  -b path    Path to build folder (will be created if not exists)"
    echo "  -c options qmake additional config options e.g. CONFIG+=abc"
    echo "  -r         Build application in release mode"
    echo "  -k path    keystore path, mandatory to sign apk in release mode"
    echo "  -p pwd     password for keystore"
    echo "  -a path    alias used when creating keystore"
    echo
}

# if any files and folder from parameters or variables does not exist then say about it
# and set $ERROR as true
function sanity_test() {
    ERROR=0
    if [ ! -f "$QT_BIN/qmake" ]; then
        echo "qmake not found at $QT_BIN"
        ERROR=1
    fi
    if [ ! -f "$QT_BIN/androiddeployqt" ]; then
        echo "androiddeployqt not found at $QT_BIN"
        ERROR=2
    fi
    type make
    if [ $? -ne 0 ]; then
        echo "make not installed"
        ERROR=3
    fi
    if [ ! -f "$PRO_FILE" ]; then
        echo "Qt project not found: $PRO_FILE"
        ERROR=4
    fi
    if [ ! -d "$JAVA_HOME" ]; then
        echo "Java home folder not found: $JAVA_HOME"
        ERROR=5
    fi
    if [ ! -d "$ANDROID_SDK_ROOT" ]; then
        echo "Android SDK root folder not found: $ANDROID_SDK_ROOT"
        ERROR=6
    fi
    if [ ! -d "$ANDROID_NDK_ROOT" ]; then
        echo "Android NDK root folder not found: $ANDROID_NDK_ROOT"
        ERROR=7
    fi
}

# build apk and rename
function build_apk() {
    echo "Project build dir: "$PROJECT_RELEASE_BUILD
    rm -rf $PROJECT_RELEASE_BUILD
    mkdir -p $PROJECT_RELEASE_BUILD
    cd $PROJECT_RELEASE_BUILD
    $QT_BIN/qmake $PRO_FILE -r -spec android-g++ $CONFIG_OPTIONS
    make
    make INSTALL_ROOT=$PROJECT_RELEASE_BUILD/android-build install

    $QT_BIN/androiddeployqt --input $PROJECT_RELEASE_BUILD/android-lib$PROJECT_NAME.so-deployment-settings.json --output $PROJECT_RELEASE_BUILD/android-build --deployment bundled --android-platform $ANDROID_API_VERSION --jdk $JAVA_HOME --gradle $DEPLOY_OPTIONS
    
    # rename to real name
    if [ -z $DEPLOY_OPTIONS ]; then
        APK_NAME=android-build-debug
    else
        APK_NAME=android-build-release-unsigned
    fi
    DATE=$(date +%Y%m%d)
    OUTPUT_DIR=$PROJECT_RELEASE_BUILD/android-build/build/outputs/apk
    mv $OUTPUT_DIR/$APK_NAME.apk $OUTPUT_DIR/$PROJECT_NAME-$DATE.apk
}

# sign apk
function sign_apk() {
   cd $OUTPUT_DIR
   jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore $KEYSTORE_PATH -storepass $PASSWORD $PROJECT_NAME-$DATE.apk $ALIAS
   zipalign -v 4 $1 $PROJECT_NAME-$DATE.apk
}

# reading parameters from command line
while getopts ":hp:n:b:rk:p:a:" opt ;
do
    case $opt in
        h) print_help
            exit 0
            ;;
        p) PROJECT_ROOT=$OPTARG
            ;;
        n) PROJECT_NAME=$OPTARG
           PRO_FILE=$PROJECT_ROOT/$PROJECT_NAME.pro
            ;;
        b) PROJECT_RELEASE_BUILD=$OPTARG
            ;;
        c) CONFIG_OPTIONS=$OPTARG
            ;;
        r) DEPLOY_OPTIONS="--release"
            ;;
        k) KEYSTORE_PATH=$OPTARG
            ;;
        p) PASSWORD=$OPTARG
            ;;
        a) ALIAS=$OPTARG
            ;;
        o) TOKEN=$OPTARG
            ;;
        *) echo "Wrong parameter"
           echo "For help run $0 -h"
           exit 1
            ;;
    esac
done

# Execution

sanity_test
if [ $ERROR -eq 1 ]; then
  echo
  print_help
  exit $ERROR
fi

BUILD_TOOLS_DIR=$ANDROID_SDK_ROOT/build-tools/$ANDROID_TOOLS_VERSION
export PATH=$JAVA_HOME/bin:$ANDROID_SDK_ROOT/tools:$ANDROID_SDK_ROOT/platform-tools:$ANDROID_NDK_ROOT:$BUILD_TOOLS_DIR:$PATH

build_apk
if [ ! -z $DEPLOY_OPTIONS ]; then
    sign_apk
fi
