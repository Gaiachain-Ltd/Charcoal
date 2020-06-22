#ifndef ABSTRACTSESSIONMANAGER_H
#define ABSTRACTSESSIONMANAGER_H

#include "controllers/abstractmanager.h"
#include "common/enums.h"

#include <QNetworkReply>
#include <QLoggingCategory>

class QGeoCoordinate;

Q_DECLARE_LOGGING_CATEGORY(sessionManager)

class AbstractSessionManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(Enums::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    explicit AbstractSessionManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    bool enabled() const;
    Enums::ConnectionState connectionState() const;
    QString token() const;

    Q_INVOKABLE virtual void ping() = 0;

    Q_INVOKABLE virtual void login(const QString &login, const QString &password) = 0;

public slots:
    void setEnabled(bool enabled);
    void updateToken(const QString &token);

signals:
    void enabledChanged(bool enabled) const;
    void connectionStateChanged(Enums::ConnectionState connectionState) const;

    void pingError(const QNetworkReply::NetworkError &code) const;
    void pingSuccess() const;

    void loginAttempt(const QString &login, const QString &password) const;
    void loginError(const QString &login, const int &code) const;
    void loginFinished(const QString &login, const QJsonObject &userDataObj) const;

protected:
    bool m_enabled = true;
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;
    QString m_token;

    bool checkValidToken() const;

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode = QNetworkReply::NoError);
};

#endif // ABSTRACTSESSIONMANAGER_H
