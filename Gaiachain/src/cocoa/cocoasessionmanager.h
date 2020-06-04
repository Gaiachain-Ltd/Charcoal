#pragma once

#include "controllers/session/restsessionmanager.h"

#include <QObject>

class CocoaSessionManager : public RestSessionManager
{
    Q_OBJECT

public:
    CocoaSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void getProducers();
    Q_INVOKABLE void getCompanies();
    Q_INVOKABLE void getDestinations();

    Q_INVOKABLE void getUnusedLotIds();
    Q_INVOKABLE void postUnusedLotId();

signals:
    void unusedLotIdsLoadError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdsLoaded(const QJsonArray &packageIds) const;
    void unusedLotIdCreateError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdCreated(const QString &packageId) const;
};
