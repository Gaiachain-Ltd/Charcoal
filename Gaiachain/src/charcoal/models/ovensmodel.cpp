#include "ovensmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#include <QDebug>

OvensModel::OvensModel(QObject *parent) : QueryModel(parent)
{
}

void OvensModel::refresh()
{
    if (m_connectionName.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when connection name is empty!")
                   << m_query;
        return;
    }

    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
}

void OvensModel::setPlotId(const QString &id)
{
    m_plotId = id;

    setDbQuery(QString("SELECT id, name, height, length, width, carbonizationEvent "
                       "FROM Ovens WHERE plot IS (SELECT id FROM Entities WHERE name=\"%1\")").arg(m_plotId));

    refresh();
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
        return query().value("id").toInt();
    case Qt::ItemDataRole::DisplayRole:
    case OvenRole::LetterId:
        return query().value("name").toString();
    case OvenRole::FirstRow:
        return tr("Traditional oven - %1 x %2 x %3m")
            .arg(query().value("height").toString())
            .arg(query().value("length").toString())
            .arg(query().value("width").toString());
    case OvenRole::SecondRow:
    {
        const QString carbId(query().value("carbonizationEvent").toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("SELECT date FROM Events WHERE id=:carbonizationEvent");
        query.bindValue(":carbonizationEvent", carbId);

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
