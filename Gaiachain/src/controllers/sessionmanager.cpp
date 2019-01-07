#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(session, "session")

#include "overlaymanager.h"
#include "../rest/loginrequest.h"

SessionManager::SessionManager(QObject *parent)
    : AbstractManager(parent)
{

}

void SessionManager::setOverlayManager(OverlayManager *manager)
{
    m_overlayManager = manager;
}

void SessionManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("sessionManager"), this);
}

void SessionManager::login(const QString &email, const QString &password)
{
    auto request = QSharedPointer<LoginRequest>::create(email, password);

    m_overlayManager->setLoginRequest(true);

    auto finishLambda = [=]() {
        // do something with data
        this->m_overlayManager->setLoginRequest(false);
    };

    connect(request.data(), &LoginRequest::finished, finishLambda);
    connect(request.data(), &LoginRequest::replyError, finishLambda);

    m_client.send(request);
}

void SessionManager::connectFinishingLambda(QSharedPointer<BaseRequest> &sharedPointer, std::function<void()> &lambda)
{
    connect(sharedPointer.data(), &BaseRequest::finished, lambda);
    connect(sharedPointer.data(), &BaseRequest::replyError, lambda);
}
