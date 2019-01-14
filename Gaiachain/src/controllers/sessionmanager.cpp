#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(session, "session")

#include "overlaymanager.h"
#include "../common/tags.h"
#include "../helpers/utility.h"
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

void SessionManager::getEntityAction(const QString &id, const int role)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id);

    auto errorLambda = [&, id](const QString &msgs, const int errorCode) {
        Q_UNUSED(msgs)
        Q_UNUSED(errorCode)
        emit entityActionDownloadedError(id);
    };

    auto finishLambda = [&, id, role](const QJsonDocument &reply) {
        const QJsonObject obj = reply.object();
        Enums::PlaceAction action = Enums::PlaceAction::InvalidPlaceAction;

        const QString ownerRole = obj.value(Tags::owner).toObject().value(Tags::role).toString();
        const int ownerRoleEnum = static_cast<int>(Utility::instance()->userTypeFromString(ownerRole));

        if (ownerRoleEnum == role) {
            const QString status = obj.value(Tags::status).toString();
            qDebug() << ownerRole << ownerRoleEnum << role << status;

            if (status == QStringLiteral("ARRIVED")) {
                action = Enums::PlaceAction::Arrived;
            } else if (status == QStringLiteral("DEPARTED")) {
                action = Enums::PlaceAction::Departed;
            }
        } else if (ownerRoleEnum != role - 1) {
            // Error!
            emit entityActionDownloadedError(id);
            return;
        }

        emit entityActionDownloaded(id, static_cast<int>(action));
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::putEntity(const QString &id, const int action)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id, static_cast<Enums::PlaceAction>(action));

    auto errorLambda = [&, id](const QString &, const int) {
        emit entitySaveResult(id, false);
    };

    auto finishLambda = [&, id](const QJsonDocument &) {
        emit entitySaveResult(id, true);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}
