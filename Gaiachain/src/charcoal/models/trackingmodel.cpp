#include "trackingmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>

#include <QDate>

TrackingModel::TrackingModel(QObject *parent) : QueryModel(parent)
{
//    setDbQuery("SELECT Entities.id, Entities.name, Events.typeId, Events.date "
//               "FROM Entities "
//               "INNER JOIN Events"
//               "ON Events.entityId=Entities.id");

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
            Enums::SupplyChainAction action = eventType(query.value("typeId").toString());

            QString name;
            switch (action) {
            case Enums::SupplyChainAction::LoggingBeginning:
                name = tr("Logging has begun");
                break;
            case Enums::SupplyChainAction::LoggingEnding:
                name = tr("Logging has ended");
                break;
            case Enums::SupplyChainAction::CarbonizationBeginning:
            {
                // TODO: extract oven name from ID!
                const QByteArray propertiesString(query.value("properties").toByteArray());
                const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
                const QVariantMap properties(propertiersJson.toVariant().toMap());
                name = tr("Oven %1 - carbonization has begun")
                           .arg(properties.value("ovenId").toString());
            }
                break;
            case Enums::SupplyChainAction::CarbonizationEnding:
            {
                // TODO: extract oven name from ID!
                const QByteArray propertiesString(query.value("properties").toByteArray());
                const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
                const QVariantMap properties(propertiersJson.toVariant().toMap());
                name = tr("Oven %1 - carbonization has ended")
                           .arg(properties.value("ovenId").toString());
            }
                break;
            case Enums::SupplyChainAction::LoadingAndTransport:
            {
                const QByteArray propertiesString(query.value("properties").toByteArray());
                const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
                const QVariantMap properties(propertiersJson.toVariant().toMap());
                name = tr("Bags have been loaded on truck %1")
                           .arg(properties.value("plateNumber").toString());
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

Enums::SupplyChainAction TrackingModel::eventType(const QString &id) const
{
    QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
    query.prepare("SELECT actionName FROM EventTypes WHERE id=:id");
    query.bindValue(":id", id);

    if (query.exec() == false) {
        qWarning() << RED("Getting event type has failed!")
                   << query.lastError().text() << "for query:" << query.lastQuery();
        return {};
    }

    query.next();
    const QString name(query.value("actionName").toString());

    if (name == "LB") {
        return Enums::SupplyChainAction::LoggingBeginning;
    } else if (name == "LE") {
        return Enums::SupplyChainAction::LoggingEnding;
    } else if (name == "CB") {
        return Enums::SupplyChainAction::CarbonizationBeginning;
    } else if (name == "CE") {
        return Enums::SupplyChainAction::CarbonizationEnding;
    } else if (name == "TR") {
        return Enums::SupplyChainAction::LoadingAndTransport;
    } else if (name == "RE") {
        return Enums::SupplyChainAction::Reception;
    }

    return Enums::SupplyChainAction::Unknown;
}
