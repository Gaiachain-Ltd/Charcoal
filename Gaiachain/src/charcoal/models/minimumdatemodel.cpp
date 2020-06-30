#include "minimumdatemodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

MinimumDateModel::MinimumDateModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
}

void MinimumDateModel::refresh()
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

void MinimumDateModel::setPlotId(const QString &id)
{
    m_plotId = id;

    if (m_plotId.isEmpty()) {
        setDbQuery(QString());
    } else {
        const QString plotName(CharcoalDbHelpers::getPlotName(id));
        const QString parentId(CharcoalDbHelpers::getEntityIdFromName(m_connectionName, plotName));

        setDbQuery(QString("SELECT date "
                           "FROM Events "
                           "WHERE entityId IN "
                           "(SELECT id FROM Entities WHERE id=\"%1\" "
                           "OR parent=\"%1\") "
                           "ORDER BY date DESC LIMIT 1").arg(parentId));
    }

    refresh();

    emit plotIdChanged(id);

    if (query().seek(0)) {
        setDate(query().value("date").toDateTime());
    }
}

QString MinimumDateModel::plotId() const
{
    return m_plotId;
}

QDateTime MinimumDateModel::date() const
{
    return m_date;
}

void MinimumDateModel::setDate(const QDateTime &date)
{
    if (m_date == date)
        return;

    m_date = date;
    emit dateChanged(m_date);
}
