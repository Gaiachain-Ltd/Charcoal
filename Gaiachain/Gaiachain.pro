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

defined(app_storage_password) {
    error(You have to provide app-specific password! Do it like this: qmake app_storage_password=\"ABCDEF\")
} else {
    DEFINES += APP_STORAGE_PASSWORD='"\\\"$$app_storage_password\\\""'
    #message(App password is: $$app_storage_password)
}

cocoa {
    DEFINES += COCOA
    RESOURCES += qml/cocoa/cocoa.qrc
    TARGET = GaiachainCocoa

    HEADERS += \
        src/cocoa/cocoauserdata.h \
        src/cocoa/cocoasessionmanager.h \
        src/cocoa/cocoadatamanager.h \
        src/cocoa/common/packagedata.h \
        src/cocoa/controllers/data/abstractdatamodelsmanager.h \
        src/cocoa/controllers/data/datalocalmanager.h \
        src/cocoa/controllers/data/datamodelsmanager.h \
        src/cocoa/controllers/data/datarequestsmanager.h \
        src/cocoa/controllers/data/dataviewmanager.h \
        src/cocoa/helpers/keywordfilterproxymodel.h \
        src/cocoa/helpers/modelhelper.h \
        src/cocoa/models/abstractmodel.h \
        src/cocoa/models/abstractreadmodel.h \
        src/cocoa/models/companymodel.h \
        src/cocoa/models/parcelmodel.h \
        src/cocoa/models/modelchangedextension.h \
        src/cocoa/models/sqltablemodel.h \
        src/cocoa/models/existsquerymodel.h \
        src/cocoa/models/namemodel.h \
        src/cocoa/models/eventmodel.h \
        src/cocoa/models/producermodel.h \
        src/cocoa/models/relationmodel.h \
        src/cocoa/models/unusedidsmodel.h \
        src/cocoa/models/query/abstractsortfilterqueryextension.h \
        src/cocoa/models/query/companytypequery.h \
        src/cocoa/models/query/cooperativeidquery.h \
        src/cocoa/models/query/daterangequery.h \
        src/cocoa/models/query/idkeywordquery.h \
        src/cocoa/models/query/isortfilterquery.h \
        src/cocoa/models/query/localeventsquery.h \
        src/cocoa/models/query/packagetypequery.h \
        src/cocoa/models/query/produceridquery.h \
        src/cocoa/models/query/sortfilterquery.h \
        src/cocoa/models/query/sortnamequery.h \
        src/cocoa/models/query/sorttimestampquery.h \
        src/cocoa/models/query/sqlquerymodel.h \
        src/cocoa/models/proxy/abstractidentityproxymodel.h \
        src/cocoa/models/proxy/latestrangeeventsproxymodel.h \
        src/cocoa/models/proxy/packagelastactionproxymodel.h \
        src/cocoa/models/proxy/packagetypeeventsproxymodel.h \
        src/cocoa/models/proxy/abstractsortfilterproxymodel.h \
        src/cocoa/models/views/abstractmodelview.h \
        src/cocoa/models/views/calendarviewmodel.h \
        src/cocoa/models/views/cooperativeviewmodel.h \
        src/cocoa/models/views/localviewmodel.h \
        src/cocoa/models/views/packagerelationsviewmodel.h \
        src/cocoa/models/views/packageviewmodel.h \
        src/cocoa/models/views/parcelviewmodel.h \
        src/cocoa/models/views/transactionsviewmodel.h \
        src/cocoa/rest/entityrequest.h \

    SOURCES = \
        src/cocoa/cocoamigrations.cpp \
        src/cocoa/cocoasessionmanager.cpp \
        src/cocoa/cocoadatamanager.cpp \
        src/cocoa/common/packagedata.cpp \
        src/cocoa/controllers/data/abstractdatamodelsmanager.cpp \
        src/cocoa/controllers/data/datalocalmanager.cpp \
        src/cocoa/controllers/data/datamodelsmanager.cpp \
        src/cocoa/controllers/data/datarequestsmanager.cpp \
        src/cocoa/controllers/data/dataviewmanager.cpp \
        src/cocoa/helpers/keywordfilterproxymodel.cpp \
        src/cocoa/helpers/modelhelper.cpp \
        src/cocoa/models/proxy/abstractidentityproxymodel.cpp \
        src/cocoa/models/abstractmodel.cpp \
        src/cocoa/models/abstractreadmodel.cpp \
        src/cocoa/models/companymodel.cpp \
        src/cocoa/models/parcelmodel.cpp \
        src/cocoa/models/sqltablemodel.cpp \
        src/cocoa/models/existsquerymodel.cpp \
        src/cocoa/models/modelchangedextension.cpp \
        src/cocoa/models/namemodel.cpp \
        src/cocoa/models/eventmodel.cpp \
        src/cocoa/models/producermodel.cpp \
        src/cocoa/models/relationmodel.cpp \
        src/cocoa/models/unusedidsmodel.cpp \
        src/cocoa/models/query/sortfilterquery.cpp \
        src/cocoa/models/query/sqlquerymodel.cpp \
        src/cocoa/models/proxy/latestrangeeventsproxymodel.cpp \
        src/cocoa/models/proxy/packagelastactionproxymodel.cpp \
        src/cocoa/models/proxy/packagetypeeventsproxymodel.cpp \
        src/cocoa/models/proxy/abstractsortfilterproxymodel.cpp \
        src/cocoa/models/views/abstractmodelview.cpp \
        src/cocoa/models/views/calendarviewmodel.cpp \
        src/cocoa/models/views/cooperativeviewmodel.cpp \
        src/cocoa/models/views/localviewmodel.cpp \
        src/cocoa/models/views/packagerelationsviewmodel.cpp \
        src/cocoa/models/views/packageviewmodel.cpp \
        src/cocoa/models/views/parcelviewmodel.cpp \
        src/cocoa/models/views/transactionsviewmodel.cpp \
        src/cocoa/rest/entityrequest.cpp \

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
        src/charcoal/charcoalsessionmanager.h \
        src/charcoal/charcoaldatamanager.h \
        src/charcoal/components/trapezoidovenitem.h \
        src/charcoal/database/charcoaldbhelpers.h \
        src/charcoal/models/dbobjects.h \
        src/charcoal/models/actioncontroller.h \
        src/charcoal/models/treespeciesmodel.h \
        src/charcoal/models/parcelsmodel.h \
        src/charcoal/models/villagesmodel.h \
        src/charcoal/models/destinationsmodel.h \
        src/charcoal/models/oventypesmodel.h \
        src/charcoal/models/unusedplotidsforcarbonizationmodel.h \
        src/charcoal/models/unusedplotidsforloggingendingmodel.h \
        src/charcoal/models/unusedplotidsforreplantationmodel.h \
        src/charcoal/models/unusedharvestidsmodelforcarbonizationending.h \
        src/charcoal/models/unusedharvestidsmodelfortransport.h \
        src/charcoal/models/unusedtransportidsmodel.h \
        src/charcoal/models/querymodel.h \
        src/charcoal/models/simplelistquerymodel.h \
        src/charcoal/models/ovensmodel.h \
        src/charcoal/models/trackingmodel.h \
        src/charcoal/models/trackingfilterproxymodel.h \
        src/charcoal/models/minimumdatemodel.h \
        src/charcoal/models/localeventsmodel.h \
        src/charcoal/models/listupdater.h \
        src/charcoal/models/trackingupdater.h \
        src/charcoal/models/replantationssender.h \
        src/charcoal/models/eventssender.h \
        src/charcoal/rest/multipartrequest.h \
        src/charcoal/rest/imagerequest.h \

    SOURCES += \
        src/charcoal/picturesmanager.cpp \
        src/charcoal/tickmarkiconprovider.cpp \
        src/charcoal/notificationmanager.cpp \
        src/charcoal/charcoalmigrations.cpp \
        src/charcoal/charcoalsessionmanager.cpp \
        src/charcoal/charcoaldatamanager.cpp \
        src/charcoal/components/trapezoidovenitem.cpp \
        src/charcoal/database/charcoaldbhelpers.cpp \
        src/charcoal/models/dbobjects.cpp \
        src/charcoal/models/actioncontroller.cpp \
        src/charcoal/models/treespeciesmodel.cpp \
        src/charcoal/models/parcelsmodel.cpp \
        src/charcoal/models/villagesmodel.cpp \
        src/charcoal/models/destinationsmodel.cpp \
        src/charcoal/models/oventypesmodel.cpp \
        src/charcoal/models/unusedplotidsforcarbonizationmodel.cpp \
        src/charcoal/models/unusedplotidsforloggingendingmodel.cpp \
        src/charcoal/models/unusedplotidsforreplantationmodel.cpp \
        src/charcoal/models/unusedharvestidsmodelforcarbonizationending.cpp \
        src/charcoal/models/unusedharvestidsmodelfortransport.cpp \
        src/charcoal/models/unusedtransportidsmodel.cpp \
        src/charcoal/models/querymodel.cpp \
        src/charcoal/models/simplelistquerymodel.cpp \
        src/charcoal/models/ovensmodel.cpp \
        src/charcoal/models/trackingmodel.cpp \
        src/charcoal/models/trackingfilterproxymodel.cpp \
        src/charcoal/models/minimumdatemodel.cpp \
        src/charcoal/models/localeventsmodel.cpp \
        src/charcoal/models/listupdater.cpp \
        src/charcoal/models/trackingupdater.cpp \
        src/charcoal/models/replantationssender.cpp \
        src/charcoal/models/eventssender.cpp \
        src/charcoal/rest/multipartrequest.cpp \
        src/charcoal/rest/imagerequest.cpp \

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
    #QMAKE_CXXFLAGS += /WX

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
    src/common/dataglobals.h \
    src/common/dummy/commondummydata.h \
    src/common/languagemanager.h \
    src/common/types.h \
    src/common/abstractuserdata.h \
    src/common/enums.h \
    src/common/globals.h \
    src/common/tags.h \
    src/common/logs.h \
    src/controllers/data/abstractdatamanager.h \
    src/controllers/databasemanager.h \
    src/controllers/offlineusershandler.h \
    src/controllers/session/abstractsessionmanager.h \
    src/controllers/session/restsessionmanager.h \
    src/controllers/maincontroller.h \
    src/controllers/pagemanager.h \
    src/controllers/usermanager.h \
    src/controllers/abstractmanager.h \
    src/database/dbhelpers.h \
    src/database/dbmigrationmanager.h \
    src/database/dbmigrations.h \
    src/helpers/packagedataproperties.h \
    src/helpers/cryptohelper.h \
    src/helpers/requestshelper.h \
    src/helpers/typetraits.h \
    src/helpers/utility.h \
    src/rest/additionaldatarequest.h \
    src/rest/authrequest.h \
    src/rest/baserequest.h \
    src/rest/restapiclient.h \

SOURCES += src/main.cpp \
    src/common/dummy/commondummydata.cpp \
    src/common/languagemanager.cpp \
    src/common/logs.cpp \
    src/common/dataglobals.cpp \
    src/common/globals.cpp \
    src/common/types.cpp \
    src/common/abstractuserdata.cpp \
    src/controllers/data/abstractdatamanager.cpp \
    src/controllers/databasemanager.cpp \
    src/controllers/offlineusershandler.cpp \
    src/controllers/session/abstractsessionmanager.cpp \
    src/controllers/session/restsessionmanager.cpp \
    src/controllers/maincontroller.cpp \
    src/controllers/pagemanager.cpp \
    src/controllers/usermanager.cpp \
    src/controllers/abstractmanager.cpp \
    src/database/dbhelpers.cpp \
    src/database/dbmigrationmanager.cpp \
    src/database/dbmigrations.cpp \
    src/helpers/packagedataproperties.cpp \
    src/helpers/cryptohelper.cpp \
    src/helpers/requestshelper.cpp \
    src/helpers/utility.cpp \
    src/rest/additionaldatarequest.cpp \
    src/rest/authrequest.cpp \
    src/rest/baserequest.cpp \
    src/rest/restapiclient.cpp \

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
