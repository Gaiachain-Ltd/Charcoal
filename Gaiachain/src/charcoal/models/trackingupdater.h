#pragma once

#include "common/enums.h"

#include <QVector>
#include <QString>
#include <QPointer>

class QJsonDocument;
class QJsonObject;
class PicturesManager;

struct UpdateResult {
    bool success = false;
    QVector<int> toReplant;
    QVector<int> toFinish;
};

class TrackingUpdater
{
public:
    TrackingUpdater(const QString &connectionName);

    void setPicturesManager(PicturesManager *manager);

    UpdateResult updateTable(const QJsonDocument &json) const;
    bool updateDetails(const QJsonDocument &json) const;

private:
    bool isValid() const;

    UpdateResult processTrackingItem(const QJsonObject &object, UpdateResult result) const;

    bool processDetailsLoggingBeginning(const QJsonObject &object) const;
    bool processDetailsLoggingEnding(const QJsonObject &object) const;
    bool processDetailsOvens(const QString &packageId, const QJsonArray &array) const;
    bool processDetailsLoadingAndTransport(const QString &packageName,
                                           const QJsonObject &object) const;
    bool processDetailsReception(const QJsonObject &object) const;

    bool updateEventDetails(const int webId, const qint64 timestamp,
                            const QVariantMap &properties) const;

    QStringList getQrCodes(const QJsonArray &codes) const;
    QStringList getImages(const QJsonArray &images) const;

    Enums::PackageType packageType(const QString &webType) const;

    QString m_connectionName;
    QPointer<PicturesManager> m_picturesManager;
};

