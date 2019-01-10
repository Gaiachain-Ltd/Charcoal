#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include "../common/enums.h"

class UserManager : public AbstractManager
{
    Q_OBJECT

public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) Q_DECL_OVERRIDE;

    Q_PROPERTY(Enums::UserType userType READ getUserType WRITE setUserType NOTIFY userTypeChanged)
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)

    Q_INVOKABLE void logOut();

    bool loggedIn() const;

    Enums::UserType getUserType() const;
    void setUserType(const Enums::UserType userType);

public slots:
    void parseLoginData(const QJsonDocument &doc);

signals:
    void userTypeChanged(Enums::UserType userType) const;
    void loggedInChanged(bool loggedIn) const;

private:
    Enums::UserType m_userType = Enums::UserType::NotLoggedUser;
    bool m_loggedIn = false;

    void setLoggedIn();
};

#endif // USERMANAGER_H
