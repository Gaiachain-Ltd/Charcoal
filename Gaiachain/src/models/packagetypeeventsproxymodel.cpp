#include "packagetypeeventsproxymodel.h"

#include "eventmodel.h"

#include "../common/dataglobals.h"
#include "../helpers/utility.h"

const QVector<Enums::PackageType> PackageTypeEventsProxyModel::sc_availablePackageTypes = {
        Enums::PackageType::Harvest, Enums::PackageType::Sac, Enums::PackageType::Lot };

PackageTypeEventsProxyModel::PackageTypeEventsProxyModel(QObject *parent)
    : AbstractIdentityProxyModel(parent)
{
    connect(this, &QAbstractItemModel::rowsInserted, this, &PackageTypeEventsProxyModel::onRowsInserted);
    connect(this, &QAbstractItemModel::rowsAboutToBeRemoved, this, &PackageTypeEventsProxyModel::onRowsToBeRemoved);
    connect(this, &QAbstractItemModel::modelReset, this, &PackageTypeEventsProxyModel::onModelReset);
}

QVector<Enums::PackageType> PackageTypeEventsProxyModel::availablePackageTypes()
{
    return sc_availablePackageTypes;
}

QHash<Enums::PackageType, int> PackageTypeEventsProxyModel::packageTypeEvents() const
{
    return m_packageTypeEvents;
}

QList<Enums::PackageType> PackageTypeEventsProxyModel::datePackageTypes(const QDate &date) const
{
    auto datePackageTypes = QList<Enums::PackageType>{};

    auto datePackageTypeEvents = m_datesPackageTypeEvents.value(date);
    for (const auto &packageType : sc_availablePackageTypes) {
        if (datePackageTypeEvents.value(packageType) > 0) {
            datePackageTypes.append(packageType);
        }
    }

    return datePackageTypes;
}

QVariantList PackageTypeEventsProxyModel::availablePackageTypesQml()
{
    return Utility::toVariantList(availablePackageTypes(), QMetaType::Int);
}

QVariantMap PackageTypeEventsProxyModel::packageTypeEventsQml() const
{
    return Utility::toVariantsMap(packageTypeEvents(), QMetaType::Int);
}

QVariantList PackageTypeEventsProxyModel::datePackageTypesQml(const QDate &date) const
{
    return Utility::toVariantList(datePackageTypes(date), QMetaType::Int);
}

void PackageTypeEventsProxyModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    auto packageTypesChanged = QSet<Enums::PackageType>{};
    auto datesChanged = QSet<QDate>{};

    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        auto action = data(rowIndex, EventModel::Action).value<Enums::SupplyChainAction>();
        auto packageType = DataGlobals::packageType(action);
        if (packageType == Enums::PackageType::Unknown) {
            qCWarning(dataModels) << "Unknown package type for action:" << action;
            continue;
        }

        // add package type event
        m_packageTypeEvents.insert(packageType, m_packageTypeEvents.value(packageType, 0) + 1);

        // add date package type event
        auto &datePackageTypeEvents = m_datesPackageTypeEvents[eventDate];
        datePackageTypeEvents.insert(packageType, datePackageTypeEvents.value(packageType, 0) + 1);

        packageTypesChanged.insert(packageType);
        if (datePackageTypeEvents.value(packageType) == 1) {   // first event on the date
            datesChanged.insert(eventDate);
        }
    }

    for (const auto &packageType : qAsConst(packageTypesChanged)) {
        emit packageTypeEventsChanged(packageType);
    }
    for (const auto &date : qAsConst(datesChanged)) {
        emit datePackageTypesChanged(date);
    }
}

void PackageTypeEventsProxyModel::onRowsToBeRemoved(const QModelIndex &parent, int first, int last)
{
    auto packageTypesChanged = QSet<Enums::PackageType>{};
    auto datesChanged = QSet<QDate>{};

    for (auto row = first; row <= last; ++row) {
        auto rowIndex = index(row, 0, parent);

        auto eventDate = data(rowIndex, EventModel::Timestamp).toDateTime().date();
        auto action = data(rowIndex, EventModel::Action).value<Enums::SupplyChainAction>();
        auto packageType = DataGlobals::packageType(action);
        if (packageType == Enums::PackageType::Unknown) {
            qCWarning(dataModels) << "Unknown package type for action:" << action;
            continue;
        }

        // add package type event
        m_packageTypeEvents.insert(packageType, m_packageTypeEvents.value(packageType, 1) - 1); // default 1 to avoid having negative

        // add date package type event
        auto &datePackageTypeEvents = m_datesPackageTypeEvents[eventDate];
        datePackageTypeEvents.insert(packageType, datePackageTypeEvents.value(packageType, 1) - 1); // default 1 to avoid having negative

        packageTypesChanged.insert(packageType);
        if (datePackageTypeEvents.value(packageType) == 0) {   // last event on the date
            datesChanged.insert(eventDate);
        }
    }

    for (const auto &packageType : qAsConst(packageTypesChanged)) {
        emit packageTypeEventsChanged(packageType);
    }
    for (const auto &date : qAsConst(datesChanged)) {
        emit datePackageTypesChanged(date);
    }
}

void PackageTypeEventsProxyModel::onModelReset()
{
    const auto dates = m_datesPackageTypeEvents.keys();

    m_packageTypeEvents.clear();
    m_datesPackageTypeEvents.clear();

    for (const auto &packageType : sc_availablePackageTypes) {
        emit packageTypeEventsChanged(packageType);
    }
    for (const auto &date : dates) {
        emit datePackageTypesChanged(date);
    }
}

