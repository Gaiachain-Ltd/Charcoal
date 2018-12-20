include(../mlog.pri)

exists(../../../tests/testConfig.pri) {
    include(../../../tests/testConfig.pri)
} else {
    warning("File testConfig.pri was not included")
}

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_mlog.cpp \
    loggingthread.cpp

HEADERS += \
    loggingthread.h
