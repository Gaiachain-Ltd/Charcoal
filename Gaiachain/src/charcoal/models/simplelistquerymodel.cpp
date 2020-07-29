#include "simplelistquerymodel.h"

#include "common/tags.h"

#include <QSqlQuery>

SimpleListQueryModel::SimpleListQueryModel(const bool hasActiveBit, QObject *parent)
    : QueryModel(parent), m_hasActiveBit(hasActiveBit), m_dbColumnForName(Tags::name)
{
}

QVariant SimpleListQueryModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case Qt::ItemDataRole::DisplayRole:
    case ListRole::Name:
        return query().value(m_dbColumnForName).toString();
    case ListRole::Id:
        return query().value(Tags::id).toInt();
    case ListRole::IsActive:
        if (m_hasActiveBit) {
            return query().value(Tags::active).toBool();
        } else {
            return true;
        }
    }

    return {};
}

QHash<int, QByteArray> SimpleListQueryModel::roleNames() const
{
    return m_roleNames;
}

void SimpleListQueryModel::setDbColumnForNameRole(const QString &dbColumn)
{
    m_dbColumnForName = dbColumn;
}
