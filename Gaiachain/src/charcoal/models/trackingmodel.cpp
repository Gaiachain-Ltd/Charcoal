#include "trackingmodel.h"

#include "database/dbhelpers.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "trackingupdater.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>

#include <QDate>

TrackingModel::TrackingModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name, typeId FROM Entities");
}

QVariant TrackingModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case TrackingRole::Id:
        return query().value("id").toString();
    case TrackingRole::Name:
    case Qt::ItemDataRole::DisplayRole:
        return query().value("name").toString();
    case TrackingRole::Type:
    {
        const QString id(query().value("typeId").toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("SELECT name FROM EntityTypes WHERE id=:id");
        query.bindValue(":id", id);

        if (query.exec() == false) {
            qWarning() << RED("Getting event type has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return {};
        }

        query.next();
        const QString type(query.value("name").toString().toLower());

        if (type == "plot") {
            return int(Enums::PackageType::Plot);
        } else if (type == "harvest") {
            return int(Enums::PackageType::Harvest);
        } else if (type == "transport") {
            return int(Enums::PackageType::Transport);
        }

        return int(Enums::PackageType::Unknown);
    }
    case TrackingRole::Events:
    {
        const QString id(query().value("id").toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("SELECT id, date, typeId, properties FROM Events WHERE entityId=:id");
        query.bindValue(":id", id);

        if (query.exec() == false) {
            qWarning() << RED("Getting event details has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return {};
        }

        QVariantList events;
        while (query.next()) {
            const QDate date(query.value("date").toDate());
            Enums::SupplyChainAction action = CharcoalDbHelpers::actionById(
                m_connectionName, query.value("typeId").toInt());

            QString name;
            switch (action) {
            case Enums::SupplyChainAction::LoggingBeginning:
                name = tr("Logging has begun");
                break;
            case Enums::SupplyChainAction::LoggingEnding:
                name = tr("Logging has ended");
                break;
            case Enums::SupplyChainAction::CarbonizationBeginning:
            case Enums::SupplyChainAction::CarbonizationEnding:
            {
                const bool isBeginning = action == Enums::SupplyChainAction::CarbonizationBeginning;
                const QString begin(tr("Oven %1 - carbonization has begun"));
                const QString end(tr("Oven %1 - carbonization has ended"));
                const QString eventId(query.value("id").toString());
                const QString ovenQuery("SELECT name FROM Ovens WHERE %1=:eventId");

                QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
                if (isBeginning) {
                    q.prepare(ovenQuery.arg("carbonizationBeginning"));
                } else {
                    q.prepare(ovenQuery.arg("carbonizationEnding"));

                }
                q.bindValue(":eventId", eventId);

                if (q.exec() == false) {
                    qWarning() << RED("Getting oven name has failed!")
                               << q.lastError().text() << "for query:" << q.lastQuery();
                    return {};
                }

                q.next();
                const QString ovenName(q.value("name").toString());
                if (isBeginning) {
                    name = begin.arg(ovenName);
                } else {
                    name = end.arg(ovenName);
                }
            }
                break;
            case Enums::SupplyChainAction::LoadingAndTransport:
            {
                const QByteArray propertiesString(query.value("properties").toByteArray());
                const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
                const QVariantMap properties(propertiersJson.toVariant().toMap());
                name = tr("Bags have been loaded on truck %1")
                           .arg(properties.value(Tags::webPlateNumber).toString());
            }
                break;
            case Enums::SupplyChainAction::Reception:
                name = tr("Reception at storage facility");
                break;
            default: name = QString();
            }

            const QVariantMap event {
                { "eventName", name },
                { "date", date.toString("dd/MM/yyyy") }
            };

            events.append(event);
        }

        return events;
    }
    }

    return {};
}

QHash<int, QByteArray> TrackingModel::roleNames() const
{
    return m_roleNames;
}

void TrackingModel::refreshWebData()
{
    // TODO: limit amount of data with canFetchMore() etc.
    const auto request = QSharedPointer<BaseRequest>::create(
        "/entities/packages/",
        BaseRequest::Type::Get);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TrackingModel::webErrorHandler,
                                      &TrackingModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void TrackingModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Data is:" << reply;
    TrackingUpdater updater(m_connectionName);
    if (updater.updateTable(reply)) {
        emit webDataRefreshed();
    } else {
        emit error(tr("Error updating tracking information"));
    }
}
