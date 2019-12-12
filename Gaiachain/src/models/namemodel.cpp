#include "namemodel.h"

const QHash<int, QByteArray> NameModel::sc_roleNames = {
    { Columns::Id,      QByteArrayLiteral("id") },
    { Columns::Name,    QByteArrayLiteral("name") }
};

const QHash<int, QMetaType::Type> NameModel::sc_roleDatabaseTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Name,    QMetaType::QString }
};

const QHash<int, QMetaType::Type> NameModel::sc_roleAppTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Name,    QMetaType::QString }
};

NameModel::NameModel(QObject *parent)
    : AbstractModel(parent)
{}

int NameModel::firstColumn() const
{
    return columnShift(Id);
}

int NameModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QHash<int, QByteArray> NameModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QMetaType::Type> NameModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> NameModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
