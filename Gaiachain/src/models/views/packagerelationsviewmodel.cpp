#include "packagerelationsviewmodel.h"

#include "../query/idkeywordquery.h"

PackageRelationsViewModel::PackageRelationsViewModel(QObject *parent)
    : RelationModel(parent)
{}

void PackageRelationsViewModel::setSourceModel(SqlQueryModel *sourceModel)
{
    m_queryModel = QPointer<SqlQueryModel>(sourceModel);
    updateFilterQuery();

    AbstractReadModel::setSourceModel(sourceModel);
}

QString PackageRelationsViewModel::packageId() const
{
    return m_packageId;
}

QStringList PackageRelationsViewModel::relatedPackages() const
{
    auto relatedIds = QStringList{};

    auto relatedEntries = getData();
    std::transform(relatedEntries.constBegin(), relatedEntries.constEnd(),
                   std::back_inserter(relatedIds), [this](const auto &relatedEntry) {
        Q_ASSERT(relatedEntry.size() >= idShift(Columns::RelatedId));
        return relatedEntry[idShift(Columns::RelatedId)].toString();
    });

    return relatedIds;
}

void PackageRelationsViewModel::setPackageId(const QString &packageId)
{
    if (m_packageId == packageId) {
        return;
    }

    m_packageId = packageId;

    updateFilterQuery();
    emit packageIdChanged(m_packageId);
}

int PackageRelationsViewModel::idShift(const RelationModel::Columns &role) const
{
    return columnShift(role) - 1;
}

void PackageRelationsViewModel::updateFilterQuery()
{
    if (!m_queryModel.isNull()) {
        m_queryModel->setSortFilterQuery(IdKeywordQuery(m_packageId, true, SortFilterQuery()));
    }
}
