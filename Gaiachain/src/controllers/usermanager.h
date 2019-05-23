#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

#include <QVariantMap>

#include "../common/enums.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(Enums::UserType userType READ getUserType WRITE setUserType NOTIFY userTypeChanged)
    Q_PROPERTY(QVariantMap userData READ userData CONSTANT)
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(int commodityType READ commodityType WRITE setCommodityType NOTIFY commodityTypeChanged)
public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void logOut();

    bool loggedIn() const;

    Enums::UserType getUserType() const;
    void setUserType(const Enums::UserType userType);

    int commodityType() const;
    void setCommodityType(const int commodityType);

    QVariantMap userData() const;

public slots:
    void parseLoginData(const QJsonDocument &doc);

signals:
    void userTypeChanged(Enums::UserType userType) const;
    void commodityTypeChanged(int commodityType) const;
    void loggedInChanged(bool loggedIn) const;
    void tokenChanged(const QString &token) const;

private:
    int m_commodityType = static_cast<int>(Enums::CommodityType::InvalidCommodity);
    Enums::UserType m_userType = Enums::UserType::NotLoggedUser;
    bool m_loggedIn = false;
    QVariantMap m_userData;

    void setLoggedIn();
};

#endif // USERMANAGER_H
