#include "minimumdatemodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

MinimumDateModel::MinimumDateModel(QObject *parent) : QueryModel(parent)
{
}

void MinimumDateModel::refresh()
{
    if (m_connectionName.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when connection name is empty!")
                   << m_query;
        return;
    }

    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
}

void MinimumDateModel::setPlotId(const QString &id)
{
    m_plotId = id;

    if (m_plotId.isEmpty()) {
        setDbQuery(QString());
    } else {
        // TODO: reuse methods from ActionController here!
        const QStringList parts(id.split("/"));

        if (parts.length() < 3) {
            qWarning() << RED("Invalid ID passed to getPlotId") << id;
            return;
        }

        const QStringList plot(parts.mid(0, 3));
        const QString plotName(plot.join("/"));

        QString parentId;

        QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));

        q.prepare("SELECT id FROM Entities WHERE name=:nameString");
        q.bindValue(":nameString", plotName);

        if (q.exec()) {
            q.next();
            parentId = q.value("id").toString();
        } else {
            qWarning() << RED("Getting Entity ID has failed!")
                       << q.lastError().text()
                       << "for query:" << q.lastQuery()
                       << "DB:" << m_connectionName;
            return;
        }

        setDbQuery(QString("SELECT date "
                           "FROM Events "
                           "WHERE entityId IS "
                           "(SELECT id FROM Entities WHERE id=\"%1\" "
                           "OR parent=\"%1\") "
                           "ORDER BY date DESC LIMIT 1").arg(parentId));
    }

    refresh();

    emit plotIdChanged(id);

    query().seek(0);
    setDate(query().value("date").toDateTime());
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
