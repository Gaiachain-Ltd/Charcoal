#include "notificationmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

NotificationManager::NotificationManager(QObject *parent) : AbstractManager(parent)
{
}

void NotificationManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("notificationManager"), this);
}

void NotificationManager::stepComplete(const Enums::SupplyChainAction step,
                                       const QString &id) const
{
    switch (step) {
    case Enums::SupplyChainAction::LoggingEnding:
        emit notify(Enums::Page::SupplyChainLoggingEnding,
                    tr("Logging process for <b>%1</b> has ended.").arg(id),
                    tr("Logs are ready to carbonize!"),
                    tr("Carbonization - beggining"));
        break;
    case Enums::SupplyChainAction::CarbonizationEnding:
        emit notify(Enums::Page::SupplyChainLoadingAndTransport,
                    tr("Carbonization process for <b>%1</b> has ended.").arg(id),
                    tr("Charcoal is ready for loading and transport!"),
                    tr("Loading and transport"));
        break;
    case Enums::SupplyChainAction::LoadingAndTransport:
        emit notify(Enums::Page::SupplyChainReception,
                    tr("Loading and transport process for <b>%1</b> has ended.").arg(id),
                    tr("If the truck has arrived, you can register it!"),
                    tr("Reception"));
        break;
    default:
        qDebug() << "Wrong";
    }
}
