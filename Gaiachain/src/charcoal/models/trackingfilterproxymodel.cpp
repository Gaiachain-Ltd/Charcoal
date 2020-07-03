#include "trackingfilterproxymodel.h"

#include "charcoal/models/trackingmodel.h"

TrackingFilterProxyModel::TrackingFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

QString TrackingFilterProxyModel::searchString() const
{
    return m_searchString;
}

int TrackingFilterProxyModel::packageTypes() const
{
    return m_packageTypes;
}

Enums::PackageTypes TrackingFilterProxyModel::currentPackageTypes() const
{
    return Enums::PackageTypes(m_packageTypes);
}

void TrackingFilterProxyModel::setSearchString(const QString &searchString)
{
    if (m_searchString == searchString)
        return;

    m_searchString = searchString;
    invalidateFilter();
    emit searchStringChanged(m_searchString);
}

void TrackingFilterProxyModel::setPackageTypes(const int packageTypes)
{
    if (m_packageTypes == packageTypes)
        return;

    m_packageTypes = packageTypes;
    invalidateFilter();
    emit packageTypesChanged(m_packageTypes);
}

bool TrackingFilterProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex &source_parent) const
{
    const auto model(sourceModel());
    const QModelIndex source(model->index(source_row, 0, source_parent));
    const Enums::PackageType type = Enums::PackageType(
        model->data(source, TrackingModel::TrackingRole::Type).toInt());

    // Filter by type
    if (currentPackageTypes().testFlag(type) == false) {
        return false;
    }

    // Filter by search string
    const QString name(model->data(source, TrackingModel::TrackingRole::Name).toString());
    if (name.contains(m_searchString)) {
        return true;
    }

    return false;
}
