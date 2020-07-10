#pragma once

#include "controllers/abstractmanager.h"
#include "common/enums.h"

#include <QSharedPointer>
#include <QNetworkReply>
#include <QObject>
#include <QTimer>

class RestSessionManager;
class UserManager;

class NotificationManager : public AbstractManager
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setSessionManager(const QSharedPointer<RestSessionManager> &manager);
    void setUserManager(const QSharedPointer<UserManager> &manager);

public slots:
    void checkNotifications();
    void stepComplete(const Enums::SupplyChainAction step,
                      const QString &pid) const;

protected slots:
    void webErrorHandler(const QString &errorString,
                         const QNetworkReply::NetworkError code) const;
    void webReplyHandler(const QJsonDocument &reply) const;

signals:
    void notify(const Enums::Page page,
                const QString &header,
                const QString &text,
                const QString &redirectText) const;

private:
    QSharedPointer<RestSessionManager> m_sessionManager;
    QSharedPointer<UserManager> m_userManager;

    QTimer m_notificationTimer;
};
