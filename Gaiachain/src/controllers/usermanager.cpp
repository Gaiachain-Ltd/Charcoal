#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QJsonDocument>

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{

}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::parseLoginData(const QJsonDocument &doc)
{
    qDebug() << "------ RECEIVED LOGIN DATA" << doc;
}
