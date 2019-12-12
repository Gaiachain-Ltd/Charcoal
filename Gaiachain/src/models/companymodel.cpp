#include "companymodel.h"

#include "../common/enums.h"

const QHash<int, QByteArray> CompanyModel::sc_roleNames = {
    { Columns::Id,      QByteArrayLiteral("id") },
    { Columns::Code,    QByteArrayLiteral("code") },
    { Columns::Name,    QByteArrayLiteral("name") },
    { Columns::Type,    QByteArrayLiteral("type") }
};

const QHash<int, QMetaType::Type> CompanyModel::sc_roleDatabaseTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Code,    QMetaType::QString },
    { Columns::Name,    QMetaType::QString },
    { Columns::Type,    QMetaType::UInt }
};

const QHash<int, QMetaType::Type> CompanyModel::sc_roleAppTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Code,    QMetaType::QString },
    { Columns::Name,    QMetaType::QString },
    { Columns::Type,    static_cast<QMetaType::Type>(qMetaTypeId<Enums::CompanyType>()) }
};

CompanyModel::CompanyModel(QObject *parent)
    : AbstractModel(parent)
{}

QString CompanyModel::columnName(const CompanyModel::Columns &column)
{
    return sc_roleNames.value(column);
}

int CompanyModel::firstColumn() const
{
    return columnShift(Id);
}

int CompanyModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QHash<int, QByteArray> CompanyModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QMetaType::Type> CompanyModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> CompanyModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
