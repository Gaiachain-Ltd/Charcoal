#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

#include "../common/enums.h"
#include "../common/dataglobals.h"
#include "../helpers/utility.h"

#include <QZXing.h>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
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
    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &AbstractSessionManager::onTokenChanged);
    connect(&m_sessionManager, &AbstractSessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData, Qt::DirectConnection);
    connect(&m_sessionManager, &AbstractSessionManager::entityLoaded, &m_dataManager, &DataManager::onEntityLoaded, Qt::DirectConnection);
    connect(&m_sessionManager, &AbstractSessionManager::entitiesLoaded, &m_dataManager, &DataManager::onEntitiesLoaded, Qt::DirectConnection);
    connect(&m_sessionManager, &AbstractSessionManager::beforeGetEntity, &m_dataManager, &DataManager::clearModels, Qt::DirectConnection);
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("isDesktop",
#ifdef DESKTOP_TESTS
                                             true);
#else
                                             false);
#endif

    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");

    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::UserType>("UserType");
    qRegisterMetaType<Enums::Popup>("Popup");
    qRegisterMetaType<Enums::PopupAction>("PopupAction");
    qRegisterMetaType<Enums::ConnectionState>("ConnectionState");
    qRegisterMetaType<Enums::SupplyChainAction>("SupplyChainAction");
    qRegisterMetaType<Enums::ActionProgress>("ActionProgress");
    qRegisterMetaType<Enums::PackageType>("PackageType");

    engine.rootContext()->setContextProperty(QStringLiteral("Utility"), Utility::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("DataGlobals"), DataGlobals::instance());

    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:///common/Helpers.qml"), "com.gaiachain.helpers", 1, 0, "Helpers");

    m_pageManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
    m_userManager.setupQmlContext(engine);
    m_sessionManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}

