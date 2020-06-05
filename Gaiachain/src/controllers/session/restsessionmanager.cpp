#include "restsessionmanager.h"

#include <QJsonObject>
#include <QJsonArray>

#include "rest/authrequest.h"

RestSessionManager::RestSessionManager(QObject *parent)
    : AbstractSessionManager(parent)
{}

void RestSessionManager::ping()
{
    const auto errorHandler = [this](const QString &, const QNetworkReply::NetworkError &code) {
        emit pingError(code);
    };
    const auto replyHandler = [this](const QJsonDocument &) {
        emit pingSuccess();
    };
    sendRequest(QSharedPointer<AuthRequest>::create(), errorHandler, replyHandler, true);
}

void RestSessionManager::login(const QString &login, const QString &password)
{
    emit loginAttempt(login, password);

    const auto errorHandler = [this, login](const QString &, const QNetworkReply::NetworkError &code) {
        emit loginError(login, code);
    };
    const auto replyHandler = [this, login](const QJsonDocument &reply) {
        if (reply.isObject()) {
            emit loginFinished(login, reply.object());
        } else {
            emit loginError(login, QNetworkReply::NetworkError::UnknownContentError);
        }
    };
    sendRequest(QSharedPointer<AuthRequest>::create(login, password), errorHandler, replyHandler);
}

void RestSessionManager::sendRequest(
    const QSharedPointer<BaseRequest> &request,
    const std::function<void (const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
    const std::function<void (const QJsonDocument &)> &replyHandler,
    bool force)
{
    if (!enabled() && !force) {
        return;
    }

    connect(request.data(), &BaseRequest::replyError,
            this, [errorHandler](const QString &msgs, const int errorCode) { errorHandler(msgs, static_cast<QNetworkReply::NetworkError>(errorCode)); });
    connect(request.data(), &BaseRequest::requestFinished, this, replyHandler);

    sendRequest(request);
}

void RestSessionManager::sendRequest(const QSharedPointer<BaseRequest> &request)
{
    connect(request.data(), &BaseRequest::requestFinished,
            this, [this](const QJsonDocument &) {
        updateConnectionStateAfterRequest();
        processFinished();
    });
    connect(request.data(), &BaseRequest::replyError,
            this, [this](const QString &, const int errorCode) {
        updateConnectionStateAfterRequest(QNetworkReply::NetworkError(errorCode));
        processFinished();
    });

    processStarted();
    updateConnectionStateBeforeRequest();

    m_client.send(request);
}
