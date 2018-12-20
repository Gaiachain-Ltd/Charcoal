QT = core
CONFIG += c++11

# Just add this line to your project to include MiloLog!
include(../mlog.pri)

TARGET = example-log
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    exampleclass.h

