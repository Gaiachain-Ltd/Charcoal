#include "ovensmodel.h"

#include "database/dbhelpers.h"
#include "common/logs.h"

#include <QSqlQuery>
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

    setDbQuery(QString("SELECT name FROM Ovens WHERE plot=%1").arg(m_plotId));
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

    const int row = index.row();
    query().seek(row);
    if (role == OvenRole::Id) {
        return {};
    }
//        if (isTraditional()) {
//            return tr("Traditional oven");
//        } else {
//            return tr("Metallic oven");
//        }
//    } else if (role == OvenTypesRole::Id) {
//        return query().value("id").toInt();
//    } else if (role == OvenTypesRole::Name) {
//        return query().value("name").toString();
//    } else if (role == OvenTypesRole::IsTraditionalOven) {
//        return isTraditional();
//    }

    return {};
}

QHash<int, QByteArray> OvensModel::roleNames() const
{
    return m_roleNames;
}
