#include "relationslistproxymodel.h"

#include "relationmodel.h"

RelationsListProxyModel::RelationsListProxyModel(QObject *parent)
    : AbstractIdentityProxyModel(parent)
{
}

QStringList RelationsListProxyModel::relatedPackages(const QString &packageId) const
{
    // INFO - in final implementation there will be a nicer solution than browsing all data
    auto relations = QStringList{};
    for (auto row = 0; row < rowCount(); ++row) {
        auto rowIndex = index(row, 0);

        auto id = data(rowIndex, RelationModel::PackageId).toString();
        if (id == packageId) {
            auto relatedId = data(rowIndex, RelationModel::RelatedId).toString();
            relations.append(relatedId);
        }
    }
    return relations;
}
