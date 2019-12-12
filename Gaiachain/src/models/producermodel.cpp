#include "producermodel.h"

const QHash<int, QByteArray> ProducerModel::sc_roleNames = {
    { Columns::Id,      QByteArrayLiteral("id") },
    { Columns::Code,    QByteArrayLiteral("code") },
    { Columns::Name,    QByteArrayLiteral("name") },
    { Columns::Village, QByteArrayLiteral("village") }
};

const QHash<int, QMetaType::Type> ProducerModel::sc_roleDatabaseTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Code,    QMetaType::QString },
    { Columns::Name,    QMetaType::QString },
    { Columns::Village, QMetaType::QString }
};

const QHash<int, QMetaType::Type> ProducerModel::sc_roleAppTypes = {
    { Columns::Id,      QMetaType::LongLong },
    { Columns::Code,    QMetaType::QString },
    { Columns::Name,    QMetaType::QString },
    { Columns::Village, QMetaType::QString }
};

ProducerModel::ProducerModel(QObject *parent)
    : AbstractModel(parent)
{}

QString ProducerModel::columnName(const ProducerModel::Columns &column)
{
    return sc_roleNames.value(column);
}

int ProducerModel::firstColumn() const
{
    return columnShift(Id);
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
