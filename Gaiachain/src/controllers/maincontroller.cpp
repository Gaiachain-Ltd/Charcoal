#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

#include <QZXing>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QtAndroid>
#endif

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
#ifdef Q_OS_ANDROID
    // check for permissions before opening scanner page to load camera faster
    // TO_DO user it only after login
    const QString cameraPermission = QStringLiteral("android.permission.CAMERA");
    if (QtAndroid::checkPermission(cameraPermission) == QtAndroid::PermissionResult::Denied) {
        auto permissionCallback = [](const QtAndroid::PermissionResultMap &) {};
        QtAndroid::requestPermissions(QStringList() << cameraPermission, permissionCallback);
    }
#endif
    m_sessionManager.setOverlayManager(&m_overlayManager);
    connect(&m_sessionManager, &SessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData, Qt::DirectConnection);
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::CommodityType>("CommodityType");
    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::PageSections>("PageSections");
    qRegisterMetaType<Enums::UserType>("UserType");
    qRegisterMetaType<Enums::PlaceType>("PlaceType");
    qRegisterMetaType<Enums::Popup>("Popup");
    qRegisterMetaType<Enums::PopupAction>("PopupAction");

    engine.rootContext()->setContextProperty(QStringLiteral("utility"), Utility::instance());

    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");

    m_pageManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
    m_userManager.setupQmlContext(engine);
    m_overlayManager.setupQmlContext(engine);
    m_sessionManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}
