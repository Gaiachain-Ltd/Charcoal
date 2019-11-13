#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

#include "../common/enums.h"
#include "../common/packagedata.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"

#include <QZXing.h>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
#endif

#ifdef USE_COMBOBOX
#include "session/dummy/fakedatapopulator.h"
#endif

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
    QLocale::setDefault(QLocale("en_GB"));

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

    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &AbstractSessionManager::updateToken);
    connect(&m_sessionManager, &AbstractSessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData);
    connect(&m_sessionManager, &AbstractSessionManager::entitiesLoaded, &m_dataManager, &DataManager::onEntitiesLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::packagesRelationsLoaded, &m_dataManager, &DataManager::onRelationsLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::additionalDataLoaded, &m_dataManager, &DataManager::onAdditionalDataLoaded);
    connect(&m_sessionManager, &AbstractSessionManager::unusedLotIdsLoaded, &m_dataManager, &DataManager::onUnusedLotIdsLoaded);

    connect(&m_userManager, &UserManager::cooperativeIdChanged, &m_dataManager, &DataManager::updateCooperativeId);
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
    qmlRegisterSingletonType(QUrl("qrc:///common/Helpers.qml"), "com.gaiachain.helpers", 1, 0, "Helpers");

    // add context properties
#ifdef USE_COMBOBOX
    engine.rootContext()->setContextProperty(QStringLiteral("fakeLogins"), FakeDataPopulator::availableLogins());
#endif
    engine.rootContext()->setContextProperty(QStringLiteral("Utility"), Utility::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("DataGlobals"), DataGlobals::instance());

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

