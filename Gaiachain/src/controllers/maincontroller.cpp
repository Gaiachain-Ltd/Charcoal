#include "maincontroller.h"

#include <QQmlApplicationEngine>

#include "../common/enums.h"

MainController::MainController(QObject *parent)
    : QObject(parent)
{
}

void MainController::setupQMLContext(QQmlApplicationEngine &engine)
{
    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.models", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::CommodityType>("CommodityType");

    m_dataManager.setupQmlContext(engine);
}

