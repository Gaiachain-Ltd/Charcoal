#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

#include <QZXing>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
    m_sessionManager.setOverlayManager(&m_overlayManager);
    auto enterPopup = [&](const QString &text, const QString &button1, const QString &button2) {
        m_pageManager.enterPopup(text, button1, button2);
    };
    connect(&m_sessionManager, &SessionManager::displayError, enterPopup);
    connect(&m_sessionManager, &SessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData, Qt::DirectConnection);
}

void MainController::setupQMLContext(QQmlApplicationEngine &engine)
{
    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::CommodityType>("CommodityType");
    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::PageSections>("PageSections");

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
