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

Enums::UserType UserManager::getUserType() const
{
    return m_userType;
}

void UserManager::parseLoginData(const QJsonDocument &doc)
{
    setUserType(Enums::UserType::Producer);

    qDebug() << "------ RECEIVED LOGIN DATA" << doc;
}

void UserManager::setUserType(const Enums::UserType userType)
{
    if (m_userType == userType)
        return;

    m_userType = userType;
    emit userTypeChanged(userType);
}
