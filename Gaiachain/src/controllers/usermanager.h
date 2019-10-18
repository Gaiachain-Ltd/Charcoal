#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include <QVariantMap>

#include "../common/enums.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(QString login READ getLogin NOTIFY loginChanged)
    Q_PROPERTY(Enums::UserType userType READ getUserType NOTIFY userTypeChanged)
    Q_PROPERTY(QVariantMap userData READ userData CONSTANT)
    Q_PROPERTY(bool loggedIn READ isLoggedIn NOTIFY loggedInChanged)
public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void skipLogin();
    Q_INVOKABLE void logOut();

    bool isLoggedIn() const;

    QString getLogin() const;
    Enums::UserType getUserType() const;

    QVariantMap userData() const;

public slots:
    void parseLoginData(const QJsonDocument &doc);

signals:
    void loginChanged(const QString &getLogin) const;
    void userTypeChanged(Enums::UserType userType) const;
    void loggedInChanged(bool isLoggedIn) const;
    void tokenChanged(const QString &token) const;

private:
    Enums::UserType m_userType = Enums::UserType::Annonymous;
    QVariantMap m_userData;

    void setUserType(const Enums::UserType userType);
};

#endif // USERMANAGER_H
