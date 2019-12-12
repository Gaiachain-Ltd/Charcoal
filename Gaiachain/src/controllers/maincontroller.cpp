#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#include <QZXing.h>

#include "../common/enums.h"
#include "../common/packagedata.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"
#include "../helpers/modelhelper.h"
#include "../helpers/packagedataproperties.h"

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
#endif

#ifdef USE_COMBOBOX
#include "../common/dummy/commondummydata.h"
#endif
#ifdef FAKE_DATA
#include "session/dummy/fakeserverstate.h"
#endif

template <typename Singleton>
QObject *registerCppOwnershipSingletonType(QQmlEngine *, QJSEngine *)
{
    QQmlEngine::setObjectOwnership(&Singleton::instance(), QQmlEngine::CppOwnership);
    return &Singleton::instance();
}

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

    setupConnections();
}

void MainController::setupConnections()
{
    connect(&m_dbManager, &DatabaseManager::databaseReady, &m_dataManager, &DataManager::setupDatabase);

    connect(&m_userManager, &UserManager::offlineModeChanged,
            &m_sessionManager, [sessionManager = &m_sessionManager](bool offlineMode) { sessionManager->setEnabled(!offlineMode); });
    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &AbstractSessionManager::updateToken);
    connect(&m_userManager, &UserManager::loggedIn, &m_sessionManager, &AbstractSessionManager::getAdditionalData);
    connect(&m_sessionManager, &AbstractSessionManager::loginAttempt, &m_userManager, &UserManager::handleLoginAttempt);
    connect(&m_sessionManager, &AbstractSessionManager::loginFinished, &m_userManager, &UserManager::readLoginData);

    connect(&m_userManager, &UserManager::userDataChanged, &m_dataManager, &DataManager::updateUserData);

    setupDataConnections();
}

void MainController::setupDataConnections()
{
    connect(&m_dataManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));
    connect(&m_dataManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));
    connect(&m_dataManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&AbstractSessionManager::postNewEntity));

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
    connect(&m_dataManager, qOverload<int, int, const QString &>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<int, int, const QString &>(&AbstractSessionManager::getEntitiesInfo));
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
    m_dbManager.setupDatabase();
}

#ifdef Q_OS_ANDROID
void MainController::setupAppPermissions()
{
    const auto appPermissions = QStringList{
            "android.permission.ACCESS_COARSE_LOCATION",
            "android.permission.ACCESS_FINE_LOCATION",
            "android.permission.CAMERA",
            "android.permission.INTERNET",
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"
    };

    // TODO: shift some permissions only after login
    // check for permissions before opening scanner page to load camera faster
    auto permissionsToRequest = QStringList{};
    std::copy_if(appPermissions.constBegin(), appPermissions.constEnd(),
                 std::back_inserter(permissionsToRequest), [](const auto &permission) {
        return QtAndroid::checkPermission(permission) == QtAndroid::PermissionResult::Denied;
    });

    QtAndroid::requestPermissions(permissionsToRequest, [](const QtAndroid::PermissionResultMap &) {});
}
#endif

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("isDesktop",
#ifdef DESKTOP_TESTS
                                             true);
#else
                                             false);
#endif

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
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:///common/Helper.qml"), "com.gaiachain.helpers", 1, 0, "Helper");

    qmlRegisterSingletonType<Utility>("com.gaiachain.helpers", 1, 0, "Utility", &registerCppOwnershipSingletonType<Utility>);
    qmlRegisterSingletonType<RequestsHelper>("com.gaiachain.helpers", 1, 0, "RequestHelper", &registerCppOwnershipSingletonType<RequestsHelper>);
    qmlRegisterSingletonType<DataGlobals>("com.gaiachain.helpers", 1, 0, "DataGlobals", &registerCppOwnershipSingletonType<DataGlobals>);
    qmlRegisterSingletonType<ModelHelper>("com.gaiachain.modelhelper", 1, 0, "ModelHelper", &registerCppOwnershipSingletonType<ModelHelper>);
    qmlRegisterSingletonType<PackageDataProperties>("com.gaiachain.packagedata", 1, 0, "PackageDataProperties", &registerCppOwnershipSingletonType<PackageDataProperties>);

    // add context properties
    engine.rootContext()->setContextProperty(QStringLiteral("AppName"), AppName);
    engine.rootContext()->setContextProperty(QStringLiteral("AppDomain"), AppDomain);
    engine.rootContext()->setContextProperty(QStringLiteral("AppVersion"), AppVersion);
    engine.rootContext()->setContextProperty(QStringLiteral("GitCommit"), GitCommit);

#ifdef USE_COMBOBOX
    engine.rootContext()->setContextProperty(QStringLiteral("dummyLogins"), CommonDummyData::availableLogins());
    engine.rootContext()->setContextProperty(QStringLiteral("dummyPassword"), CommonDummyData::commonPassword());
#endif
#ifdef FAKE_DATA
    engine.rootContext()->setContextProperty(QStringLiteral("fakeServer"), &FakeServerState::instance());
#endif

    // setup other components
    m_pageManager.setupQmlContext(engine);
    m_userManager.setupQmlContext(engine);
    m_dbManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
    m_sessionManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::startInitialWork()
{
    QMetaObject::invokeMethod(this, &MainController::initialWork);
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}

