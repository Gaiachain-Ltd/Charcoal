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

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}

void UserManager::logOut()
{
    if (isLoggedIn()) {
        setLogin({});
        updateUserData({});

        emit tokenChanged({});
        emit loggedInChanged(false);
    }
}

bool UserManager::offlineAvailable(const QString &login) const
{
    return m_offlineHandler.hasUser(login);
}

bool UserManager::offlineLogin(const QString &login, const QString &password)
{
    if (m_offlineHandler.canLogin(login, password)) {
        setLogin(login);
        updateUserData(m_offlineHandler.getUser(login));
        setOfflineMode(true);
        emit loggedInChanged(true);
        return true;
    }
    return false;
}

bool UserManager::isLoggedIn() const
{
    return !m_userData.isAnonymous();
}

QString UserManager::getLogin() const
{
    return m_login;
}

UserData UserManager::getUserData() const
{
    return m_userData;
}

bool UserManager::isOfflineMode() const
{
    return m_offlineMode;
}

void UserManager::handleLoginAttempt(const QString &login, const QString &password)
{
    m_offlineHandler.putTemporaryPassword(login, password);
}

void UserManager::readLoginData(const QString &login, const QJsonObject &userDataObj)
{
    setOfflineMode(false);
    m_offlineHandler.acknowledgePassword(login);

    setLogin(login);

    auto userData = UserData{};
    userData.email = userDataObj.value(Tags::login).toString();
    userData.cooperativeId = userDataObj.value(Tags::cooperativeId).toString();
    userData.cooperativeName = userDataObj.value(Tags::cooperativeName).toString();

    const auto role = userDataObj.value(Tags::role).toString();
    userData.type = RequestsHelper::userTypeFromString(role);

    updateUserData(userData);
    m_offlineHandler.putUser(m_login, userData);

    emit tokenChanged(userDataObj.value(Tags::token).toString());
    emit loggedInChanged(true);
}

void UserManager::setOfflineMode(bool offline)
{
    if (m_offlineMode != offline) {
        m_offlineMode = offline;
        emit offlineModeChanged(offline);
    }
}

void UserManager::setLogin(const QString &login)
{
    if (m_login != login) {
        m_login = login;
        emit loginChanged(login);
    }
}

void UserManager::updateUserData(const UserData &userData)
{
    m_userData = userData;
    emit userDataChanged(userData);
}
