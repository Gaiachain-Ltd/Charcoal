#!/bin/bash

############################################
#
# MILO @ 2016
#
# iOS Package Builder
#
############################################ 
#
# USAGE
#
# Set following environment variables:
# QT_BIN - full path to qt binaries
#


function print_help() {
    echo "iOS Package Builder";
    echo "MILO @ 2016"
    echo
    echo "Usage: $0 -p PATH -n NAME -b PATH [-c CONFIG][-r][-s NAME]"
    echo "Parameters:"
    echo "  -p path    Path to project root folder"
    echo "  -n name    pro file name without suffix e.g. for test.pro it would be -n test"
    echo "  -b path    Path to build folder (will be created if not exists)"
    echo "  -c options qmake additional config options e.g. CONFIG+=abc"
    echo "  -s name    signing identity name"
    exho "  -v name    provisioning profile name"
    echo
}

# check if tools and paths are valid
# will stop on first error
function sanity_check() {
  
  if [ -z $QT_BIN ]; then
    echo "QT_BIN not set"
    ERROR=1; return
  fi
  
  if [ ! -x $QT_BIN/qmake ]; then
   echo "Cannot run qmake"
   ERROR=2; return
  fi
  
  type xcodebuild
  if [ $? -ne 0 ]; then
     echo "Cannot run xcodebuild"
     ERROR=3; return
  fi
  
  if [ -z $PROJECT_ROOT ]; then
    echo "PROJECT_ROOT not set"
    ERROR=4; return
  fi
  
  if [ ! -d $PROJECT_ROOT ]; then
    echo "Cannot find $PROJECT_ROOT"
    ERROR=5; return
  fi
  
  if [ -z $PROJECT_NAME ]; then
    echo "PROJECT_NAME not set"
    ERROR=6; return
  fi
  
  if [ ! -e $PRO_FILE ]; then
    echo "Cannot find $PRO_FILE"
    ERROR=7; return
  fi
  
  if [ -z $SIGNING_CERTIFICATE ]; then
    echo "SIGNING_CERTIFICATE not set"
    ERROR=8; return
  fi

  echo "Sanity check passed"
}

function build_qt_app() {
  echo "Building app"
  rm -rf $BUILD_PATH
  mkdir -p $BUILD_PATH
  cd $BUILD_PATH
  $QT_BIN/qmake $PRO_FILE -r -spec macx-ios-clang CONFIG+=iphoneos $CONFIG_OPTIONS
  xcodebuild -scheme $PROJECT_NAME -sdk iphoneos -archivePath $BUILD_PATH/$PROJECT_NAME.xcarchive archive
}

function send_on_testflight() {
    echo "Sign and upload"
    if [ -z SIGNING_IDENTITY ]; then
        echo "Signing Identity not provided"
        exit 20
    fi
    if [ -z PROVISIONING_PROFILE ]; then
        echo "Provisioning Profile not provided"
        exit 21
    fi
    xcodebuild -exportArchive -archivePath $BUILD_PATH/$PROJECT_NAME.xcarchive -exportOptionsPlist $PROJECT_ROOT/ios/Info.plist -exportPath $BUILD_PATH -exportSigningIdentity $SIGNING_IDENTITY -exportProfisioningProfile $PROVISIONING_PROFILE
}


#main
ERROR=0
CONFIGURATION="Debug"
while getopts ":hp:n:b:c:s:v:" opt ;
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
        b) BUILD_PATH=$OPTARG
            ;;
        c) CONFIG_OPTIONS=$OPTARG
            ;;
        s) SIGNING_IDENTITY=$OPTARG
            ;;
        v) PROVISIONING_PROFILE=$OPTARG
            ;;
        *) echo "Wrong parameter";
           echo "For help run $0 -h";
           exit 1
            ;;
    esac
done

sanity_check
if [ $ERROR != 0 ]; then
  exit $ERROR
fi

OBJROOT=$BUILD_PATH/obj
SYMROOT=$BUILD_PATH/sym
DSTROOT=$BUILD_PATH/archive
build_qt_app
send_on_testflight