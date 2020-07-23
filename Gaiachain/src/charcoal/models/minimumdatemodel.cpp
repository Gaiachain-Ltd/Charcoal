#include "minimumdatemodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"
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
        qWarning() << RED("Cannot establish DB connection when connection "
                          "name is empty!") << m_query;
        return;
    }

    if (shouldRefreshWebData()) {
        refreshWebData();
    }

    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));

    emit refreshed();
}

void MinimumDateModel::setPlotId(const int id)
{
    m_plotId = id;

    if (m_plotId == -1) {
        setDbQuery(QString());
    } else {
        setDbQuery(QString("SELECT eventDate FROM Events "
                           "WHERE entityId=%1 "
                           "ORDER BY date DESC LIMIT 1").arg(m_plotId));
    }

    refresh();

    emit plotIdChanged(m_plotId);

    if (query().seek(0)) {
        const qint64 timestamp = query().value(Tags::eventDate).toLongLong();
        setDate(QDateTime::fromSecsSinceEpoch(timestamp));
    }
}

int MinimumDateModel::plotId() const
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
