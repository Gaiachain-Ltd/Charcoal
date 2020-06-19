#pragma once

#include "controllers/data/abstractdatamanager.h"

#include "controllers/data/datarequestsmanager.h"
#include "controllers/data/datamodelsmanager.h"
#include "controllers/data/datalocalmanager.h"
#include "controllers/data/dataviewmanager.h"

#include <QObject>

class CocoaDataManager : public AbstractDataManager
{
    Q_OBJECT

public:
    CocoaDataManager(QObject *parent = nullptr);

    Q_INVOKABLE QString generateHarvestId(const QDate &date,
                                          const QString &parcelCode);
    void setupQmlContext(QQmlApplicationEngine &engine) override;
    void setupDatabase(const QString &dbPath) override;

    void updateUserData(const UserData &userData) override;
    bool processing() const override;

    Q_INVOKABLE void preparePackageData(const QString &packageId);

    Q_INVOKABLE void addAction(const QString &packageId,
                               const Enums::SupplyChainAction &action,
                               const QGeoCoordinate &coordinate,
                               const QDateTime &timestamp,
                               const QVariantMap &properties);

    Q_INVOKABLE void addAction(const QString &packageId,
                               const Enums::SupplyChainAction &action,
                               const QByteArray &codeData,
                               const QGeoCoordinate &coordinate,
                               const QDateTime &timestamp,
                               const QVariantMap &properties);

    Q_INVOKABLE void addAction(const Enums::SupplyChainAction &action,
                               const QByteArray &codeData,
                               const QGeoCoordinate &coordinate,
                               const QDateTime &timestamp,
                               const QVariantMap &properties);

    Q_INVOKABLE void sendOfflineActions() override;
    Q_INVOKABLE void removeOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action);

    Q_INVOKABLE void fetchEventData(const QString &packageId, const Enums::PackageType &type);
    Q_INVOKABLE void fetchRangeEvents(const QDateTime &from, const QDateTime &to);
    Q_INVOKABLE void fetchLimitRangeEvents(int limit, int offset, const QDateTime &from, const QDateTime &to);
    Q_INVOKABLE void fetchLimitKeywordEvents(int limit, int offset, const QString &keyword,
                                             const QSet<Enums::PackageType> &filteredPackages, int cooperativeId);
    Q_INVOKABLE void fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction);

signals:
    void collectingDataChanged(bool processing) const;

    void addActionRequest(const QString &packageId, const Enums::SupplyChainAction &action,
                          const QGeoCoordinate &coordinate, const QDateTime &timestamp,
                          const QVariantMap &properties);
    void addActionRequest(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                          const QGeoCoordinate &coordinate, const QDateTime &timestamp,
                          const QVariantMap &properties);
    void addActionRequest(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                          const QGeoCoordinate &coordinate, const QDateTime &timestamp,
                          const QVariantMap &properties);

    void eventsInfoNeeded(const QDateTime &from, const QDateTime &to) const;
    void eventsInfoNeeded(int limit, int offset,
                          const QDateTime &from, const QDateTime &to) const;
    void eventsInfoNeeded(int limit, int offset, const QString &keyword,
                          const QSet<Enums::PackageType> &filteredPackages,
                          int cooperativeId) const;
    void lastActionEventsInfoNeeded(const Enums::SupplyChainAction &lastAction) const;
    void eventsNeeded(const QStringList &ids) const;

    void packageData(const PackageData &packageData) const;
    void eventInserted(const Gaia::ModelEntry &entryData) const;
    void relationInserted(const Gaia::ModelEntry &entryData) const;

    void localActionAdded(const QString &packageId,
                          const Enums::SupplyChainAction &action) const;
    void localActionDuplicated(const QString &packageId,
                               const Enums::SupplyChainAction &action) const;
    void localActionDataError(const QString &packageId,
                              const Enums::SupplyChainAction &action) const;

public slots:
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);
    void onActionAdded(const QString &packageId, const QByteArray &,
                       const Enums::SupplyChainAction &action);
    void onActionAddError(const QString &packageId, const QByteArray &,
                          const Enums::SupplyChainAction &action,
                          const QNetworkReply::NetworkError &error);

    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onEntitiesInfoLoaded(const QJsonArray &entitiesInfo);
    void onEntitiesLoaded(const QJsonArray &entities);

private:
    DataRequestsManager m_requestsHandler;
    DataModelsManager m_modelsHandler;
    DataLocalManager m_localHandler;
    DataViewManager m_viewHandler;

    void setupHandlersConnections();
};
