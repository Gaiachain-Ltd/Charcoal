#include "namemodel.h"

const QHash<int, QByteArray> NameModel::sc_roleNames = {
    { Columns::Name,          QByteArrayLiteral("name") }
};

const QHash<int, QVariant::Type> NameModel::sc_roleDatabaseTypes = {
    { Columns::Name,          QVariant::String }
};

const QHash<int, QVariant::Type> NameModel::sc_roleAppTypes = {
    { Columns::Name,          QVariant::String }
};

NameModel::NameModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent)
    : AbstractModel(tableName, db, parent)
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

QHash<int, QVariant::Type> NameModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QVariant::Type> NameModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
