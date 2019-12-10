#ifndef OFFLINEUSERSHANDLER_H
#define OFFLINEUSERSHANDLER_H

#include <QSettings>

#include "../common/userdata.h"

class OfflineUsersHandler final
{
public:
    OfflineUsersHandler();

    bool hasUser(const QString &login) const;
    bool canLogin(const QString &login, const QString &password) const;

    void putTemporaryPassword(const QString &login, const QString &password);
    void acknowledgePassword(const QString &login);

    void putUser(const QString &login, const UserData &userData, const QString &token);
    QPair<UserData, QString> getUser(const QString &login) const;

private:
    const QString c_dataFilePath;

    QSettings m_usersData;

    void addUser(const QString &login);
};

#endif // OFFLINEUSERSHANDLER_H
