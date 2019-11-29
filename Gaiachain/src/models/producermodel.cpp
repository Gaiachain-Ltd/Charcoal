#include "producermodel.h"

const QHash<int, QByteArray> ProducerModel::sc_roleNames = {
    { Columns::ProducerId,    QByteArrayLiteral("producerId") },
    { Columns::Name,          QByteArrayLiteral("name") },
    { Columns::Village,       QByteArrayLiteral("village") },
    { Columns::ParcelCodes,   QByteArrayLiteral("parcelCodes") }
};

const QHash<int, QMetaType::Type> ProducerModel::sc_roleDatabaseTypes = {
    { Columns::ProducerId,    QMetaType::QString },
    { Columns::Name,          QMetaType::QString },
    { Columns::Village,       QMetaType::QString },
    { Columns::ParcelCodes,   QMetaType::QString }
};

const QHash<int, QMetaType::Type> ProducerModel::sc_roleAppTypes = {
    { Columns::ProducerId,    QMetaType::QString },
    { Columns::Name,          QMetaType::QString },
    { Columns::Village,       QMetaType::QString },
    { Columns::ParcelCodes,   QMetaType::QStringList }
};

ProducerModel::ProducerModel(QObject *parent)
    : AbstractModel(parent)
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

QHash<int, QMetaType::Type> ProducerModel::roleDatabaseTypes() const
{
    return sc_roleDatabaseTypes;
}

QHash<int, QMetaType::Type> ProducerModel::roleAppTypes() const
{
    return sc_roleAppTypes;
}
