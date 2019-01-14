#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../common/tags.h"
#include "../common/location.h"
#include "../helpers/utility.h"

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

    m_userData.insert(Tags::company, obj.value(Tags::companyName).toString());
    const QJsonArray locationArray = obj.value(Tags::location).toArray();
    Location location;
    location.lat = locationArray.at(0).toDouble();
    location.lon = locationArray.at(1).toDouble();
    m_userData.insert(Tags::location, QVariant::fromValue(location));
    const QString &role = obj.value(Tags::role).toString();
    const Enums::UserType userType = Utility::instance()->userTypeFromString(role);
    m_userData.insert(Tags::place, static_cast<int>(userType));

    emit tokenChanged(obj.value(Tags::token).toString());

    setUserType(userType);
}

QVariantMap UserManager::userData() const
{
    return m_userData;
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
