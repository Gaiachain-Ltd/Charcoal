#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include "offlineusershandler.h"
#include "common/enums.h"

#ifdef COCOA
#include "cocoa/cocoauserdata.h"
#elif CHARCOAL
#include "charcoal/charcoaluserdata.h"
#endif

class UserManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(UserData userData READ getUserData NOTIFY userDataChanged)
    Q_PROPERTY(QString login READ getLogin NOTIFY loginChanged)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(bool offlineMode READ isOfflineMode NOTIFY offlineModeChanged)

public:
    explicit UserManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void logOut();
    Q_INVOKABLE bool offlineAvailable(const QString &login) const;
    Q_INVOKABLE bool offlineLogin(const QString &login, const QString &password);

    bool isLoggedIn() const;

    QString getLogin() const;
    UserData getUserData() const;

    bool isOfflineMode() const;

public slots:
    void handleLoginAttempt(const QString &login, const QString &password);
    void readLoginData(const QString &login, const QJsonObject &userDataObj);

signals:
    void tokenChanged(const QString &token) const;
    void loggedInChanged(bool isLoggedIn) const;
    void loggedIn() const;

    void loginChanged(const QString &login) const;
    void userDataChanged(const UserData &userData) const;

    void offlineModeChanged(bool offlineMode);

private:
    OfflineUsersHandler m_offlineHandler;
    bool m_offlineMode = false;

    QString m_login;
    UserData m_userData;

    void setOfflineMode(bool offline);
    void setLogin(const QString &login);
    void updateUserData(const UserData &userData);
};

#endif // USERMANAGER_H
