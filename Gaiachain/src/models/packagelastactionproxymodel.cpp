#include "packagelastactionproxymodel.h"

#include "eventmodel.h"
#include "../common/enums.h"
#include "../common/dataglobals.h"

PackageLastActionProxyModel::PackageLastActionProxyModel(const Enums::SupplyChainAction &lastAction, QObject *parent)
    : AbstractSortFilterProxyModel(parent), m_lastAction(lastAction)
{}

void PackageLastActionProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    if (this->sourceModel()) {
        disconnect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this, &AbstractSortFilterProxyModel::resetFilter);
    }

    AbstractSortFilterProxyModel::setSourceModel(sourceModel);

    if (this->sourceModel()) {
        connect(this->sourceModel(), &QAbstractItemModel::rowsInserted, this, &AbstractSortFilterProxyModel::resetFilter);
    }
}

Gaia::ModelData PackageLastActionProxyModel::getData() const
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

bool PackageLastActionProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto index = sourceModel()->index(sourceRow, 0, sourceParent);

    auto action = sourceModel()->data(index, EventModel::Action).value<Enums::SupplyChainAction>();
    if (action == m_lastAction) {
        auto packageId = sourceModel()->data(index, EventModel::PackageId).toString();

        auto actionsListIndexes = sourceModel()->match(sourceModel()->index(0, 0), EventModel::PackageId, packageId, -1, Qt::MatchExactly);
        auto laterActionIt = std::find_if(actionsListIndexes.constBegin(), actionsListIndexes.constEnd(),
                                          [this](const auto &index) {
            auto action = sourceModel()->data(index, EventModel::Action).template value<Enums::SupplyChainAction>();
            return (action > m_lastAction);
        });
        return (laterActionIt == actionsListIndexes.constEnd());    // no later action
    }
    return false;
}
