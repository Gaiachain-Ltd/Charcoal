#include "parcelmodel.h"

const QHash<int, QByteArray> ParcelModel::sc_roleNames = {
    { Columns::Id,          QByteArrayLiteral("id") },
    { Columns::Code,        QByteArrayLiteral("code") },
    { Columns::ProducerId,  QByteArrayLiteral("producerId") }
};

const QHash<int, QMetaType::Type> ParcelModel::sc_roleDatabaseTypes = {
    { Columns::Id,          QMetaType::LongLong },
    { Columns::Code,        QMetaType::QString },
    { Columns::ProducerId,  QMetaType::LongLong }
};

const QHash<int, QMetaType::Type> ParcelModel::sc_roleAppTypes = {
    { Columns::Id,          QMetaType::LongLong },
    { Columns::Code,        QMetaType::QString },
    { Columns::ProducerId,  QMetaType::LongLong }
};

ParcelModel::ParcelModel(QObject *parent)
    : AbstractModel(parent)
{}

QString ParcelModel::columnName(const ParcelModel::Columns &column)
{
    return sc_roleNames.value(column);
}

int ParcelModel::firstColumn() const
{
    return columnShift(Id);
}

int ParcelModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QHash<int, QByteArray> ParcelModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QMetaType::Type> ParcelModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> ParcelModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
