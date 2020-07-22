#include "notificationmanager.h"

#include "helpers/requestshelper.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/tags.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

NotificationManager::NotificationManager(QObject *parent) : AbstractManager(parent)
{
    // Ping for notifications every 5 seconds
    m_notificationTimer.setInterval(5000);
    m_notificationTimer.setSingleShot(false);
    m_notificationTimer.setTimerType(Qt::TimerType::VeryCoarseTimer);

    connect(&m_notificationTimer, &QTimer::timeout,
            this, &NotificationManager::checkNotifications);

    m_notificationTimer.start();
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
    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get()) == false) {
        return;
    }

    // General "notifications" endpoint - Web will know which user is calling
    // from the token
    const auto request = QSharedPointer<BaseRequest>::create(
        "/notifications/",
        BaseRequest::Type::Get
        );

    request->setQuiet(true);
    request->setToken(m_sessionManager->token());
    m_sessionManager->sendRequest(request, this,
                                  &NotificationManager::webErrorHandler,
                                  &NotificationManager::webReplyHandler,
                                  false, false);
}

void NotificationManager::stepComplete(const Enums::SupplyChainAction nextStep,
                                       const QString &pid) const
{
    switch (nextStep) {
    case Enums::SupplyChainAction::LoggingEnding:
        emit notify(Enums::Page::SupplyChainLoggingEnding,
                    tr("Logging process for <b>%1</b> has begun.").arg(pid),
                    tr("Trees are ready to be felled!"),
                    tr("Logging - ending"));
        break;
    case Enums::SupplyChainAction::CarbonizationBeginning:
        emit notify(Enums::Page::SupplyChainCarbonizationBeginning,
                    tr("Logging process for <b>%1</b> has ended.").arg(pid),
                    tr("Logs are ready to carbonize!"),
                    tr("Carbonization - beggining"));
        break;
    case Enums::SupplyChainAction::CarbonizationEnding:
        emit notify(Enums::Page::SupplyChainCarbonizationEnding,
                    tr("Carbonization process for <b>%1</b> has begun.").arg(pid),
                    tr("End carbonization when wood is fully burned!"),
                    tr("Carbonization - ending"));
        break;
    case Enums::SupplyChainAction::LoadingAndTransport:
        emit notify(Enums::Page::SupplyChainLoadingAndTransport,
                    tr("Carbonization process for <b>%1</b> has ended.").arg(pid),
                    tr("Charcoal is ready for loading and transport!"),
                    tr("Loading and transport"));
        break;
    case Enums::SupplyChainAction::Reception:
        emit notify(Enums::Page::SupplyChainReception,
                    tr("Loading and transport process for <b>%1</b> has ended.").arg(pid),
                    tr("If the truck has arrived, you can register it!"),
                    tr("Reception"));
        break;
    case Enums::SupplyChainAction::Replantation:
        emit notify(Enums::Page::PageReplantation,
                    tr("Reception process for <b>%1</b> has ended.").arg(pid),
                    tr("If the parcel has been replanted, register it!"),
                    tr("Replantation"));
        break;
    case Enums::SupplyChainAction::LoggingBeginning:
    case Enums::SupplyChainAction::Tracking:
    case Enums::SupplyChainAction::Unknown:
    case Enums::SupplyChainAction::SupplyChainActionCount:
        qDebug() << "Wrong step for notification" << nextStep << pid;
    }
}

void NotificationManager::webErrorHandler(const QString &errorString,
                                          const QNetworkReply::NetworkError code) const
{
    qWarning() << "Notification ping reply error!" << errorString << code;
}

void NotificationManager::webReplyHandler(const QJsonDocument &reply) const
{
    const QJsonObject object = reply.object();

    const int count = object.value("count").toInt();
    const QJsonArray results = object.value("results").toArray();

    if (count <= 0) {
        return;
    }

    qDebug() << "Notification ping success!" << reply;
    const QJsonValue value = results.last();
    const QJsonObject currentObject = value.toObject();
    const QString pid(currentObject.value("package_pid").toString());
    const QString actionName(currentObject.value("action_name").toString());
    const Enums::SupplyChainAction step = CharcoalDbHelpers::actionByName(
        actionName);

    stepComplete(step, pid);
    // We accept only one notification at a time
    return;
}
