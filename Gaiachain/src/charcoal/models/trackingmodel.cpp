#include "trackingmodel.h"

#include "database/dbhelpers.h"
#include "common/enums.h"
#include "common/logs.h"

#include <QSqlQuery>
#include <QSqlError>

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
        query.prepare("SELECT date FROM Events WHERE entityId=:id");
        query.bindValue(":id", id);

        if (query.exec() == false) {
            qWarning() << RED("Getting event details has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return {};
        }

        QVariantList events;
        while (query.next()) {
            const QDate date(query.value("date").toDate());
            const QString name(QString("Event %1").arg(events.count() + 1));
            const QVariantMap event {
                { "eventName", name },
                { "date", date }
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
