#include "eventmodel.h"

const QHash<int, QByteArray> EventModel::sc_roleNames = {
    { ModelRole::PackageId,     QByteArrayLiteral("packageId") },
    { ModelRole::UserRole,      QByteArrayLiteral("userRole") },
    { ModelRole::CooperativeId, QByteArrayLiteral("cooperativeId") },
    { ModelRole::Timestamp,     QByteArrayLiteral("timestamp") },
    { ModelRole::Action,        QByteArrayLiteral("action") },
    { ModelRole::Properties,    QByteArrayLiteral("properties") }
};

EventModel::EventModel(QObject *parent)
    : AbstractModel(parent)
{}

int EventModel::lastRole() const
{
    return LastRole;
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    return sc_roleNames;
}
