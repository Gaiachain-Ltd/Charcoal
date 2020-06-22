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

    template <typename Object, typename PointerToErrorHandler, typename PointerToReplyHandler>
    void sendRequest(const QSharedPointer<BaseRequest> &request,
                     const Object *target,
                     PointerToErrorHandler errorHandler,
                     PointerToReplyHandler replyHandler,
                     bool force = false)
    {
        if (!enabled() && !force) {
            return;
        }

        connect(request.data(), &BaseRequest::replyError,
                target, errorHandler);
        connect(request.data(), &BaseRequest::requestFinished,
                target, replyHandler);

        sendRequest(request);
    }

    void sendRequest(const QSharedPointer<BaseRequest> &request,
                     const std::function<void(const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
                     const std::function<void(const QJsonDocument &)> &replyHandler,
                     bool force = false);
    void sendRequest(const QSharedPointer<BaseRequest> &request);

protected:
    RestAPIClient m_client;
};

#endif // SESSIONMANAGER_H
