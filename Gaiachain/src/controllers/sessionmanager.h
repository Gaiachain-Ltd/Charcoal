#ifndef SESSION_H
#define SESSION_H

#include "abstractmanager.h"
#include "../rest/restapiclient.h"
#include "../rest/baserequest.h"

class OverlayManager;
class UserManager;

class SessionManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    void setOverlayManager(OverlayManager *manager);
    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void login(const QString &email, const QString &password);
    Q_INVOKABLE void getEntity();
    Q_INVOKABLE void getEntity(const QString &id);
    Q_INVOKABLE void getEntities(const QJsonArray &ids);
    Q_INVOKABLE void putEntity(const QString &id, const int action);
    Q_INVOKABLE void getEntityAction(const QString &id, const int role);

public slots:
    void onTokenChanged(const QString &token);

private:
    OverlayManager *m_overlayManager;
    RestAPIClient m_client;
    QString m_token;

signals:
    void displayLoginError(const int code) const;
    void loginFinished(const QJsonDocument &doc) const;
    void entityLoadError(const int code) const;
    void entityLoaded(const QJsonObject &entity) const;
    void entitiesLoaded(const QJsonArray &entities) const;
    void entityActionDownloaded(const QString &id, const int action) const;
    void entityActionDownloadedError(const QString &id, const bool exists) const;

    void beforeGetEntity() const;

    void entitySaveResult(const QString &id, const bool result) const;
};

#endif // SESSION_H
