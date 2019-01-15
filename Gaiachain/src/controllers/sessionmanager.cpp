#include "sessionmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(session, "session")

#include "overlaymanager.h"
#include "../common/tags.h"
#include "../common/globals.h"
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

    auto errorLambda = [&](const QString &, const int code) {
        m_overlayManager->setLoginRequest(false);
        emit displayLoginError(code);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntity()
{
    emit beforeGetEntity();
    auto request = QSharedPointer<EntityRequest>::create(m_token);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        const QJsonArray &array = reply.array();
        QJsonArray::const_iterator it = array.constBegin();
        QJsonArray batchArray;
        while(it != array.constEnd()) {
            batchArray.push_back(*it);
            if (batchArray.size() > MAX_BATCH_SIZE) {
                this->getEntities(batchArray);
                batchArray = QJsonArray();
            }
            ++it;
        }
        if (batchArray.size() > 0)
            this->getEntities(batchArray);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntity(const QString &id)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        emit entityLoaded(reply.object());
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntities(const QJsonArray &ids)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, ids);

    auto errorLambda = [&](const QString &, const int code) {
        emit entityLoadError(code);
    };

    auto finishLambda = [&](const QJsonDocument &reply) {
        QJsonArray array;
        const QJsonObject &obj = reply.object();
        QJsonObject::const_iterator it = obj.constBegin();
        while (it != obj.constEnd()) {
            array.push_back(*it);
            ++it;
        }
        emit entitiesLoaded(array);
    };

    connect(request.data(), &BaseRequest::requestFinished, finishLambda);
    connect(request.data(), &BaseRequest::replyError, errorLambda);

    m_client.send(request);
}

void SessionManager::getEntityAction(const QString &id, const int role)
{
    auto request = QSharedPointer<EntityRequest>::create(m_token, id);

    auto errorLambda = [&, id](const QString &, const int) {
        emit entityActionDownloadedError(id, false);
    };

    auto finishLambda = [&, id, role](const QJsonDocument &reply) {
        const QJsonObject obj = reply.object();
        Enums::PlaceAction action = Enums::PlaceAction::InvalidPlaceAction;

        const QString ownerRole = obj.value(Tags::owner).toObject().value(Tags::role).toString();
        const int ownerRoleEnum = static_cast<int>(Utility::instance()->userTypeFromString(ownerRole));

        if (ownerRoleEnum == role) {
            const QString status = obj.value(Tags::status).toString();

            if (status == QStringLiteral("ARRIVED")) {
                action = Enums::PlaceAction::Arrived;
            } else {
                emit entityActionDownloadedError(id, true);
            }
        } else if (ownerRoleEnum < role - 1) {
            // Error - not arrived yet
            emit entityActionDownloadedError(id, false);
            return;
        } else if (ownerRoleEnum > role) {
            // Error - already departed
            emit entityActionDownloadedError(id, true);
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
