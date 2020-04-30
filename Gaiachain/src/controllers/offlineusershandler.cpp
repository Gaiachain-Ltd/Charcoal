#include "offlineusershandler.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>

#include "../helpers/cryptohelper.h"

namespace {
const QLatin1String UsersFileName =
#ifdef COCOA
    QLatin1String("users");
#elif CHARCOAL
    QLatin1String("users-charcoal");
#else
    #error(Cannot determine users file name!)
#endif
}

namespace SettingKey {
const QLatin1String Logins = QLatin1String("logins");
const QLatin1String TmpSalt = QLatin1String("tmpSalt");
const QLatin1String TmpPassword = QLatin1String("tmpPassword");
const QLatin1String Salt = QLatin1String("salt");
const QLatin1String Password = QLatin1String("password");
const QLatin1String Token = QLatin1String("token");
const QLatin1String Email = QLatin1String("email");
const QLatin1String Type = QLatin1String("type");
const QLatin1String CooperativeId = QLatin1String("cooperativeId");
const QLatin1String CooperativeCode = QLatin1String("cooperativeCode");
const QLatin1String CooperativeName = QLatin1String("cooperativeName");
}

OfflineUsersHandler::OfflineUsersHandler()
    : c_dataFilePath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + UsersFileName + "." + CryptoHelper::EncryptedSettingsExt),
      m_usersData(c_dataFilePath, CryptoHelper::encryptedSettingsFormat())
{
}

bool OfflineUsersHandler::hasUser(const QString &login) const
{
    return m_usersData.value(SettingKey::Logins).toStringList().contains(login);
}

bool OfflineUsersHandler::canLogin(const QString &login, const QString &password) const
{
    if (!hasUser(login)) {
        return false;
    }

    const_cast<QSettings &>(m_usersData).beginGroup(login); // beginGroup is not a const function :(

    auto salt = m_usersData.value(SettingKey::Salt).toByteArray();
    auto storedHash = m_usersData.value(SettingKey::Password).toByteArray();

    const_cast<QSettings &>(m_usersData).endGroup();        // endGroup is not a const function :(

    return (storedHash == CryptoHelper::passwordHash(password.toLatin1(), salt));
}

void OfflineUsersHandler::putTemporaryPassword(const QString &login, const QString &password)
{
    m_usersData.beginGroup(login);

    auto salt = CryptoHelper::randomSalt();
    m_usersData.setValue(SettingKey::TmpSalt, salt);
    m_usersData.setValue(SettingKey::TmpPassword, CryptoHelper::passwordHash(password.toLatin1(), salt));

    m_usersData.endGroup();
}

void OfflineUsersHandler::acknowledgePassword(const QString &login)
{
    m_usersData.beginGroup(login);

    m_usersData.setValue(SettingKey::Salt, m_usersData.value(SettingKey::TmpSalt));
    m_usersData.setValue(SettingKey::Password, m_usersData.value(SettingKey::TmpPassword));

    m_usersData.endGroup();
}

void OfflineUsersHandler::putUser(const QString &login, const UserData &userData, const QString &token)
{
    if (userData.isAnonymous()) {
        return;
    }

    if (!hasUser(login)) {
        addUser(login);
    }

    m_usersData.beginGroup(login);

    m_usersData.setValue(SettingKey::Token, token);
    m_usersData.setValue(SettingKey::Email, userData.email);
    m_usersData.setValue(SettingKey::Type, QVariant::fromValue(userData.type));
    m_usersData.setValue(SettingKey::CooperativeId, userData.cooperativeId);
    m_usersData.setValue(SettingKey::CooperativeCode, userData.cooperativeCode);
    m_usersData.setValue(SettingKey::CooperativeName, userData.cooperativeName);

    m_usersData.endGroup();
}

QPair<UserData, QString> OfflineUsersHandler::getUser(const QString &login) const
{
    auto token = QString{};
    auto userData = UserData{};
    if (hasUser(login)) {
        const_cast<QSettings &>(m_usersData).beginGroup(login); // beginGroup is not a const function :(

        token = m_usersData.value(SettingKey::Token).toString();
        userData.email = m_usersData.value(SettingKey::Email).toString();
        userData.type = m_usersData.value(SettingKey::Type).value<Enums::UserType>();
        userData.cooperativeId = m_usersData.value(SettingKey::CooperativeId).toUInt();
        userData.cooperativeCode = m_usersData.value(SettingKey::CooperativeCode).toString();
        userData.cooperativeName = m_usersData.value(SettingKey::CooperativeName).toString();

        const_cast<QSettings &>(m_usersData).endGroup();        // endGroup is not a const function :(
    }
    return { userData, token };
}

void OfflineUsersHandler::addUser(const QString &login)
{
    auto logins = m_usersData.value(SettingKey::Logins).toStringList();
    logins.append(login);
    logins.removeDuplicates();
    m_usersData.setValue(SettingKey::Logins, logins);
}
