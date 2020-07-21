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

    auto request = QSharedPointer<AuthRequest>::create();
    request->setQuiet(true);
    if (request->isTokenRequired()) {
        request->setToken(token());
    }

    sendRequest(request, errorHandler, replyHandler, true);
}

void RestSessionManager::login(const QString &login, const QString &password)
{
    emit loginAttempt(login, password);

    const auto errorHandler = [this, login](const QString &,
                                            const QNetworkReply::NetworkError &code) {
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

bool RestSessionManager::hasPostRequests() const
{
    return m_client.hasPostRequests();
}

void RestSessionManager::sendRequest(
    const QSharedPointer<BaseRequest> &request,
    const std::function<void (const QString &, const QNetworkReply::NetworkError &)> &errorHandler,
    const std::function<void (const QJsonDocument &)> &replyHandler,
    const bool force, const bool updateProcessing)
{
    if (!enabled() && !force) {
        return;
    }

    connect(request.data(), &BaseRequest::replyError,
            this, [errorHandler](const QString &msgs, const int errorCode) { errorHandler(msgs, static_cast<QNetworkReply::NetworkError>(errorCode)); });
    connect(request.data(), &BaseRequest::requestFinished, this, replyHandler);

    sendRequest(request, updateProcessing);
}

void RestSessionManager::sendRequest(const QSharedPointer<BaseRequest> &request, const bool updateProcessing)
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

    if (updateProcessing) {
        processStarted();
    }
    updateConnectionStateBeforeRequest();

    m_client.send(request);
}
