#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

#include <QZXing>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
}

void MainController::setupQMLContext(QQmlApplicationEngine &engine)
{
    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::CommodityType>("CommodityType");
    qRegisterMetaType<Enums::Page>("Page");

    engine.rootContext()->setContextProperty(QStringLiteral("utility"), Utility::instance());
    engine.rootContext()->setContextProperty(QStringLiteral("restSession"), &m_session);

    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");

    m_pageManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}
