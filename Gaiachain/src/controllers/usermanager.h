#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include <QVariantMap>

#include "../common/enums.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(QString login READ getLogin NOTIFY loginChanged)
    Q_PROPERTY(QString cooperativeId READ getCooperativeId NOTIFY cooperativeIdChanged)
    Q_PROPERTY(Enums::UserType userType READ getUserType NOTIFY userTypeChanged)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)
public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void logOut();

    bool isLoggedIn() const;

    QString getLogin() const;
    QString getCooperativeId() const;

    Enums::UserType getUserType() const;

    QVariantMap userData() const;

public slots:
    void parseLoginData(const QJsonDocument &doc);

signals:
    void tokenChanged(const QString &token) const;

    void loginChanged(QString login);
    void cooperativeIdChanged(QString cooperativeId);
    void userDataChanged(const QVariantMap &userData) const;

    void userTypeChanged(Enums::UserType userType) const;
    void loggedInChanged(bool isLoggedIn) const;

private:
    Enums::UserType m_userType = Enums::UserType::Annonymous;
    QVariantMap m_userData;

    void setUserType(const Enums::UserType userType);
    QString m_login;
};

#endif // USERMANAGER_H
