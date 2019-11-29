#include "namemodel.h"

const QHash<int, QByteArray> NameModel::sc_roleNames = {
    { Columns::Name,          QByteArrayLiteral("name") }
};

const QHash<int, QMetaType::Type> NameModel::sc_roleDatabaseTypes = {
    { Columns::Name,          QMetaType::QString }
};

const QHash<int, QMetaType::Type> NameModel::sc_roleAppTypes = {
    { Columns::Name,          QMetaType::QString }
};

NameModel::NameModel(QObject *parent)
    : AbstractModel(parent)
{}

const QString NameModel::columnName()
{
    return sc_roleNames.value(Columns::Name);
}

int NameModel::firstColumn() const
{
    return columnShift(Name);
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
