#pragma once

#include "common/enums.h"

#include <QString>
#include <QPointer>

class QJsonDocument;
class QJsonObject;
class PicturesManager;

class TrackingUpdater
{
public:
    TrackingUpdater(const QString &connectionName);

    void setPicturesManager(PicturesManager *manager);

    bool updateTable(const QJsonDocument &json) const;
    bool updateDetails(const QJsonDocument &json) const;

private:
    bool isValid() const;

    bool processTrackingItem(const QJsonObject &object) const;

    bool processDetailsLoggingBeginning(const int webId, const QJsonObject &object) const;
    bool processDetailsLoggingEnding(const int webId, const QJsonObject &object) const;
    bool processDetailsOvens(const int webId, const QJsonArray &array) const;
    bool processDetailsLoadingAndTransport(const int webId,
                                           const QString &packageName,
                                           const QJsonObject &object) const;
    bool processDetailsReception(const int webId, const QJsonObject &object) const;

    bool updateEventDetails(const int webId, const int typeId,
                            const QVariantMap &properties) const;

    QStringList getQrCodes(const QJsonArray &codes) const;
    QStringList getImages(const QJsonArray &images) const;

    Enums::PackageType packageType(const QString &webType) const;

    QString m_connectionName;
    QPointer<PicturesManager> m_picturesManager;
};

