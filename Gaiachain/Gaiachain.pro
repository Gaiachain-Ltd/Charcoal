## Milo Solutions - project file TEMPLATE
#
#
## (c) Milo Solutions, 2016

exists(../local.pri) {
    include(../local.pri)
}

QT = core gui qml quick svg

include(../version.pri)

# Warning! QStringBuilder can crash your app! See last point here:
# https://www.kdab.com/uncovering-32-qt-best-practices-compile-time-clazy/
# !!!
DEFINES *= QT_USE_QSTRINGBUILDER
QMAKE_CXXFLAGS += -Werror

TEMPLATE = app
CONFIG += c++14
TARGET = Gaiachain

HEADERS += \
    src/models/shipmentmodel.h \
    src/models/eventmodel.h \
    src/helpers/utility.h \
    src/controllers/maincontroller.h \
    src/controllers/datamanager.h \
    src/models/commodityproxymodel.h \
    src/common/enums.h \
    src/models/commoditydaterangeproxymodel.h \
    src/common/globals.h \
    src/controllers/pagemanager.h

SOURCES += src/main.cpp \ 
    src/models/shipmentmodel.cpp \
    src/models/eventmodel.cpp \
    src/helpers/utility.cpp \
    src/controllers/maincontroller.cpp \
    src/controllers/datamanager.cpp \
    src/models/commodityproxymodel.cpp \
    src/models/commoditydaterangeproxymodel.cpp \
    src/controllers/pagemanager.cpp

RESOURCES +=  \
    qml/qml.qrc \
    resources/resources.qrc

OTHER_FILES += \
    ../Gaiachain.doxyfile \
    ../README.md \
    ../Release.md \
    ../.gitignore \
    ../license-Qt.txt \
    ../.gitlab-ci.yml

## Put all build files into build directory
##  This also works with shadow building, so don't worry!
BUILD_DIR = build
OBJECTS_DIR = $$BUILD_DIR
MOC_DIR = $$BUILD_DIR
RCC_DIR = $$BUILD_DIR
UI_DIR = $$BUILD_DIR
DESTDIR = $$BUILD_DIR/bin

## Platforms
include(platforms/android/android.pri)
include(platforms/linux/linux.pri)

## Modules
include(../milo/mconfig/mconfig.pri)
include(../milo/mlog/mlog.pri)
#include(../milo/mscripts/mscripts.pri) # TO_DO was is das?
include(../milo/mbarcodescanner/mbarcodescanner.pri)
#include(../milo/mcrypto/mcrypto.pri)  # TO_DO disabling it until potential database encryption implementation
#include(../milo/mrestapi/mrestapi.pri) # TO_DO disabling it until client server implementation

DISTFILES += \
    platforms/android/android/AndroidManifest.xml \
    platforms/android/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/android/gradlew \
    platforms/android/android/res/values/libs.xml \
    platforms/android/android/build.gradle \
    platforms/android/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/android/gradlew.bat \
    platforms/android/android/AndroidManifest.xml \
    platforms/android/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/android/gradlew \
    platforms/android/android/res/values/libs.xml \
    platforms/android/android/build.gradle \
    platforms/android/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/android/gradlew.bat \
