#ifndef SESSION_H
#define SESSION_H

#include "abstractmanager.h"
#include "../rest/restapiclient.h"
#include "../rest/baserequest.h"
#include "../common/enums.h"

class UserManager;

class SessionManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(Enums::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    explicit SessionManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Enums::ConnectionState connectionState() const;

    Q_INVOKABLE void login(const QString &email, const QString &password);

    Q_INVOKABLE void getEntity();
    Q_INVOKABLE void getEntity(const QString &id);
    Q_INVOKABLE void getEntities(const QJsonArray &ids);
    Q_INVOKABLE void putEntity(const QString &id, const int action);
    Q_INVOKABLE void getEntityAction(const QString &id, const int role);

public slots:
    void onTokenChanged(const QString &token);

signals:
    void connectionStateChanged(Enums::ConnectionState connectionState);

    void loginError(const int code) const;
    void loginFinished(const QJsonDocument &doc) const;

    void entityLoadError(const int code) const;
    void entityLoaded(const QJsonObject &entity) const;
    void entitiesLoaded(const QJsonArray &entities) const;
    void entityActionDownloaded(const QString &id, const int action) const;
    void entityActionDownloadedError(const QString &id, const bool exists) const;

    void beforeGetEntity() const;

    void entitySaveResult(const QString &id, const bool result) const;

private:
    RestAPIClient m_client;
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;

    QString m_token;

    void sendRequest(const QSharedPointer<BaseRequest> &request);

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const int errorCode = QNetworkReply::NoError);
};

#endif // SESSION_H
