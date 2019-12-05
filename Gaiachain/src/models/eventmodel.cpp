#include "eventmodel.h"

const QHash<int, QByteArray> EventModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::Action,          QByteArrayLiteral("action") },
    { Columns::Timestamp,       QByteArrayLiteral("timestamp") },
    { Columns::CooperativeId,   QByteArrayLiteral("cooperativeId") },
    { Columns::Properties,      QByteArrayLiteral("properties") },
    { Columns::LocationLat,     QByteArrayLiteral("locationLat") },
    { Columns::LocationLon,     QByteArrayLiteral("locationLon") },
    { Columns::IsLocal,         QByteArrayLiteral("isLocal") },
    { Columns::LastUsed,        QByteArrayLiteral("lastUsed") }
};

const QHash<int, QMetaType::Type> EventModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::Action,          QMetaType::UInt },
    { Columns::Timestamp,       QMetaType::LongLong },
    { Columns::CooperativeId,   QMetaType::UInt },
    { Columns::Properties,      QMetaType::QByteArray },
    { Columns::LocationLat,     QMetaType::Double },
    { Columns::LocationLon,     QMetaType::Double },
    { Columns::IsLocal,         QMetaType::Bool },
    { Columns::LastUsed,        QMetaType::LongLong }
};

const QHash<int, QMetaType::Type> EventModel::sc_roleAppTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::Action,          static_cast<QMetaType::Type>(qMetaTypeId<Enums::SupplyChainAction>()) },
    { Columns::Timestamp,       QMetaType::QDateTime },
    { Columns::CooperativeId,   QMetaType::UInt },
    { Columns::Properties,      QMetaType::QVariantMap },
    { Columns::LocationLat,     QMetaType::Double },
    { Columns::LocationLon,     QMetaType::Double },
    { Columns::IsLocal,         QMetaType::Bool },
    { Columns::LastUsed,        QMetaType::QDateTime }
};

EventModel::EventModel(QObject *parent)
    : AbstractModel(parent)
{}

QString EventModel::columnName(const EventModel::Columns &column)
{
    return sc_roleNames.value(column);
}

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

QHash<int, QMetaType::Type> EventModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> EventModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
