#include "unusedidsmodel.h"

#include "common/enums.h"

const QHash<int, QByteArray> UnusedIdsModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::PackageType,     QByteArrayLiteral("packageType") }
};

const QHash<int, QMetaType::Type> UnusedIdsModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::PackageType,     QMetaType::UInt }
};

const QHash<int, QMetaType::Type> UnusedIdsModel::sc_roleAppTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::PackageType,     static_cast<QMetaType::Type>(qMetaTypeId<Enums::PackageType>()) }
};

UnusedIdsModel::UnusedIdsModel(QObject *parent)
    : AbstractModel(parent)
{}

QString UnusedIdsModel::columnName(const UnusedIdsModel::Columns &column)
{
    return sc_roleNames.value(column);
}

int UnusedIdsModel::firstColumn() const
{
    return columnShift(PackageId);
}

int UnusedIdsModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QHash<int, QByteArray> UnusedIdsModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QMetaType::Type> UnusedIdsModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> UnusedIdsModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
