#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include "../common/enums.h"

class UserManager : public AbstractManager
{
    Q_OBJECT

public:
    explicit UserManager(QObject *parent = nullptr);

    Q_PROPERTY(Enums::UserType userType READ getUserType WRITE setUserType NOTIFY userTypeChanged)

    virtual void setupQmlContext(QQmlApplicationEngine &engine) Q_DECL_OVERRIDE;

    Enums::UserType getUserType() const;
    void setUserType(const Enums::UserType userType);

public slots:
    void parseLoginData(const QJsonDocument &doc);

signals:
    void userTypeChanged(Enums::UserType userType) const;

private:
    Enums::UserType m_userType = Enums::UserType::NotLoggedUser;
};

#endif // USERMANAGER_H
