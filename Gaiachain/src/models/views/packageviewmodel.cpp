#include "packageviewmodel.h"

#include "../query/idkeywordquery.h"
#include "../../common/packagedata.h"
#include "../../common/dataglobals.h"

PackageViewModel::PackageViewModel(QObject *parent)
    : EventModel(parent)
{}

void PackageViewModel::setSourceModel(SqlQueryModel *sourceModel)
{
    m_queryModel = QPointer<SqlQueryModel>(sourceModel);
    updateFilterQuery();

    AbstractReadModel::setSourceModel(sourceModel);
}

QString PackageViewModel::packageId() const
{
    return m_packageId;
}

void PackageViewModel::fillPackageData(PackageData &packageData) const
{
    auto packageActions = getData();
    std::for_each(packageActions.constBegin(), packageActions.constEnd(),
                  [this, &packageData](const auto &actionEntry) {
        Q_ASSERT(actionEntry.size() >= idShift(Columns::Properties));
        packageData.cooperativeId = actionEntry[idShift(Columns::CooperativeId)].toUInt();

        const auto action = actionEntry[idShift(Columns::Action)].template value<Enums::SupplyChainAction>();
        const auto timestamp = actionEntry[idShift(Columns::Timestamp)].toDateTime();
        packageData.actionDates.insert(action, timestamp);

        const auto properties = actionEntry[idShift(Columns::Properties)].toMap();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        packageData.properties.insert(properties);
#else
        packageData.properties.unite(properties);
#endif
    });

    packageData.type = packageData.actionDates.isEmpty() ? Enums::PackageType::Unknown
                                                         : DataGlobals::packageType(packageData.actionDates.keys().first());
}

void PackageViewModel::setPackageId(const QString &packageId)
{
    if (m_packageId == packageId) {
        return;
    }

    m_packageId = packageId;

    updateFilterQuery();
    emit packageIdChanged(m_packageId);
}

int PackageViewModel::idShift(const EventModel::Columns &role) const
{
    return columnShift(role) - 1;
}

void PackageViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        m_queryModel->setSortFilterQuery(IdKeywordQuery(m_packageId, true, SortFilterQuery()));
    }
}
