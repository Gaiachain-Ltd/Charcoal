#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "abstractsessionmanager.h"
#include "../../rest/restapiclient.h"
#include "../../rest/baserequest.h"

class SessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit SessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void ping() override;
    Q_INVOKABLE void login(const QString &login, const QString &password) override;

    Q_INVOKABLE void getAdditionalData() override;
    Q_INVOKABLE void getProducers() override;
    Q_INVOKABLE void getCompanies() override;
    Q_INVOKABLE void getDestinations() override;

    Q_INVOKABLE void getEntitiesInfo(int limit, const QDateTime &from, const QString &keyword) override;
    Q_INVOKABLE void getEntitiesInfo(const QDateTime &from, const QDateTime &to, const QString &keyword) override;
    Q_INVOKABLE void getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction) override;

    Q_INVOKABLE void getEntities(const QStringList &packageIds) override;

    Q_INVOKABLE void postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties) override;
    Q_INVOKABLE void postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties) override;
    Q_INVOKABLE void postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                   const QDateTime &timestamp, const QVariantMap &properties) override;

    Q_INVOKABLE void getUnusedLotIds() override;
    Q_INVOKABLE void postUnusedLotId() override;

private:
    RestAPIClient m_client;

    void sendRequest(const QSharedPointer<BaseRequest> &request,
                     const std::function<void(const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
                     const std::function<void(const QJsonDocument &)> &replyHandler);
    void sendRequest(const QSharedPointer<BaseRequest> &request);
};

#endif // SESSIONMANAGER_H
