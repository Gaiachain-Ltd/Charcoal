#include "localonlyproxymodel.h"

#include "eventmodel.h"
#include "../common/dataglobals.h"

LocalOnlyProxyModel::LocalOnlyProxyModel(QObject *parent)
    : AbstractSortFilterProxyModel(parent)
{}

Gaia::ModelData LocalOnlyProxyModel::getData() const
{
    auto modelData = Gaia::ModelData{};

    for (auto row = 0; row < rowCount(); ++row) {
        modelData.append({ data(index(row, 0), EventModel::Columns::PackageId),
                           data(index(row, 0), EventModel::Columns::Action),
                           data(index(row, 0), EventModel::Columns::Timestamp),
                           data(index(row, 0), EventModel::Columns::Properties),
                           data(index(row, 0), EventModel::Columns::LocationLat),
                           data(index(row, 0), EventModel::Columns::LocationLon)
                         });
    }

    return modelData;
}

bool LocalOnlyProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    return sourceModel()->data(index, EventModel::IsLocal).toBool();
}
