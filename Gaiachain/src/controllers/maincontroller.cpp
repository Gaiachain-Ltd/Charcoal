#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#include <QZXing.h>

#ifdef Q_OS_ANDROID
#include "androidpermissionshandler.h"
#endif

#include "../common/enums.h"
#include "../common/packagedata.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"
#include "../helpers/modelhelper.h"
#include "../helpers/packagedataproperties.h"
#include "../helpers/keywordfilterproxymodel.h"

#ifdef EASY_LOGIN
#include "../common/dummy/commondummydata.h"
#endif

template <typename Singleton>
QObject *registerCppOwnershipSingletonType(QQmlEngine *, QJSEngine *)
{
    QQmlEngine::setObjectOwnership(&Singleton::instance(), QQmlEngine::CppOwnership);
    return &Singleton::instance();
}

MainController::MainController(QObject *parent)
    : AbstractManager(parent),
      m_application(new Application(this)),
      m_languageManager(new LanguageManager(this))
{
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

    setupConnections();

    m_languageManager->load();
}

void MainController::setupConnections()
{
    connect(&m_dbManager, &DatabaseManager::databaseReady, &m_dataManager, &DataManager::setupDatabase);

    connect(&m_userManager, &UserManager::offlineModeChanged,
            &m_sessionManager, [sessionManager = &m_sessionManager](bool offlineMode) { sessionManager->setEnabled(!offlineMode); });
    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &AbstractSessionManager::updateToken);
    connect(&m_sessionManager, &AbstractSessionManager::loginAttempt, &m_userManager, &UserManager::handleLoginAttempt);
    connect(&m_sessionManager, &AbstractSessionManager::loginFinished, &m_userManager, &UserManager::readLoginData);

    connect(&m_userManager, &UserManager::userDataChanged, &m_dataManager, &DataManager::updateUserData);

    setupDataConnections();
}

void MainController::setupDataConnections()
{
    connect(&m_dataManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));
    connect(&m_dataManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));
    connect(&m_dataManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));

    connect(&m_sessionManager, &AbstractSessionManager::entitySaved,
            &m_dataManager, &DataManager::onActionAdded);
    connect(&m_sessionManager, &AbstractSessionManager::entitySaveError,
            &m_dataManager, &DataManager::onActionAddError);

    connect(&m_sessionManager, &AbstractSessionManager::connectionStateChanged,
            &m_dataManager, [dataManager = &m_dataManager, userManager = &m_userManager, pageManager = &m_pageManager](Enums::ConnectionState connectionState) {
        if (connectionState == Enums::ConnectionState::ConnectionSuccessful &&
                !userManager->isOfflineMode() &&
                pageManager->topPage() != Enums::Page::Login &&
                pageManager->topPage() != Enums::Page::LoginLoading) {
            dataManager->sendOfflineActions();
        }
    });

    connect(&m_dataManager, qOverload<const QDateTime &, const QDateTime &>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<const QDateTime &, const QDateTime &>(&AbstractSessionManager::getEntitiesInfo));
    connect(&m_dataManager, qOverload<int, int, const QDateTime &, const QDateTime &>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<int, int, const QDateTime &, const QDateTime &>(&AbstractSessionManager::getEntitiesInfo));
    connect(&m_dataManager, qOverload<int, int, const QString &, const QSet<Enums::PackageType> &, int>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<int, int, const QString &, const QSet<Enums::PackageType> &, int>(&AbstractSessionManager::getEntitiesInfo));
    connect(&m_dataManager, &DataManager::lastActionEventsInfoNeeded, &m_sessionManager, &AbstractSessionManager::getLastActionEntitiesInfo);
    connect(&m_dataManager, &DataManager::eventsNeeded, &m_sessionManager, &AbstractSessionManager::getEntities);
    connect(&m_dataManager, &DataManager::lastActionEventsInfoNeeded, &m_sessionManager, &AbstractSessionManager::getLastActionEntitiesInfo);

    connect(&m_sessionManager, &AbstractSessionManager::entitiesInfoLoaded, &m_dataManager, &DataManager::onEntitiesInfoLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::entitiesLoaded, &m_dataManager, &DataManager::onEntitiesLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::additionalDataLoaded, &m_dataManager, &DataManager::onAdditionalDataLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::unusedLotIdsLoaded, &m_dataManager, &DataManager::onUnusedLotIdsLoaded);
}

void MainController::initialWork()
{
#ifdef Q_OS_ANDROID
    QMetaObject::invokeMethod(&Android::PermissionsHandler::instance(),
                              std::bind(&Android::PermissionsHandler::requestPermissions, &Android::PermissionsHandler::instance(),
                                        QList<Android::PermissionsHandler::Permissions>{ Android::PermissionsHandler::Permissions::Internet,
                                                                                         Android::PermissionsHandler::Permissions::Storage }));
#endif
    m_dbManager.setupDatabase();
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    // register namespace for enums
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");

    qRegisterMetaType<Enums::Page>("Enums::Page");
    qRegisterMetaType<Enums::Popup>("Enums::Popup");
    qRegisterMetaType<Enums::PopupAction>("Enums::PopupAction");
    qRegisterMetaType<Enums::ConnectionState>("Enums::ConnectionState");
    qRegisterMetaType<Enums::UserType>("Enums::UserType");
    qRegisterMetaType<Enums::PackageType>("Enums::PackageType");
    qRegisterMetaType<Enums::SupplyChainAction>("Enums::SupplyChainAction");
    qRegisterMetaType<Gaia::ModelData>("Gaia::ModelData");
    qRegisterMetaType<Gaia::ModelEntry>("Gaia::ModelEntry");

    // register other types
    qRegisterMetaType<PackageData>("PackageData");
    qmlRegisterUncreatableType<PackageData>("com.gaiachain.types", 1, 0, "PackageData", "Cannot create types in QML");

    // register singleton types
    qmlRegisterSingletonType(QUrl("qrc:///GStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
#ifdef COCOA
    qmlRegisterSingletonType(QUrl("qrc:///cocoa/CocoaStyle.qml"), "com.gaiachain.style", 1, 0, "GStyle");
#endif
#ifdef CHARCOAL
    qmlRegisterSingletonType(QUrl("qrc:///charcoal/CharcoalStyle.qml"), "com.gaiachain.style", 1, 0, "GStyle");
#endif
    qmlRegisterSingletonType(QUrl("qrc:///GStatic.qml"), "com.gaiachain.static", 1, 0, "Static");
    qmlRegisterSingletonType(QUrl("qrc:///common/Helper.qml"), "com.gaiachain.helpers", 1, 0, "Helper");

    qmlRegisterSingletonType<Utility>("com.gaiachain.helpers", 1, 0,
                                      "Utility", &registerCppOwnershipSingletonType<Utility>);
    qmlRegisterSingletonType<RequestsHelper>("com.gaiachain.helpers", 1, 0,
                                             "RequestHelper", &registerCppOwnershipSingletonType<RequestsHelper>);
    qmlRegisterSingletonType<DataGlobals>("com.gaiachain.helpers", 1, 0,
                                          "DataGlobals", &registerCppOwnershipSingletonType<DataGlobals>);
    qmlRegisterSingletonType<ModelHelper>("com.gaiachain.helpers", 1, 0,
                                          "ModelHelper", &registerCppOwnershipSingletonType<ModelHelper>);
    qmlRegisterSingletonType<PackageDataProperties>("com.gaiachain.types", 1, 0,
                                                    "PackageDataProperties", &registerCppOwnershipSingletonType<PackageDataProperties>);

#ifdef Q_OS_ANDROID
    qmlRegisterSingletonType<Android::PermissionsHandler>("com.gaiachain.platforms", 1, 0,
                                                          "AndroidPermissionsHandler", &registerCppOwnershipSingletonType<Android::PermissionsHandler>);
#else
    qmlRegisterModule("com.gaiachain.platforms", 1, 0);
#endif

    // register qml types
    qmlRegisterType<KeywordFilterProxyModel>("com.gaiachain.helpers", 1, 0, "KeywordFilterProxyModel");

    // add context property
    engine.rootContext()->setContextProperty(QStringLiteral("mainController"), this);

    // setup other components
    m_languageManager->connectQmlEngine(&engine);
    m_pageManager.setupQmlContext(engine);
    m_userManager.setupQmlContext(engine);
    m_dbManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
    m_sessionManager.setupQmlContext(engine);
    m_picturesManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::startInitialWork()
{
    QMetaObject::invokeMethod(this, &MainController::initialWork);
}

QString MainController::flavor() const
{
#ifdef COCOA
    return QStringLiteral("cocoa");
#elif CHARCOAL
    return QStringLiteral("charcoal");
#else
    return QString();
#endif
}

bool MainController::isDesktop() const
{
#ifdef DESKTOP_TESTS
    return true;
#else
    return false;
#endif
}

bool MainController::isEasyLogin() const
{
#ifdef EASY_LOGIN
    return true;
#else
    return false;
#endif
}

QStringList MainController::easyLoginList() const
{
#ifdef EASY_LOGIN
    return CommonDummyData::availableLogins();
#else
    return QStringList();
#endif
}

QString MainController::easyLoginPassword() const
{
#ifdef EASY_LOGIN
    return CommonDummyData::commonPassword();
#else
    return QString();
#endif
}

Application *MainController::application() const
{
    return m_application;
}

LanguageManager *MainController::languageManager() const
{
    return m_languageManager;
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}

