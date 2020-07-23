#pragma once

#include "common/enums.h"

#include <QObject>
#include <QPointer>
#include <QVariant>

class QGeoCoordinate;
class QSqlQuery;
class PicturesManager;

class ActionController : public QObject
{
    Q_OBJECT

public:
    explicit ActionController(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
    void setPicturesManager(PicturesManager *manager);

    Q_INVOKABLE QString generatePlotId(const QString &userId,
                                       const QString &parcelCode,
                                       const QDate &date) const;
    Q_INVOKABLE QString generateHarvestId(const QString &plotId,
                                          const QString &userId) const;
    Q_INVOKABLE QString generateTransportId(const QString &harvestId,
                                            const QString &licensePlate,
                                            const int transportNumber,
                                            const QDate &date) const;

    Q_INVOKABLE bool plotExists(const QString &userId,
                                const QString &parcelCode,
                                const QDate &date);

    Q_INVOKABLE QString getPlotId(const QString &packageId);

    Q_INVOKABLE int getTransportIdFromBags(const QVariantList &scannedQrs) const;
    Q_INVOKABLE QString getEntityName(const int id) const;

    Q_INVOKABLE int nextTransportNumber(const int harvestId,
                                        const bool isPausedEvent) const;
    Q_INVOKABLE int bagCountInTransport(const int transportId) const;
    Q_INVOKABLE QString plateNumberInTransport(const int transportId) const;
    Q_INVOKABLE int scannedBagsCount(const int transportId) const;
    Q_INVOKABLE int scannedBagsTotal(const int transportId) const;
    Q_INVOKABLE int registeredTrucksCount(const int transportId) const;
    Q_INVOKABLE int registeredTrucksTotal(const int transportId) const;

    Q_INVOKABLE QString nextOvenNumber(int parentEntityId) const;
    Q_INVOKABLE QVariantList defaultOvenDimensions(const int ovenId) const;

    Q_INVOKABLE void registerLoggingBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &parcel,
        const int parcelId,
        const int villageId,
        const int treeSpeciesId
        ) const;

    Q_INVOKABLE void registerLoggingEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int plotId,
        const int numberOfTrees
        ) const;

    Q_INVOKABLE void registerCarbonizationBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &plotId,
        const int plotDbId,
        const QString &ovenId,
        const int ovenIdNumber,
        const QVariantList &ovenDimensions
        ) const;

    Q_INVOKABLE void registerCarbonizationEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int harvestId,
        const QVariantList &ovenIds
        ) const;

    Q_INVOKABLE void registerLoadingAndTransport(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &transportId,
        const int harvestId,
        const QString &plateNumber,
        const QString &destination,
        const int destinationId,
        const QVariantList &scannedQrs,
        const bool pauseEvent
        ) const;

    Q_INVOKABLE void registerReception(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const int transportId,
        const QVariantList &documents,
        const QVariantList &receipts,
        const QVariantList &scannedQrs
        ) const;

    Q_INVOKABLE void finalizeSupplyChain(const int transportId) const;

    Q_INVOKABLE void registerReplantation(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const int plotId,
        const int numberOfTrees,
        const int treeSpeciesId,
        const QDateTime &beginningDate,
        const QDateTime &endingDate
        ) const;

signals:
    void refreshLocalEvents() const;

    void finalizePackages(const QVector<int> &webIds) const;

private:
    int scannedBagsForAction(const int transportId,
                             const Enums::SupplyChainAction action) const;

    bool insertEntity(QSqlQuery *query, const int typeId,
                      const QString &packageId,
                      const int parentId = -1) const;
    bool insertEvent(QSqlQuery *query,
                     const int entityId,
                     const int eventTypeId,
                     const QString &userId,
                     const QDateTime &timestamp,
                     const QDateTime &eventDate,
                     const QGeoCoordinate &coordinate,
                     const QVariantMap &properties,
                     const bool pauseEvent = false) const;
    bool updateEvent(QSqlQuery *query,
                     const int eventId,
                     const int entityId,
                     const int eventTypeId,
                     const QString &userId,
                     const QDateTime &timestamp,
                     const QDateTime &eventDate,
                     const QGeoCoordinate &coordinate,
                     const QVariantMap &properties,
                     const bool pauseEvent = false) const;

    const QString dateFormat = "dd-MM-yyyy";

    QString m_connectionName;
    QPointer<PicturesManager> m_picturesManager;
};

