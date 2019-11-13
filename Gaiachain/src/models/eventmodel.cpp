#include "eventmodel.h"

#include "../common/enums.h"

namespace {
    const QLatin1String TableName = QLatin1String("Events");
}

const QHash<int, QByteArray> EventModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::Action,          QByteArrayLiteral("action") },
    { Columns::Timestamp,       QByteArrayLiteral("timestamp") },
    { Columns::UserRole,        QByteArrayLiteral("userRole") },
    { Columns::CooperativeId,   QByteArrayLiteral("cooperativeId") },
    { Columns::Properties,      QByteArrayLiteral("properties") },
    { Columns::LocationLat,     QByteArrayLiteral("locationLat") },
    { Columns::LocationLon,     QByteArrayLiteral("locationLon") },
    { Columns::IsLocal,         QByteArrayLiteral("isLocal") },
    { Columns::LastUsed,        QByteArrayLiteral("lastUsed") }
};

const QHash<int, QVariant::Type> EventModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::Action,          QVariant::UInt },
    { Columns::Timestamp,       QVariant::LongLong },
    { Columns::UserRole,        QVariant::UInt },
    { Columns::CooperativeId,   QVariant::String },
    { Columns::Properties,      QVariant::ByteArray },
    { Columns::LocationLat,     QVariant::Double },
    { Columns::LocationLon,     QVariant::Double },
    { Columns::IsLocal,         QVariant::Bool },
    { Columns::LastUsed,        QVariant::LongLong }
};

const QHash<int, QVariant::Type> EventModel::sc_roleAppTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::Action,          static_cast<QVariant::Type>(qMetaTypeId<Enums::SupplyChainAction>()) },
    { Columns::Timestamp,       QVariant::DateTime },
    { Columns::UserRole,        QVariant::UInt },
    { Columns::CooperativeId,   QVariant::String },
    { Columns::Properties,      QVariant::Map },
    { Columns::LocationLat,     QVariant::Double },
    { Columns::LocationLon,     QVariant::Double },
    { Columns::IsLocal,         QVariant::Bool },
    { Columns::LastUsed,        QVariant::DateTime }
};

EventModel::EventModel(QSqlDatabase db, QObject *parent)
    : AbstractModel(TableName, db, parent)
{}

int EventModel::firstColumn() const
{
    return columnShift(PackageId);
}

int EventModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QList<int> EventModel::editableRoles() const
{
    return { Columns::IsLocal, Columns::LastUsed };
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QVariant::Type> EventModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QVariant::Type> EventModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
