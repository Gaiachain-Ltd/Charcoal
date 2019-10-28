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

    Q_INVOKABLE virtual void getFullData();

    Q_INVOKABLE virtual void getAdditionalData() = 0;

    Q_INVOKABLE virtual void getRelations(const QString &id) = 0;
    Q_INVOKABLE virtual void addRelation(const QString &id, const QStringList &ids) = 0;

    Q_INVOKABLE virtual void getEntitiesInfo(int count, const QDateTime &from = {}) = 0;
    Q_INVOKABLE virtual void getEntitiesInfo(const QDateTime &to, const QDateTime &from = {}) = 0;
    Q_INVOKABLE virtual void getEntities(const QStringList &ids) = 0;
    Q_INVOKABLE virtual void getEntity(const QString &id) = 0;
    Q_INVOKABLE virtual void getEntitId(const QByteArray &codeData) = 0;
    Q_INVOKABLE virtual void putEntityAction(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                             const QVariantMap &properties, const QByteArray &codeData = {}) = 0;
    Q_INVOKABLE virtual void putEntity(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                       const QVariantMap &properties, const QByteArray &codeData) = 0;

    Q_INVOKABLE virtual void getUnusedLotIds() = 0;
    Q_INVOKABLE virtual void createUnusedLotId() = 0;

    void updateToken(const QString &token);

signals:
    void connectionStateChanged(Enums::ConnectionState connectionState);

    void beforeGetFullData() const;

    void loginError(const int code) const;
    void loginFinished(const QJsonDocument &doc) const;

    void additionalDataLoadError(const int code) const;
    void additionalDataLoaded(const QJsonObject &fixesValues) const;

    void relationsLoadError(const int code) const;
    void packageRelationsLoaded(const QJsonArray &packages) const;
    void packagesRelationsLoaded(const QJsonObject &relations) const;
    void packageRelationsSaveResult(const QString &id, const bool &result) const;

    void entityLoadError(const int code) const;
    void entitiesLoaded(const QJsonArray &entities) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entityIdLoaded(const QString &id) const;
    void entitySaveResult(const QString &id, const bool &result) const;

    void unusedLotIdsLoadError(const int code) const;
    void unusedLotIdsLoaded(const QJsonArray &ids) const;
    void createUnusedLotIdResult(const QString &id, const bool &result) const;

protected:
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;
    QString m_token;

    virtual void getAllRelations() = 0;
    virtual void getAllEntities() = 0;

    bool checkValidToken() const;

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode = QNetworkReply::NoError);
};

#endif // ABSTRACTSESSIONMANAGER_H
