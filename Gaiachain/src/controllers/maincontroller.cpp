#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

#include "../common/enums.h"
#include "../common/packagedata.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"
#include "../helpers/modelhelper.h"
#include "../helpers/packagedataproperties.h"

#include <QZXing.h>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
#endif

#ifdef USE_COMBOBOX
#include "session/dummy/fakedatapopulator.h"
#endif
#ifdef FAKE_DATA
#include "session/dummy/fakeserverstate.h"
#endif

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
    QLocale::setDefault(QLocale("en_GB"));

    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

#ifdef Q_OS_ANDROID
    // check for permissions before opening scanner page to load camera faster
    // TODO user it only after login
    const QString cameraPermission = QStringLiteral("android.permission.CAMERA");
    if (QtAndroid::checkPermission(cameraPermission) == QtAndroid::PermissionResult::Denied) {
        auto permissionCallback = [](const QtAndroid::PermissionResultMap &) {};
        QtAndroid::requestPermissions(QStringList() << cameraPermission, permissionCallback);
    }
#endif
    setupConnections();
}

void MainController::setupConnections()
{
    connect(&m_dbManager, &DatabaseManager::databaseReady, &m_dataManager, &DataManager::setupModels);

    connect(&m_userManager, &UserManager::loggedIn, &m_sessionManager, &AbstractSessionManager::getInitialData);
    connect(&m_userManager, &UserManager::loggedIn, &m_dataManager, &DataManager::getInitialData);
    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &AbstractSessionManager::updateToken);
    connect(&m_sessionManager, &AbstractSessionManager::loginAttempt, &m_userManager, &UserManager::handleLoginAttempt);
    connect(&m_sessionManager, &AbstractSessionManager::loginFinished, &m_userManager, &UserManager::readLoginData);

    connect(&m_userManager, &UserManager::userDataChanged, &m_dataManager, &DataManager::updateUserData);

    setupDataConnections();
}

void MainController::setupDataConnections()
{
    connect(&m_dataManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &, const QByteArray &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QString &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &, const QByteArray &>(&AbstractSessionManager::putEntityAction));
    connect(&m_dataManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&DataManager::addActionRequest),
            &m_sessionManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &>(&AbstractSessionManager::putEntityAction));
    connect(&m_dataManager, qOverload<const Enums::SupplyChainAction &,
            const QDateTime &, const QVariantMap &, const QByteArray &>(&DataManager::addActionRequest),
            &m_sessionManager, &AbstractSessionManager::postNewEntity);
    connect(&m_sessionManager, &AbstractSessionManager::entitySaved,
            &m_dataManager, &DataManager::onActionAdded);
    connect(&m_sessionManager, &AbstractSessionManager::entitySaveError,
            &m_dataManager, &DataManager::onActionAddError);

    connect(&m_sessionManager, &AbstractSessionManager::connectionStateChanged,
            &m_dataManager, [dataManager = &m_dataManager, userManager = &m_userManager](Enums::ConnectionState connectionState) {
        if (connectionState == Enums::ConnectionState::ConnectionSuccessful
                && !userManager->isOfflineMode()) {
            dataManager->sendOfflineActions();
        }
    });
    connect(&m_userManager, &UserManager::loggedIn,
            &m_dataManager, [dataManager = &m_dataManager, userManager = &m_userManager]() {
        if (!userManager->isOfflineMode()) {
            dataManager->sendOfflineActions();
        }
    });

    connect(&m_dataManager, qOverload<const QDateTime &, const QDateTime &, const QString &>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<const QDateTime &, const QDateTime &, const QString &>(&AbstractSessionManager::getEntitiesInfo));
    connect(&m_dataManager, qOverload<int, const QDateTime &, const QString &>(&DataManager::eventsInfoNeeded),
            &m_sessionManager, qOverload<int, const QDateTime &, const QString &>(&AbstractSessionManager::getEntitiesInfo));
    connect(&m_dataManager, &DataManager::lastActionEventsInfoNeeded, &m_sessionManager, &AbstractSessionManager::getLastActionEntitiesInfo);
    connect(&m_dataManager, &DataManager::eventsNeeded, &m_sessionManager, &AbstractSessionManager::getEntities);
    connect(&m_dataManager, &DataManager::relationsNeeded,
            &m_sessionManager, qOverload<const QStringList &>(&AbstractSessionManager::getRelations));

    connect(&m_sessionManager, &AbstractSessionManager::entitiesInfoLoaded, &m_dataManager, &DataManager::onEntitiesInfoLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::entitiesLoaded, &m_dataManager, &DataManager::onEntitiesLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::relationsLoaded, &m_dataManager, &DataManager::onRelationsLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::additionalDataLoaded, &m_dataManager, &DataManager::onAdditionalDataLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::unusedLotIdsLoaded, &m_dataManager, &DataManager::onUnusedLotIdsLoaded);
}

void MainController::initialWork()
{
    m_dbManager.setupDatabase();
}

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

    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::Popup>("Popup");
    qRegisterMetaType<Enums::PopupAction>("PopupAction");
    qRegisterMetaType<Enums::ConnectionState>("ConnectionState");
    qRegisterMetaType<Enums::UserType>("UserType");
    qRegisterMetaType<Enums::PlaceType>("PlaceType");
    qRegisterMetaType<Enums::PackageType>("PackageType");
    qRegisterMetaType<Enums::SupplyChainAction>("SupplyChainAction");

    // register other types
    qRegisterMetaType<PackageData>("PackageData");
    qmlRegisterUncreatableType<PackageData>("com.gaiachain.types", 1, 0, "PackageData", "Cannot create types in QML");

    // register singleton types
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:///common/Helper.qml"), "com.gaiachain.helpers", 1, 0, "Helper");

    qmlRegisterSingletonType<RequestsHelper>("com.gaiachain.helpers", 1, 0, "RequestHelper",
                                             [](QQmlEngine *, QJSEngine *) -> QObject * { return &RequestsHelper::instance(); });
    qmlRegisterSingletonType<ModelHelper>("com.gaiachain.modelhelper", 1, 0, "ModelHelper",
                                                    [](QQmlEngine *, QJSEngine *) -> QObject* { return &ModelHelper::instance(); });

    qmlRegisterSingletonType<PackageDataProperties>("com.gaiachain.packagedata", 1, 0, "PackageDataProperties",
                                                    [](QQmlEngine *, QJSEngine *) -> QObject* { return &PackageDataProperties::instance(); });

    qmlRegisterSingletonType<Utility>("com.gaiachain.helpers", 1, 0, "Utility",
                                                    [](QQmlEngine *, QJSEngine *) -> QObject* { return &Utility::instance(); });

    qmlRegisterSingletonType<DataGlobals>("com.gaiachain.helpers", 1, 0, "DataGlobals",
                                                    [](QQmlEngine *, QJSEngine *) -> QObject* { return &DataGlobals::instance(); });

    // add context properties
#ifdef USE_COMBOBOX
    engine.rootContext()->setContextProperty(QStringLiteral("fakeLogins"), FakeDataPopulator::availableLogins());
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

