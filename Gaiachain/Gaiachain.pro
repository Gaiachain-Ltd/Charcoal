## Milo Solutions - project file TEMPLATE
#
#
## (c) Milo Solutions, 2016

QT = core

include(../version.pri)

# Warning! QStringBuilder can crash your app! See last point here:
# https://www.kdab.com/uncovering-32-qt-best-practices-compile-time-clazy/
# !!!
DEFINES *= QT_USE_QSTRINGBUILDER
QMAKE_CXXFLAGS += -Werror

TEMPLATE = app
CONFIG += c++14
TARGET = Gaiachain

HEADERS +=

SOURCES += src/main.cpp 

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

## Modules
include(../milo/mbarcodescanner/mbarcodescanner.pri)
include(../milo/mconfig/mconfig.pri)
include(../milo/mcrypto/mcrypto.pri)
include(../milo/mlog/mlog.pri)
include(../milo/mrestapi/mrestapi.pri)
include(../milo/mscripts/mscripts.pri)
