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

    Q_INVOKABLE void getAdditionalData();

    Q_INVOKABLE void getEntitiesInfo(const QDateTime &from, const QDateTime &to);
    Q_INVOKABLE void getEntitiesInfo(int limit, int offset, const QDateTime &from,
                                     const QDateTime &to);
    Q_INVOKABLE void getEntitiesInfo(int limit, int offset, const QString &keyword,
                                     const QSet<Enums::PackageType> &filteredPackages,
                                     int cooperativeId);
    Q_INVOKABLE void getLastActionEntitiesInfo(
        const Enums::SupplyChainAction &lastAction);

    Q_INVOKABLE void getEntities(const QStringList &packageIds);

    Q_INVOKABLE void postNewEntity(const QString &packageId,
                                   const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate,
                                   const QDateTime &timestamp,
                                   const QVariantMap &properties);

    Q_INVOKABLE void postNewEntity(const QString &packageId,
                                   const QByteArray &codeData,
                                   const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate,
                                   const QDateTime &timestamp,
                                   const QVariantMap &properties);

    Q_INVOKABLE void postNewEntity(const QByteArray &codeData,
                                   const Enums::SupplyChainAction &action,
                                   const QGeoCoordinate &coordinate,
                                   const QDateTime &timestamp,
                                   const QVariantMap &properties);

signals:
    void additionalDataLoadError(const QNetworkReply::NetworkError &code) const;
    void additionalDataLoaded(const QJsonObject &data) const;

    void entitiesLoadError(const QNetworkReply::NetworkError &code) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void entitySaveError(const QString &packageId, const QByteArray &codeData,
                         const Enums::SupplyChainAction &action,
                         const QNetworkReply::NetworkError &code) const;
    void entitySaved(const QString &packageId, const QByteArray &codeData,
                     const Enums::SupplyChainAction &action) const;

    void unusedLotIdsLoadError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdsLoaded(const QJsonArray &packageIds) const;
    void unusedLotIdCreateError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdCreated(const QString &packageId) const;
};
