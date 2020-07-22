#include "simplelistquerymodel.h"

#include "common/tags.h"

#include <QSqlQuery>

SimpleListQueryModel::SimpleListQueryModel(QObject *parent) : QueryModel(parent)
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
        return query().value(Tags::name).toString();
    case ListRole::Id:
        return query().value(Tags::id).toInt();
    }

    return {};
}

QHash<int, QByteArray> SimpleListQueryModel::roleNames() const
{
    return m_roleNames;
}
