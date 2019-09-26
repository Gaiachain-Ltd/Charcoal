#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

#include <QZXing.h>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
#endif

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
    QLocale::setDefault(QLocale("en_UK"));

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
    connect(&m_userManager, &UserManager::tokenChanged, &m_sessionManager, &SessionManager::onTokenChanged);
    connect(&m_sessionManager, &SessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData, Qt::DirectConnection);
#ifndef FAKE_DATA
    connect(&m_sessionManager, &SessionManager::entityLoaded, &m_dataManager, &DataManager::onEntityLoaded, Qt::DirectConnection);
    connect(&m_sessionManager, &SessionManager::entitiesLoaded, &m_dataManager, &DataManager::onEntitiesLoaded, Qt::DirectConnection);
    connect(&m_sessionManager, &SessionManager::beforeGetEntity, &m_dataManager, &DataManager::clearModels, Qt::DirectConnection);
#endif
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("isDesktop",
#ifdef DESKTOP_TESTS
                                             true);
#else
                                             false);
#endif

    engine.rootContext()->setContextProperty("fakeData",
#ifdef FAKE_DATA
                                             true);
#else
                                             false);
#endif

    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::UserType>("UserType");
    qRegisterMetaType<Enums::PlaceType>("PlaceType");
    qRegisterMetaType<Enums::Popup>("Popup");
    qRegisterMetaType<Enums::PopupAction>("PopupAction");
    qRegisterMetaType<Enums::ConnectionState>("ConnectionState");
    qRegisterMetaType<Enums::SupplyChainAction>("SupplyChainAction");
    qRegisterMetaType<Enums::PackageType>("PackageType");

    engine.rootContext()->setContextProperty(QStringLiteral("utility"), Utility::instance());

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

