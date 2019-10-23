#include "packagedataproxymodel.h"

#include "eventmodel.h"
#include "../common/dataglobals.h"

PackageDataProxyModel::PackageDataProxyModel(QObject *parent)
    : AbstractIdentityProxyModel(parent)
{
}

void PackageDataProxyModel::fillPackageData(PackageData &packageData) const
{
    // INFO - in final implementation there will be a nicer solution than browsing all data
    for (auto row = 0; row < rowCount(); ++row) {
        auto rowIndex = index(row, 0);

        auto id = data(rowIndex, EventModel::PackageId).toString();
        if (id == packageData.id) {
            auto action = data(rowIndex, EventModel::Action).value<Enums::SupplyChainAction>();
            auto timestamp = data(rowIndex, EventModel::Timestamp).toDateTime();
            packageData.actionDates.insert(action, timestamp);

            auto properties = data(rowIndex, EventModel::Properties).toMap();
            packageData.properties.unite(properties);
        }
    }

    packageData.type = packageData.actionDates.isEmpty() ? Enums::PackageType::Unknown
                                                         : DataGlobals::packageType(packageData.actionDates.keys().first());
}
