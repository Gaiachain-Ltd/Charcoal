#include "notificationmanager.h"

#include "charcoal/database/charcoaldbhelpers.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/tags.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonObject>

#include <QDebug>

NotificationManager::NotificationManager(QObject *parent) : AbstractManager(parent)
{
    // Ping for notifications every 5 seconds
    m_notificationTimer.setInterval(5000);
    m_notificationTimer.setSingleShot(false);
    m_notificationTimer.setTimerType(Qt::TimerType::VeryCoarseTimer);

    connect(&m_notificationTimer, &QTimer::timeout,
            this, &NotificationManager::checkNotifications);
}

void NotificationManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(
                QStringLiteral("notificationManager"), this);
}

void NotificationManager::setSessionManager(const QSharedPointer<RestSessionManager> &manager)
{
    m_sessionManager = manager;
}

void NotificationManager::setUserManager(const QSharedPointer<UserManager> &manager)
{
    m_userManager = manager;
}

void NotificationManager::checkNotifications()
{
    const bool isLoggedIn = m_userManager->isLoggedIn();
    if (isLoggedIn == false) {
        return;
    }

    // TODO: use real URL from Web!
    const auto request = QSharedPointer<BaseRequest>::create(
        "/entities/notifications/",
        BaseRequest::Type::Post
        );

    request->setToken(m_sessionManager->token());
    m_sessionManager->sendRequest(request, this,
                                  &NotificationManager::webErrorHandler,
                                  &NotificationManager::webReplyHandler);
}

void NotificationManager::stepComplete(const Enums::SupplyChainAction step,
                                       const QString &pid) const
{
    switch (step) {
    case Enums::SupplyChainAction::LoggingEnding:
        emit notify(Enums::Page::SupplyChainCarbonizationBeginning,
                    tr("Logging process for <b>%1</b> has ended.").arg(pid),
                    tr("Logs are ready to carbonize!"),
                    tr("Carbonization - beggining"));
        break;
    case Enums::SupplyChainAction::CarbonizationEnding:
        emit notify(Enums::Page::SupplyChainLoadingAndTransport,
                    tr("Carbonization process for <b>%1</b> has ended.").arg(pid),
                    tr("Charcoal is ready for loading and transport!"),
                    tr("Loading and transport"));
        break;
    case Enums::SupplyChainAction::LoadingAndTransport:
        emit notify(Enums::Page::SupplyChainReception,
                    tr("Loading and transport process for <b>%1</b> has ended.").arg(pid),
                    tr("If the truck has arrived, you can register it!"),
                    tr("Reception"));
        break;
    default:
        qDebug() << "Wrong step for notification" << step << pid;
    }
}

void NotificationManager::webErrorHandler(const QString &errorString,
                                          const QNetworkReply::NetworkError code) const
{
    qDebug() << "Notification ping reply error!" << errorString << code;
}

void NotificationManager::webReplyHandler(const QJsonDocument &reply) const
{
    qDebug() << "Notification ping success!" << reply;

    const QString pid(reply.object().value(Tags::pid).toString());
    const QString actionName(reply.object().value(Tags::action).toString());
    const Enums::SupplyChainAction step = CharcoalDbHelpers::actionByName(
        actionName);

    stepComplete(step, pid);
}
