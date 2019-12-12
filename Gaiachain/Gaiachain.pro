## Milo Solutions - project file TEMPLATE
#
#
## (c) Milo Solutions, 2016

QT += core gui qml quick svg sql

exists(../local.pri) {
    include(../local.pri)
}

use_combobox {
    DEFINES+=USE_COMBOBOX
}

fake_data {
    DEFINES+=FAKE_DATA
}

release_server {
    DEFINES+=RELEASE_SERVER
}

english {
    DEFINES+=ENGLISH_LANGUAGE
}

include(../version.pri)

# qzxing
CONFIG += qzxing_qml
CONFIG += qzxing_multimedia
QZXING_PATH=$$PWD/3rdparty/qzxing

INCLUDEPATH += $$QZXING_PATH
include($$QZXING_PATH/QZXing.pri)

# Warning! QStringBuilder can crash your app! See last point here:
# https://www.kdab.com/uncovering-32-qt-best-practices-compile-time-clazy/
# !!!
DEFINES *= QT_USE_QSTRINGBUILDER
QMAKE_CXXFLAGS += -Wno-deprecated-copy # because of QTBUG-75210

TEMPLATE = app
CONFIG += c++17
TARGET = Gaiachain

HEADERS += \
    src/common/dataglobals.h \
    src/common/dummy/commondummydata.h \
    src/common/locale.h \
    src/common/types.h \
    src/common/userdata.h \
    src/controllers/data/abstractdatamodelsmanager.h \
    src/controllers/data/datamanager.h \
    src/controllers/data/datamodelsmanager.h \
    src/controllers/data/datarequestsmanager.h \
    src/controllers/data/dataviewmodelsmanager.h \
    src/controllers/databasemanager.h \
    src/controllers/offlineusershandler.h \
    src/controllers/session/abstractsessionmanager.h \
    src/database/dbhelpers.h \
    src/database/dbmigrationmanager.h \
    src/database/dbmigrations.h \
    src/helpers/cryptohelper.h \
    src/helpers/modelhelper.h \
    src/helpers/packagedataproperties.h \
    src/helpers/requestshelper.h \
    src/helpers/typetraits.h \
    src/models/abstractidentityproxymodel.h \
    src/models/abstractmodel.h \
    src/models/abstractreadmodel.h \
    src/models/companymodel.h \
    src/models/cooperativeeventsproxymodel.h \
    src/models/parcelmodel.h \
    src/models/proxy/abstractsortfilterqueryextension.h \
    src/models/proxy/companytypequery.h \
    src/models/proxy/isortfilterquery.h \
    src/models/proxy/produceridquery.h \
    src/models/proxy/sortfilterquery.h \
    src/models/proxy/sortnamequery.h \
    src/models/proxy/sqlquerymodel.h \
    src/models/sqltablemodel.h \
    src/models/daterangeproxymodel.h \
    src/models/existsquerymodel.h \
    src/models/latesteventsproxymodel.h \
    src/models/latestrangeeventsproxymodel.h \
    src/models/localonlyproxymodel.h \
    src/models/modelchangedextension.h \
    src/models/namemodel.h \
    src/models/packagedataproxymodel.h \
    src/models/packagelastactionproxymodel.h \
    src/models/packagetypeeventsproxymodel.h \
    src/models/eventmodel.h \
    src/helpers/utility.h \
    src/controllers/maincontroller.h \
    src/common/enums.h \
    src/common/globals.h \
    src/controllers/pagemanager.h \
    src/models/packagetypeidsproxymodel.h \
    src/models/packagetypeproxymodel.h \
    src/models/producermodel.h \
    src/models/relationmodel.h \
    src/models/relationslistproxymodel.h \
    src/models/searcheventsproxymodel.h \
    src/common/packagedata.h \
    src/models/selectedidsproxymodel.h \
    src/models/unusedidsmodel.h \
    src/models/views/abstractmodelview.h \
    src/models/views/parcelviewmodel.h \
    src/rest/additionaldatarequest.h \
    src/rest/authrequest.h \
    src/rest/baserequest.h \
    src/rest/restapiclient.h \
    src/controllers/session/sessionmanager.h \
    src/controllers/usermanager.h \
    src/controllers/abstractmanager.h \
    src/models/abstractsortfilterproxymodel.h \
    src/common/tags.h \
    src/rest/entityrequest.h \
    src/common/logs.h

SOURCES += src/main.cpp \
    src/common/dummy/commondummydata.cpp \
    src/common/logs.cpp \
    src/common/packagedata.cpp \
    src/common/dataglobals.cpp \
    src/common/globals.cpp \
    src/common/types.cpp \
    src/common/userdata.cpp \
    src/controllers/data/abstractdatamodelsmanager.cpp \
    src/controllers/data/datamanager.cpp \
    src/controllers/data/datamodelsmanager.cpp \
    src/controllers/data/datarequestsmanager.cpp \
    src/controllers/data/dataviewmodelsmanager.cpp \
    src/controllers/databasemanager.cpp \
    src/controllers/offlineusershandler.cpp \
    src/controllers/session/abstractsessionmanager.cpp \
    src/database/dbhelpers.cpp \
    src/database/dbmigrationmanager.cpp \
    src/database/dbmigrations.cpp \
    src/helpers/cryptohelper.cpp \
    src/helpers/modelhelper.cpp \
    src/helpers/packagedataproperties.cpp \
    src/helpers/requestshelper.cpp \
    src/models/abstractidentityproxymodel.cpp \
    src/models/abstractmodel.cpp \
    src/models/abstractreadmodel.cpp \
    src/models/companymodel.cpp \
    src/models/cooperativeeventsproxymodel.cpp \
    src/models/parcelmodel.cpp \
    src/models/proxy/sortfilterquery.cpp \
    src/models/proxy/sqlquerymodel.cpp \
    src/models/sqltablemodel.cpp \
    src/models/daterangeproxymodel.cpp \
    src/models/existsquerymodel.cpp \
    src/models/latesteventsproxymodel.cpp \
    src/models/latestrangeeventsproxymodel.cpp \
    src/models/localonlyproxymodel.cpp \
    src/models/modelchangedextension.cpp \
    src/models/namemodel.cpp \
    src/models/packagedataproxymodel.cpp \
    src/models/packagelastactionproxymodel.cpp \
    src/models/packagetypeeventsproxymodel.cpp \
    src/models/eventmodel.cpp \
    src/helpers/utility.cpp \
    src/controllers/maincontroller.cpp \
    src/controllers/pagemanager.cpp \
    src/models/packagetypeidsproxymodel.cpp \
    src/models/packagetypeproxymodel.cpp \
    src/models/producermodel.cpp \
    src/models/relationmodel.cpp \
    src/models/relationslistproxymodel.cpp \
    src/models/searcheventsproxymodel.cpp \
    src/models/selectedidsproxymodel.cpp \
    src/models/unusedidsmodel.cpp \
    src/models/views/abstractmodelview.cpp \
    src/models/views/parcelviewmodel.cpp \
    src/rest/additionaldatarequest.cpp \
    src/rest/authrequest.cpp \
    src/rest/baserequest.cpp \
    src/rest/restapiclient.cpp \
    src/controllers/session/sessionmanager.cpp \
    src/controllers/usermanager.cpp \
    src/rest/entityrequest.cpp \
    src/controllers/abstractmanager.cpp \
    src/models/abstractsortfilterproxymodel.cpp

RESOURCES +=  \
    qml/qml.qrc \
    resources/resources.qrc \
    translations/translations.qrc

OTHER_FILES += \
    ../Gaiachain.doxyfile \
    ../README.md \
    ../Release.md \
    ../.gitignore \
    ../license-Qt.txt \
    ../.gitlab-ci.yml

fake_data {
    HEADERS += \
        src/controllers/session/dummy/fakeserver.h \
        src/controllers/session/dummy/fakeserverstate.h \
        src/controllers/session/dummy/fakesessionmanager.h \
        src/controllers/session/dummy/fakedatapopulator.h

    SOURCES += \
        src/controllers/session/dummy/fakeserver.cpp \
        src/controllers/session/dummy/fakeserverstate.cpp \
        src/controllers/session/dummy/fakesessionmanager.cpp \
        src/controllers/session/dummy/fakedatapopulator.cpp
}

INCLUDEPATH += $$PWD/3rdparty/OpenSSL/OpenSSL/include

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

CONFIG += no-openssl    # disable openssl for mcrypto until issue #82497
## Modules
include(../milo/mconfig/mconfig.pri)
include(../milo/mlog/mlog.pri)
#include(../milo/mscripts/mscripts.pri) # TODO disabling it until potential usecase
include(../milo/mcrypto/mcrypto.pri)
include(../milo/mrestapi/mrestapi.pri)

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
