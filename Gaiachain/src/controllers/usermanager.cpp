#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QJsonDocument>

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{

}

bool UserManager::loggedIn() const
{
    return m_userType != Enums::UserType::NotLoggedUser;
}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::logOut()
{
    setUserType(Enums::UserType::NotLoggedUser);
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

void UserManager::setLoggedIn()
{
    bool loggedIn = m_userType != Enums::UserType::NotLoggedUser;
    if (loggedIn == m_loggedIn)
        return;

    m_loggedIn = loggedIn;
    emit loggedInChanged(m_loggedIn);
}

void UserManager::setUserType(const Enums::UserType userType)
{
    if (m_userType == userType)
        return;

    m_userType = userType;
    setLoggedIn();
    emit userTypeChanged(userType);
}
