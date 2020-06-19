#include "relationmodel.h"

const QHash<int, QByteArray> RelationModel::sc_roleNames = {
    { Columns::PackageId,       QByteArrayLiteral("packageId") },
    { Columns::RelatedId,       QByteArrayLiteral("relatedId") },
    { Columns::LastUsed,        QByteArrayLiteral("lastUsed") }
};

const QHash<int, QMetaType::Type> RelationModel::sc_roleDatabaseTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::RelatedId,       QMetaType::QString },
    { Columns::LastUsed,        QMetaType::LongLong }
};

const QHash<int, QMetaType::Type> RelationModel::sc_roleAppTypes = {
    { Columns::PackageId,       QMetaType::QString },
    { Columns::RelatedId,       QMetaType::QString },
    { Columns::LastUsed,        QMetaType::QDateTime }
};

RelationModel::RelationModel(QObject *parent)
    : AbstractModel(parent)
{}

QString RelationModel::columnName(const RelationModel::Columns &column)
{
    return sc_roleNames.value(column);
}

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

QHash<int, QMetaType::Type> RelationModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> RelationModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
