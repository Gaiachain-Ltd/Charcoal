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
    connect(this, &UserManager::loggedInChanged,
            this, [this](bool loggedId) { if (loggedId) { emit loggedIn(); } });
}

bool UserManager::isLoggedIn() const
{
    return !m_userData.isAnonymous();
}

QString UserManager::getLogin() const
{
    return m_userData.email;
}

UserData UserManager::getUserData() const
{
    return m_userData;
}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::logOut()
{
    if (isLoggedIn()) {
        m_userData = {};

        emit tokenChanged({});
        emit userDataChanged(m_userData);
        emit loggedInChanged(false);
    }
}

void UserManager::readLoginData(const QJsonObject &userDataObj)
{
    m_userData.email = userDataObj.value(Tags::email).toString();
    m_userData.cooperativeId = userDataObj.value(Tags::cooperativeId).toString();
    m_userData.cooperativeName = userDataObj.value(Tags::cooperativeName).toString();

    const auto role = userDataObj.value(Tags::role).toString();
    m_userData.type = RequestsHelper::userTypeFromString(role);

    emit loggedInChanged(true);
    emit tokenChanged(userDataObj.value(Tags::token).toString());
    emit userDataChanged(m_userData);
}
