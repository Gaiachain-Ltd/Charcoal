#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include <QVariantMap>

#include "../common/enums.h"
#include "../common/userdata.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(UserData userData READ getUserData NOTIFY userDataChanged)
    Q_PROPERTY(QString login READ getLogin NOTIFY userDataChanged)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)
public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void logOut();
    bool isLoggedIn() const;

    QString getLogin() const;
    UserData getUserData() const;

public slots:
    void readLoginData(const QJsonObject &userDataObj);

signals:
    void tokenChanged(const QString &token) const;
    void loggedInChanged(bool isLoggedIn) const;
    void loggedIn() const;

    void userDataChanged(const UserData &userData) const;

private:
    UserData m_userData;
};

#endif // USERMANAGER_H
