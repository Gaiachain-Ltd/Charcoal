#pragma once

#include "common/enums.h"

#include <QString>

class QJsonDocument;
class QJsonObject;

class TrackingUpdater
{
public:
    TrackingUpdater(const QString &connectionName);

    bool updateTable(const QJsonDocument &json) const;

private:
    bool isValid() const;

    bool processTrackingItem(const QJsonObject &object) const;
    Enums::PackageType packageType(const QString &webType) const;

    QString m_connectionName;
};

