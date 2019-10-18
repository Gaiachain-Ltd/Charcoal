#ifndef ABSTRACTSESSIONMANAGER_H
#define ABSTRACTSESSIONMANAGER_H

#include "../abstractmanager.h"
#include "../../common/enums.h"

#include <QNetworkReply>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(sessionManager)

class AbstractSessionManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(Enums::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    explicit AbstractSessionManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Enums::ConnectionState connectionState() const;

    Q_INVOKABLE virtual void login(const QString &email, const QString &password) = 0;

    Q_INVOKABLE virtual void getEntity() = 0;
    Q_INVOKABLE virtual void getEntity(const QString &id) = 0;
    Q_INVOKABLE virtual void putEntity(const QString &id, const Enums::SupplyChainAction &action, const QVariantMap &properties) = 0;

public slots:
    void onTokenChanged(const QString &token);

signals:
    void connectionStateChanged(Enums::ConnectionState connectionState);

    void loginError(const int code) const;
    void loginFinished(const QJsonDocument &doc) const;

    void entityLoadError(const int code) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void beforeGetEntity() const;
    void entitySaveResult(const QString &id, const bool result) const;

protected:
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;
    QString m_token;

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode = QNetworkReply::NoError);
};

#endif // ABSTRACTSESSIONMANAGER_H
