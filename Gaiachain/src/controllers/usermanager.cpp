#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../common/tags.h"
#include "../common/location.h"
#include "../common/dataglobals.h"
#include "../common/logs.h"

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{
    connect(this, &UserManager::userDataChanged,
            this, [this]() {
        emit loginChanged(getLogin());
        emit companyIdChanged(getCompanyId());
    });
    connect(this, &UserManager::loggedInChanged,
            this, [this]() { emit loginChanged(getLogin()); });
}

bool UserManager::isLoggedIn() const
{
    return m_userType != Enums::UserType::Annonymous;
}

QString UserManager::getLogin() const
{
    return isLoggedIn() ? m_userData[Tags::email].toString() : QString{};
}

QString UserManager::getCompanyId() const
{
    return m_userData[Tags::companyId].toString();
}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::logOut()
{
    m_userData.clear();

    setUserType(Enums::UserType::Annonymous);
    emit tokenChanged({});
    emit userDataChanged(m_userData);
}

Enums::UserType UserManager::getUserType() const
{
    return m_userType;
}

void UserManager::parseLoginData(const QJsonDocument &doc)
{
    const QJsonObject obj = doc.object();

    m_userData.insert(Tags::email, obj.value(Tags::email).toString());
    m_userData.insert(Tags::companyId, obj.value(Tags::companyId).toString());
    m_userData.insert(Tags::companyName, obj.value(Tags::companyName).toString());

    const auto locationArray = obj.value(Tags::companyLocation).toArray();
    m_userData.insert(Tags::companyLocation, QVariant::fromValue(Location{ locationArray.at(0).toDouble(),
                                                                    locationArray.at(1).toDouble() }));

    const auto role = obj.value(Tags::role).toString();
    const auto userType = DataGlobals::userTypeFromString(role);

    setUserType(userType);
    emit tokenChanged(obj.value(Tags::token).toString());
    emit userDataChanged(m_userData);
}

QVariantMap UserManager::userData() const
{
    return m_userData;
}

void UserManager::setUserType(const Enums::UserType userType)
{
    if (m_userType == userType)
        return;

    auto prevUserType = m_userType;
    m_userType = userType;
    emit userTypeChanged(userType);

    if (prevUserType == Enums::UserType::Annonymous ||
            userType == Enums::UserType::Annonymous) {
        emit loggedInChanged(isLoggedIn());
    }
}
