#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "../common/tags.h"

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
    const QJsonObject obj = doc.object();
    emit tokenChanged(obj.value(Tags::token).toString());
    const QString &role = obj.value(Tags::role).toString();

    setUserType(Enums::UserTypeStruct::userTypeFromString(role));
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
