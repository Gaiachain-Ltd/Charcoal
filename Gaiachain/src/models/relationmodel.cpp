#include "relationmodel.h"

namespace {
    const QLatin1String TableName = QLatin1String("Relations");
}

const QHash<int, QByteArray> RelationModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::RelatedId,       QByteArrayLiteral("relatedId") },
    { Columns::LastUsed,        QByteArrayLiteral("lastUsed") }
};

const QHash<int, QVariant::Type> RelationModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::RelatedId,       QVariant::String },
    { Columns::LastUsed,        QVariant::LongLong }
};

const QHash<int, QVariant::Type> RelationModel::sc_roleAppTypes = {
    { Columns::PackageId,       QVariant::String },
    { Columns::RelatedId,       QVariant::String },
    { Columns::LastUsed,        QVariant::DateTime }
};

RelationModel::RelationModel(QSqlDatabase db, QObject *parent)
    : AbstractModel(TableName, db, parent)
{}

int RelationModel::firstColumn() const
{
    return columnShift(PackageId);
}

int RelationModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QList<int> RelationModel::editableRoles() const
{
    return { Columns::LastUsed };
}

QHash<int, QByteArray> RelationModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QVariant::Type> RelationModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QVariant::Type> RelationModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
