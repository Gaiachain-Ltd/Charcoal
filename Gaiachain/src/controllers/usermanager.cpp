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
        setOfflineMode(false);
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
        setOfflineMode(true);
        setLogin(login);

        auto [ userData, token ] = m_offlineHandler.getUser(login);
        updateUserData(userData);

        emit tokenChanged(token);
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
    //qDebug().noquote() << "Reading login data" << login << QJsonDocument(userDataObj).toJson();

    setOfflineMode(false);
    m_offlineHandler.acknowledgePassword(login);

    setLogin(login);

    auto userData = UserData{};
    userData.email = RequestsHelper::checkAndValue(userDataObj, Tags::login).toString();

#ifdef COCOA
    auto cooperativeObj = RequestsHelper::checkAndValue(userDataObj, Tags::company).toObject();
    userData.cooperativeId = static_cast<quint32>(
        RequestsHelper::checkAndValue(cooperativeObj, Tags::id).toInt());
    userData.cooperativeCode = RequestsHelper::checkAndValue(
                                   cooperativeObj, Tags::pid).toString();
    userData.cooperativeName = RequestsHelper::checkAndValue(
                                   cooperativeObj, Tags::name).toString();
    const auto roleObj = RequestsHelper::checkAndValue(userDataObj, Tags::role).toObject();
    const auto role = RequestsHelper::checkAndValue(roleObj, Tags::name).toString();
    userData.type = RequestsHelper::userTypeFromString(role);
#elif CHARCOAL
    const auto roleObj = RequestsHelper::checkAndValue(userDataObj, Tags::role).toObject();
    const auto role = RequestsHelper::checkAndValue(roleObj, Tags::name).toString();
    if (role == "SUPER_USER") {
        userData.type = Enums::UserType::SuperUser;
    }

    userData.code = RequestsHelper::checkAndValue(userDataObj, Tags::code).toString();
    userData.contact = RequestsHelper::checkAndValue(userDataObj, Tags::contact).toString();
    userData.job = RequestsHelper::checkAndValue(userDataObj, Tags::function).toString();
    userData.name = RequestsHelper::checkAndValue(userDataObj, Tags::fullName).toString();
#endif


    const auto token = RequestsHelper::checkAndValue(userDataObj, Tags::accessToken).toString();

    updateUserData(userData);
    m_offlineHandler.putUser(login, userData, token);

    emit tokenChanged(token);
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
