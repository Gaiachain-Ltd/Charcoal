#include "ovensmodel.h"

#include "database/dbhelpers.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "common/logs.h"
#include "common/tags.h"

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

void OvensModel::setPlotId(const int id)
{
    m_plotId = CharcoalDbHelpers::getParentEntityId(m_connectionName, id);

    setDbQuery(QString("SELECT id, type, name, oven_height, oven_length, oven_width, "
                       "carbonizationBeginning "
                       "FROM Ovens WHERE carbonizationEnding IS NULL "
                       "AND plot=%1").arg(m_plotId));

    refresh();

    qDebug() << "OVEN MODELS:" << id << rowCount();

    emit plotIdChanged(id);
}

int OvensModel::plotId() const
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
        return query().value(Tags::id).toString();
    case Qt::ItemDataRole::DisplayRole:
    case OvenRole::LetterId:
        return query().value(Tags::name).toString();
    case OvenRole::FirstRow:
    {
        const int ovenTypeId(query().value(Tags::type).toInt());
        const Enums::OvenType type = CharcoalDbHelpers::getOvenType(
            m_connectionName, ovenTypeId);
        const bool isMetallic = (type == Enums::OvenType::Metallic);

        return tr("%1 - %2 x %3 x %4m")
            .arg(isMetallic? tr("Metallic oven") : tr("Traditional oven"))
            .arg(query().value(Tags::webOvenHeight).toString())
            .arg(query().value(Tags::webOvenWidth).toString())
            .arg(query().value(Tags::webOvenLength).toString());
    }
    case OvenRole::SecondRow:
    {
        const int carbId(query().value("carbonizationBeginning").toInt());
        QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
        q.prepare("SELECT eventDate FROM Events WHERE id=:carbonizationBeginning");
        q.bindValue(":carbonizationBeginning", carbId);

        if (q.exec() == false) {
            qWarning() << RED("Getting carbonization beginning date has failed!")
                       << q.lastError().text() << "for query:" << q.lastQuery();
            return {};
        }

        q.next();
        const qint64 timestamp = q.value(Tags::eventDate).toLongLong();
        const QDateTime date = QDateTime::fromSecsSinceEpoch(timestamp);
        return tr("Carbonization beginning: %1").arg(date.toString("dd/MM/yyyy"));
    }
    }

    return {};
}

QHash<int, QByteArray> OvensModel::roleNames() const
{
    return m_roleNames;
}
