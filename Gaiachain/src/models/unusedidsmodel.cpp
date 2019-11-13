#include "unusedidsmodel.h"

#include "../common/enums.h"

namespace {
    const QLatin1String TableName = QLatin1String("UnusedIds");
}

const QHash<int, QByteArray> UnusedIdsModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::PackageType,     QByteArrayLiteral("packageType") }
};

const QHash<int, QVariant::Type> UnusedIdsModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::PackageType,     QVariant::UInt }
};

const QHash<int, QVariant::Type> UnusedIdsModel::sc_roleAppTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::PackageType,     static_cast<QVariant::Type>(qMetaTypeId<Enums::PackageType>()) }
};

UnusedIdsModel::UnusedIdsModel(QSqlDatabase db, QObject *parent)
    : AbstractModel(TableName, db, parent)
{}

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

QHash<int, QVariant::Type> UnusedIdsModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QVariant::Type> UnusedIdsModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
