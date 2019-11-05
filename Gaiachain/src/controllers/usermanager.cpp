#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../common/tags.h"
#include "../common/logs.h"
#include "../helpers/requestshelper.h"

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{
    connect(this, &UserManager::userDataChanged,
            this, [this]() {
        emit loginChanged(getLogin());
        emit cooperativeIdChanged(getCooperativeId());
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

QString UserManager::getCooperativeId() const
{
    return m_userData[Tags::cooperativeId].toString();
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
    m_userData.insert(Tags::cooperativeId, obj.value(Tags::cooperativeId).toString());
    m_userData.insert(Tags::cooperativeName, obj.value(Tags::cooperativeName).toString());

    const auto role = obj.value(Tags::role).toString();
    const auto userType = RequestsHelper::userTypeFromString(role);

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
