#include "maincontroller.h"

#include <QQmlApplicationEngine>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
}

void MainController::setupQMLContext(QQmlApplicationEngine &engine)
{
    m_dataManager.setupQmlContext(engine);
}

