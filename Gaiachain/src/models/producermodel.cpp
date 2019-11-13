#include "producermodel.h"

namespace {
    const QLatin1String TableName = QLatin1String("Producers");
}

const QHash<int, QByteArray> ProducerModel::sc_roleNames = {
    { Columns::ProducerId,    QByteArrayLiteral("producerId") },
    { Columns::Name,          QByteArrayLiteral("name") },
    { Columns::Village,       QByteArrayLiteral("village") },
    { Columns::ParcelCodes,   QByteArrayLiteral("parcelCodes") }
};

const QHash<int, QVariant::Type> ProducerModel::sc_roleDatabaseTypes = {
    { Columns::ProducerId,    QVariant::String },
    { Columns::Name,          QVariant::String },
    { Columns::Village,       QVariant::String },
    { Columns::ParcelCodes,   QVariant::String }
};

const QHash<int, QVariant::Type> ProducerModel::sc_roleAppTypes = {
    { Columns::ProducerId,    QVariant::String },
    { Columns::Name,          QVariant::String },
    { Columns::Village,       QVariant::String },
    { Columns::ParcelCodes,   QVariant::StringList }
};

ProducerModel::ProducerModel(QSqlDatabase db, QObject *parent)
    : AbstractModel(TableName, db, parent)
{}

int ProducerModel::firstColumn() const
{
    return columnShift(ProducerId);
}

int ProducerModel::lastColumn() const
{
    return columnShift(LastColumn);
}

QHash<int, QByteArray> ProducerModel::roleNames() const
{
    return sc_roleNames;
}

QHash<int, QVariant::Type> ProducerModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QVariant::Type> ProducerModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
