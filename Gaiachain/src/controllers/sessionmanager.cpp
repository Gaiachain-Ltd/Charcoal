#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonArray>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(session, "session")

#include "overlaymanager.h"
#include "../rest/loginrequest.h"
#include "../rest/entityrequest.h"

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

void SessionManager::onTokenChanged(const QString &token)
{
    m_token = token;
}

void SessionManager::login(const QString &email, const QString &password)
{
    auto request = QSharedPointer<LoginRequest>::create(email, password);

    m_overlayManager->setLoginRequest(true);

    auto finishLambda = [&](const QJsonDocument &reply) {
        emit loginFinished(reply);
        m_overlayManager->setLoginRequest(false);
        this->getEntity();
    };

    auto errorLambda = [&]() {
        m_overlayManager->setLoginRequest(false);
        displayLoginError(tr("Login failed. Try again."), QString() , tr("Close"));
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntity()
{
    auto request = QSharedPointer<EntityRequest>::create(m_token);

    auto errorLambda = [&](const QString &msgs, const int errorCode) {
        qDebug() << "--------- ENTITY_ERROR" << errorCode << msgs;
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        const QJsonArray &array = reply.array();
        QJsonArray::const_iterator it = array.constBegin();
        while(it != array.constEnd()) {
            this->getEntity((*it).toString());
            ++it;
        }
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntity(const QString &id)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id);

    auto errorLambda = [&](const QString &msgs, const int errorCode) {
        qDebug() << "--------- ENTITY_ERROR" << errorCode << msgs;
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        emit entityLoaded(reply);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::putEntity(const QString &id)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id, Enums::PlaceAction::Departed);

    auto errorLambda = [&](const QString &msgs, const int errorCode) {
        qDebug() << "--------- ENTITY_ERROR" << errorCode << msgs;
    };

    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}
