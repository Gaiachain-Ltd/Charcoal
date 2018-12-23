#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

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

    m_pageManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
}

