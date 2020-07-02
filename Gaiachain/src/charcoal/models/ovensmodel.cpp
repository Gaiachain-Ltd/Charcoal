#include "ovensmodel.h"

#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "common/logs.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#include <QDebug>

OvensModel::OvensModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
}

void OvensModel::refresh()
{
    if (m_connectionName.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when connection name is empty!")
                   << m_query;
        return;
    }

    if (shouldRefreshWebData()) {
        refreshWebData();
    }

    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));

    emit refreshed();
}

void OvensModel::setPlotId(const QString &id)
{
    m_plotId = id;

    setDbQuery(QString("SELECT id, type, name, oven_height, oven_length, oven_width, "
                       "carbonizationBeginning "
                       "FROM Ovens WHERE carbonizationEnding IS NULL "
                       "AND plot IS "
                       "(SELECT id FROM Entities WHERE name=\"%1\")").arg(m_plotId));

    refresh();

    qDebug() << "OVEN MODELS:" << rowCount();

    emit plotIdChanged(id);
}

QString OvensModel::plotId() const
{
    return m_plotId;
}

QVariant OvensModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case OvenRole::Id:
        return query().value("id").toString();
    case Qt::ItemDataRole::DisplayRole:
    case OvenRole::LetterId:
        return query().value("name").toString();
    case OvenRole::FirstRow:
    {
        const QString ovenTypeId(query().value("type").toString());
        QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
        q.prepare("SELECT type FROM OvenTypes WHERE id=:ovenTypeId");
        q.bindValue(":ovenTypeId", ovenTypeId);

        if (q.exec() == false) {
            qWarning() << RED("Getting oven type has failed!")
                       << q.lastError().text() << "for query:" << q.lastQuery();
            return {};
        }

        q.next();
        const int type(q.value("type").toInt());
        const bool isMetallic = (type == CharcoalDbHelpers::metalOvenType);

        return tr("%1 - %2 x %3 x %4m")
            .arg(isMetallic? tr("Metallic oven") : tr("Traditional oven"))
            .arg(query().value("oven_height").toString())
            .arg(query().value("oven_width").toString())
            .arg(query().value("oven_length").toString());
    }
    case OvenRole::SecondRow:
    {
        const QString carbId(query().value("carbonizationBeginning").toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("SELECT date FROM Events WHERE id=:carbonizationBeginning");
        query.bindValue(":carbonizationBeginning", carbId);

        if (query.exec() == false) {
            qWarning() << RED("Getting carbonization beginning date has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return {};
        }

        query.next();
        const QDate date(query.value("date").toDate());
        return tr("Carbonization beginning: %1").arg(date.toString("dd/MM/yyyy"));
    }
    }

    return {};
}

QHash<int, QByteArray> OvensModel::roleNames() const
{
    return m_roleNames;
}
