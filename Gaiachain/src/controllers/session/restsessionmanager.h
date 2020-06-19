#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "abstractsessionmanager.h"
#include "rest/restapiclient.h"
#include "rest/baserequest.h"

class QGeoCoordinate;

class RestSessionManager : public AbstractSessionManager
{
    Q_OBJECT

public:
    explicit RestSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void ping() override;
    Q_INVOKABLE void login(const QString &login, const QString &password) override;

protected:
    RestAPIClient m_client;

    void sendRequest(const QSharedPointer<BaseRequest> &request,
                     const std::function<void(const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
                     const std::function<void(const QJsonDocument &)> &replyHandler,
                     bool force = false);
    void sendRequest(const QSharedPointer<BaseRequest> &request);
};

#endif // SESSIONMANAGER_H
