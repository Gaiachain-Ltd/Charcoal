#include "producermodel.h"

const QHash<int, QByteArray> ProducerModel::sc_roleNames = {
    { ModelRole::ProducerId,    QByteArrayLiteral("producerId") },
    { ModelRole::Name,          QByteArrayLiteral("name") },
    { ModelRole::Village,       QByteArrayLiteral("village") },
    { ModelRole::ParcelCodes,   QByteArrayLiteral("parcelCodes") }
};

ProducerModel::ProducerModel(QObject *parent)
    : AbstractModel(parent)
{}

int ProducerModel::lastRole() const
{
    return LastRole;
}

QHash<int, QByteArray> ProducerModel::roleNames() const
{
    return sc_roleNames;
}
