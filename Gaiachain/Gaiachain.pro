## Milo Solutions - project file TEMPLATE
#
#
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
  DEFINES+=COCOA
  RESOURCES+=qml/cocoa/cocoa.qrc
}

charcoal {
  DEFINES+=CHARCOAL
  RESOURCES+=qml/charcoal/charcoal.qrc
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
TARGET = Gaiachain

HEADERS += \
    src/common/dataglobals.h \
    src/common/dummy/commondummydata.h \
    src/common/locale.h \
    src/common/types.h \
    src/common/userdata.h \
    src/controllers/data/abstractdatamodelsmanager.h \
    src/controllers/data/datalocalmanager.h \
    src/controllers/data/datamanager.h \
    src/controllers/data/datamodelsmanager.h \
    src/controllers/data/datarequestsmanager.h \
    src/controllers/data/dataviewmanager.h \
    src/controllers/databasemanager.h \
    src/controllers/offlineusershandler.h \
    src/controllers/session/abstractsessionmanager.h \
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
    src/controllers/session/sessionmanager.h \
    src/controllers/usermanager.h \
    src/controllers/abstractmanager.h \
    src/models/proxy/abstractsortfilterproxymodel.h \
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
    src/controllers/data/datalocalmanager.cpp \
    src/controllers/data/datamanager.cpp \
    src/controllers/data/datamodelsmanager.cpp \
    src/controllers/data/datarequestsmanager.cpp \
    src/controllers/data/dataviewmanager.cpp \
    src/controllers/databasemanager.cpp \
    src/controllers/offlineusershandler.cpp \
    src/controllers/session/abstractsessionmanager.cpp \
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
    src/controllers/session/sessionmanager.cpp \
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

INCLUDEPATH += $$PWD/3rdparty/OpenSSL/OpenSSL/include
# NOTE: Variable for MCrypto
OPENSSL_PATH = $$PWD/3rdparty/OpenSSL/OpenSSL

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

#CONFIG += no-openssl    # disable openssl for mcrypto until issue #82497
## Modules
include(../milo/mconfig/mconfig.pri)
include(../milo/mlog/mlog.pri)
#include(../milo/mscripts/mscripts.pri) # TODO disabling it until potential usecase
include(../milo/mcrypto/mcrypto.pri)
include(../milo/mrestapi/mrestapi.pri)

DISTFILES += \
    qml/charcoal/CharcoalStyle.qml \
    qml/cocoa/CocoaStyle.qml \
    qml/cocoa/components/CalendarItem.qml \
    qml/cocoa/components/CalendarItemGridExtension.qml \
    qml/cocoa/components/CalendarMonthItem.qml \
    qml/cocoa/components/CalendarNavigator.qml \
    qml/cocoa/components/CalendarPackageEventsBars.qml \
    qml/cocoa/components/CalendarPageItem.qml \
    qml/cocoa/components/CalendarSupplyItem.qml \
    qml/cocoa/components/CalendarWeekItem.qml \
    qml/cocoa/components/CalendarYearItem.qml \
    qml/cocoa/components/ChangeAnimation.qml \
    qml/cocoa/components/CommunicationStateItem.qml \
    qml/cocoa/components/ConnectionStateItem.qml \
    qml/cocoa/components/EventsListView.qml \
    qml/cocoa/components/Footer.qml \
    qml/cocoa/components/MenuButton.qml \
    qml/cocoa/components/NavigationHeader.qml \
    qml/cocoa/components/OfflineModeHandler.qml \
    qml/cocoa/components/PackageDataDetails.qml \
    qml/cocoa/components/PackageDataHarvestDetails.qml \
    qml/cocoa/components/PackageDataLotDetails.qml \
    qml/cocoa/components/PackageDataSacDetails.qml \
    qml/cocoa/components/SupplyChainCategorySubmenu.qml \
    qml/cocoa/components/SupplyChainCategorySubmenuButton.qml \
    qml/cocoa/components/SupplyChainSubmenu.qml \
    qml/cocoa/components/SupplyChainSubmenuButton.qml \
    qml/cocoa/components/SupplyChainSubmenuExpandButton.qml \
    qml/cocoa/components/TodayIndicator.qml \
    qml/cocoa/pages/CalendarEventsPage.qml \
    qml/cocoa/pages/CalendarPage.qml \
    qml/cocoa/pages/CalendarPageBase.qml \
    qml/cocoa/pages/CalendarYearPage.qml \
    qml/cocoa/pages/GPage.qml \
    qml/cocoa/pages/LoginLoadingPage.qml \
    qml/cocoa/pages/LoginPage.qml \
    qml/cocoa/pages/MainMenuPage.qml \
    qml/cocoa/pages/MapPage.qml \
    qml/cocoa/pages/PackageDataPage.qml \
    qml/cocoa/pages/QRScannerPage.qml \
    qml/cocoa/pages/SupplyChainAddHarvestIdPage.qml \
    qml/cocoa/pages/SupplyChainBaggingPage.qml \
    qml/cocoa/pages/SupplyChainExportReceptionPage.qml \
    qml/cocoa/pages/SupplyChainGrainProcessingPage.qml \
    qml/cocoa/pages/SupplyChainHarvestPage.qml \
    qml/cocoa/pages/SupplyChainLotCreationPage.qml \
    qml/cocoa/pages/SupplyChainPage.qml \
    qml/cocoa/pages/SupplyChainPageBase.qml \
    qml/cocoa/pages/SupplyChainSectionReceptionPage.qml \
    qml/cocoa/pages/SupplyChainSummaryPage.qml \
    qml/cocoa/pages/SupplyChainWarehouseTransportPage.qml \
    qml/cocoa/pages/TransactionsPage.qml \
    qml/cocoa/pages/WaitingTransactionsPage.qml \
    qml/cocoa/popups/AboutPopup.qml \
    qml/cocoa/popups/ButtonPopup.qml \
    qml/cocoa/popups/CommentPopup.qml \
    qml/cocoa/popups/ConfirmPopup.qml \
    qml/cocoa/popups/GPopup.qml \
    qml/cocoa/popups/InformationPopup.qml \
    qml/cocoa/popups/NotificationPopup.qml \
    qml/cocoa/popups/TextPopup.qml \
    qml/cocoa/popups/TimedPopup.qml \
    qml/cocoa/popups/WaitOverlayPopup.qml \
    qml/cocoa/popups/YesNoQuestionPopup.qml
