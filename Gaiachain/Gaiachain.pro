## (c) Milo Solutions, 2016-2020

QT += core gui qml quick svg sql location

exists(../local.pri) {
    include(../local.pri)
}

easy_login {
    DEFINES+=EASY_LOGIN
}

release_server {
    DEFINES+=RELEASE_SERVER
}

!cocoa:!charcoal {
    error("Specify application flavor!")
}

cocoa:charcoal {
    error("Can't compile BOTH cocoa and charcoal flavors! Pick one")
}

cocoa {
    DEFINES += COCOA
    RESOURCES += qml/cocoa/cocoa.qrc
    TARGET = GaiachainCocoa

    HEADERS += \
        src/cocoa/cocoauserdata.h \
        src/cocoa/cocoasessionmanager.h

    SOURCES = \
        src/cocoa/cocoamigrations.cpp \
        src/cocoa/cocoasessionmanager.cpp
}

charcoal {
    DEFINES += CHARCOAL
    RESOURCES += \
        qml/charcoal/charcoal.qrc \
        resources/resources-charcoal.qrc
    TARGET = GaiachainCharcoal

    HEADERS += \
        src/charcoal/charcoaluserdata.h \
        src/charcoal/picturesmanager.h \
        src/charcoal/tickmarkiconprovider.h \
        src/charcoal/notificationmanager.h \
        src/charcoal/charcoalsessionmanager.h

    SOURCES += \
        src/charcoal/picturesmanager.cpp \
        src/charcoal/tickmarkiconprovider.cpp \
        src/charcoal/notificationmanager.cpp \
        src/charcoal/charcoalmigrations.cpp \
        src/charcoal/charcoalsessionmanager.cpp
}

include(../version.pri)
include(3rdparty/OpenSSL/openssl.pri)

# qzxing
CONFIG += qzxing_qml
CONFIG += qzxing_multimedia
QZXING_PATH=$$PWD/3rdparty/qzxing/src

INCLUDEPATH += $$QZXING_PATH
include($$QZXING_PATH/QZXing.pri)

# Warning! QStringBuilder can crash your app! See last point here:
# https://www.kdab.com/uncovering-32-qt-best-practices-compile-time-clazy/
# !!!
DEFINES *= QT_USE_QSTRINGBUILDER

msvc* {
    QMAKE_CXXFLAGS += /WX

    lto {
        QMAKE_CXXFLAGS += /GL
        QMAKE_LFLAGS += /LTCG
    }
} else {
    QMAKE_CXXFLAGS += -Werror
    # issue with application/x-sharedlib vs application/x-executable, without it
    # sharedlib is produced
    !android{
        QMAKE_LFLAGS += -no-pie
    }

    lto {
        QMAKE_CXXFLAGS += -flto
        QMAKE_LFLAGS += -flto
    }
}

TEMPLATE = app
CONFIG += c++17

INCLUDEPATH += src

HEADERS += \
    src/cocoa/cocoadatamanager.h \
    src/common/dataglobals.h \
    src/common/dummy/commondummydata.h \
    src/common/languagemanager.h \
    src/common/types.h \
    src/common/abstractuserdata.h \
    src/controllers/data/abstractdatamanager.h \
    src/controllers/data/abstractdatamodelsmanager.h \
    src/controllers/data/datalocalmanager.h \
    src/controllers/data/datamodelsmanager.h \
    src/controllers/data/datarequestsmanager.h \
    src/controllers/data/dataviewmanager.h \
    src/controllers/databasemanager.h \
    src/controllers/offlineusershandler.h \
    src/controllers/session/abstractsessionmanager.h \
    src/controllers/session/restsessionmanager.h \
    src/database/dbhelpers.h \
    src/database/dbmigrationmanager.h \
    src/database/dbmigrations.h \
    src/helpers/cryptohelper.h \
    src/helpers/keywordfilterproxymodel.h \
    src/helpers/modelhelper.h \
    src/helpers/packagedataproperties.h \
    src/helpers/requestshelper.h \
    src/helpers/typetraits.h \
    src/models/proxy/abstractidentityproxymodel.h \
    src/models/abstractmodel.h \
    src/models/abstractreadmodel.h \
    src/models/companymodel.h \
    src/models/parcelmodel.h \
    src/models/query/abstractsortfilterqueryextension.h \
    src/models/query/companytypequery.h \
    src/models/query/cooperativeidquery.h \
    src/models/query/daterangequery.h \
    src/models/query/idkeywordquery.h \
    src/models/query/isortfilterquery.h \
    src/models/query/localeventsquery.h \
    src/models/query/packagetypequery.h \
    src/models/query/produceridquery.h \
    src/models/query/sortfilterquery.h \
    src/models/query/sortnamequery.h \
    src/models/query/sorttimestampquery.h \
    src/models/query/sqlquerymodel.h \
    src/models/sqltablemodel.h \
    src/models/existsquerymodel.h \
    src/models/proxy/latestrangeeventsproxymodel.h \
    src/models/modelchangedextension.h \
    src/models/namemodel.h \
    src/models/proxy/packagelastactionproxymodel.h \
    src/models/proxy/packagetypeeventsproxymodel.h \
    src/models/eventmodel.h \
    src/helpers/utility.h \
    src/controllers/maincontroller.h \
    src/common/enums.h \
    src/common/globals.h \
    src/controllers/pagemanager.h \
    src/models/producermodel.h \
    src/models/relationmodel.h \
    src/common/packagedata.h \
    src/models/unusedidsmodel.h \
    src/models/views/abstractmodelview.h \
    src/models/views/calendarviewmodel.h \
    src/models/views/cooperativeviewmodel.h \
    src/models/views/localviewmodel.h \
    src/models/views/packagerelationsviewmodel.h \
    src/models/views/packageviewmodel.h \
    src/models/views/parcelviewmodel.h \
    src/models/views/transactionsviewmodel.h \
    src/rest/additionaldatarequest.h \
    src/rest/authrequest.h \
    src/rest/baserequest.h \
    src/rest/restapiclient.h \
    src/controllers/usermanager.h \
    src/controllers/abstractmanager.h \
    src/models/proxy/abstractsortfilterproxymodel.h \
    src/common/tags.h \
    src/rest/entityrequest.h \
    src/common/logs.h

SOURCES += src/main.cpp \
    src/cocoa/cocoadatamanager.cpp \
    src/common/dummy/commondummydata.cpp \
    src/common/languagemanager.cpp \
    src/common/logs.cpp \
    src/common/packagedata.cpp \
    src/common/dataglobals.cpp \
    src/common/globals.cpp \
    src/common/types.cpp \
    src/common/abstractuserdata.cpp \
    src/controllers/data/abstractdatamanager.cpp \
    src/controllers/data/abstractdatamodelsmanager.cpp \
    src/controllers/data/datalocalmanager.cpp \
    src/controllers/data/datamodelsmanager.cpp \
    src/controllers/data/datarequestsmanager.cpp \
    src/controllers/data/dataviewmanager.cpp \
    src/controllers/databasemanager.cpp \
    src/controllers/offlineusershandler.cpp \
    src/controllers/session/abstractsessionmanager.cpp \
    src/controllers/session/restsessionmanager.cpp \
    src/database/dbhelpers.cpp \
    src/database/dbmigrationmanager.cpp \
    src/database/dbmigrations.cpp \
    src/helpers/cryptohelper.cpp \
    src/helpers/keywordfilterproxymodel.cpp \
    src/helpers/modelhelper.cpp \
    src/helpers/packagedataproperties.cpp \
    src/helpers/requestshelper.cpp \
    src/models/proxy/abstractidentityproxymodel.cpp \
    src/models/abstractmodel.cpp \
    src/models/abstractreadmodel.cpp \
    src/models/companymodel.cpp \
    src/models/parcelmodel.cpp \
    src/models/query/sortfilterquery.cpp \
    src/models/query/sqlquerymodel.cpp \
    src/models/sqltablemodel.cpp \
    src/models/existsquerymodel.cpp \
    src/models/proxy/latestrangeeventsproxymodel.cpp \
    src/models/modelchangedextension.cpp \
    src/models/namemodel.cpp \
    src/models/proxy/packagelastactionproxymodel.cpp \
    src/models/proxy/packagetypeeventsproxymodel.cpp \
    src/models/eventmodel.cpp \
    src/helpers/utility.cpp \
    src/controllers/maincontroller.cpp \
    src/controllers/pagemanager.cpp \
    src/models/producermodel.cpp \
    src/models/relationmodel.cpp \
    src/models/unusedidsmodel.cpp \
    src/models/views/abstractmodelview.cpp \
    src/models/views/calendarviewmodel.cpp \
    src/models/views/cooperativeviewmodel.cpp \
    src/models/views/localviewmodel.cpp \
    src/models/views/packagerelationsviewmodel.cpp \
    src/models/views/packageviewmodel.cpp \
    src/models/views/parcelviewmodel.cpp \
    src/models/views/transactionsviewmodel.cpp \
    src/rest/additionaldatarequest.cpp \
    src/rest/authrequest.cpp \
    src/rest/baserequest.cpp \
    src/rest/restapiclient.cpp \
    src/controllers/usermanager.cpp \
    src/rest/entityrequest.cpp \
    src/controllers/abstractmanager.cpp \
    src/models/proxy/abstractsortfilterproxymodel.cpp

RESOURCES +=  \
    qml/common.qrc \
    resources/resources.qrc \
    translations/translations.qrc

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
#include(../milo/mscripts/mscripts.pri) # TODO disabling it until potential usecase
include(../milo/mcrypto/mcrypto.pri)
include(../milo/mrestapi/mrestapi.pri)

asan {
    message("Address sanitizer: enabled. Use only in debug builds")
    LIBS += -lcurl
    CONFIG += sanitizer sanitize_address
    QMAKE_CXXFLAGS+="-fsanitize=address -fno-omit-frame-pointer"
    QMAKE_CFLAGS+="-fsanitize=address -fno-omit-frame-pointer"
    QMAKE_LFLAGS+="-fsanitize=address"
    CONFIG += ubsan
}

msan {
    message("Memory sanitizer: enabled. Use only in debug builds.")
    LIBS += -lcurl
    QMAKE_CXXFLAGS+="-fsanitize=memory"
    QMAKE_LFLAGS+="-fsanitize=memory"
    CONFIG += ubsan
}

tsan {
    message("Thread sanitizer: enabled. Use only in debug builds")
    LIBS += -lcurl
    QMAKE_CXXFLAGS+="-fsanitize=thread"
    QMAKE_LFLAGS+="-fsanitize=thread"
    CONFIG += ubsan
}

ubsan {
    message("Undefined behavior sanitizer: enabled. Use only in debug builds")
    QMAKE_CXXFLAGS+="-fsanitize=undefined"
    QMAKE_LFLAGS+="-fsanitize=undefined"
}
