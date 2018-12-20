## Milo Solutions - project file template
#
#
## (c) Milo Solutions, 2016

include(../mrestapi.pri)

QT += core network

## Set up application version
VERSION=1.0
## Add version define. You can now use this in C++ code:
##  QString someString(APP_VERSION);
DEFINES+= APP_VERSION=\\\"$$VERSION\\\"

CONFIG -= app_bundle

TEMPLATE = app

CONFIG += c++11

DEFINES *= QT_USE_QSTRINGBUILDER

TARGET = restapicommunication

INCLUDEPATH += restCommunicationStuff

HEADERS += \
    mrBabu/address.h \
    mrBabu/addressptr.h \
    mrBabu/session.h \
    mrBabu/user.h \
    mrBabu/userptr.h \
    restCommunicationStuff/restapiclient.h \
    restCommunicationStuff/mrbaburequest.h \
    restCommunicationStuff/mrbaburequests/loginrequest.h \
    restCommunicationStuff/mrbaburequests/logoutrequest.h \
    restCommunicationStuff/mrbaburequests/registerrequest.h \
    restCommunicationStuff/mrbaburequests/userinforequest.h \
    restCommunicationStuff/openweathermaprequest.h \
    restCommunicationStuff/mrbaburequestconfig.h \
    restCommunicationStuff/openweathermaprequestconfig.h \
    restCommunicationStuff/openweathermaprequests/weatherbycitynamerequest.h \
    openWeatherMap/weather.h \
	config.h


SOURCES += main.cpp \
    mrBabu/address.cpp \
    mrBabu/session.cpp \
    mrBabu/user.cpp \
    restCommunicationStuff/restapiclient.cpp \
    restCommunicationStuff/mrbaburequest.cpp \
    restCommunicationStuff/mrbaburequests/loginrequest.cpp \
    restCommunicationStuff/mrbaburequests/logoutrequest.cpp \
    restCommunicationStuff/mrbaburequests/registerrequest.cpp \
    restCommunicationStuff/mrbaburequests/userinforequest.cpp \
    restCommunicationStuff/openweathermaprequest.cpp \
    restCommunicationStuff/mrbaburequestconfig.cpp \
    restCommunicationStuff/openweathermaprequestconfig.cpp \
    restCommunicationStuff/openweathermaprequests/weatherbycitynamerequest.cpp \
    openWeatherMap/weather.cpp \
	config.cpp


## Put all build files into build directory
##  This also works with shadow building, so don't worry!
BUILD_DIR = build
OBJECTS_DIR = $$BUILD_DIR
MOC_DIR = $$BUILD_DIR
RCC_DIR = $$BUILD_DIR
UI_DIR = $$BUILD_DIR
DESTDIR = $$BUILD_DIR/bin

production-server {
    DEFINES += USE_PRODUCTION_SERVER
}
