#pragma once

#include "common/enums.h"

#include <QObject>
#include <QPointer>
#include <QVariant>

class QGeoCoordinate;
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

    Q_INVOKABLE QString getPlotId(const QString &packageId);

    Q_INVOKABLE QString getTransportIdFromBags(const QVariantList &scannedQrs) const;
    Q_INVOKABLE int nextTransportNumber(const QString &harvestId) const;
    Q_INVOKABLE int bagCountInTransport(const QString &transportId) const;
    Q_INVOKABLE QString plateNumberInTransport(const QString &transportId) const;
    Q_INVOKABLE int scannedBagsCount(const QString &transportId) const;
    Q_INVOKABLE int scannedBagsTotal(const QString &transportId) const;
    Q_INVOKABLE int registeredTrucksCount(const QString &transportId) const;
    Q_INVOKABLE int registeredTrucksTotal(const QString &transportId) const;

    Q_INVOKABLE QString nextOvenNumber(const QString &plotId) const;
    Q_INVOKABLE QVariantList defaultOvenDimensions(const int ovenType) const;

    Q_INVOKABLE void registerLoggingBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &parcel,
        const QString &village,
        const QString &treeSpecies
        ) const;

    Q_INVOKABLE void registerLoggingEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &plotId,
        const int numberOfTrees
        ) const;

    Q_INVOKABLE void registerCarbonizationBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &plotId,
        const QString &ovenId,
        const QString &ovenType,
        const QVariantList &ovenDimensions
        ) const;

    Q_INVOKABLE void registerCarbonizationEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &harvestId,
        const QString &plotId,
        const QVariantList &ovenIds
        ) const;

    Q_INVOKABLE void registerLoadingAndTransport(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &transportId,
        const QString &harvestId,
        const QString &plateNumber,
        const QString &destination,
        const QVariantList &scannedQrs
        ) const;

    Q_INVOKABLE void registerReception(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QDateTime &eventDate,
        const QString &userId,
        const QString &transportId,
        const QVariantList &documents,
        const QVariantList &receipts,
        const QVariantList &scannedQrs
        ) const;

    Q_INVOKABLE void finalizeSupplyChain(const QString &plotId) const;

    Q_INVOKABLE void registerReplantation(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &plotId,
        const int numberOfTrees,
        const QString &treeSpecies,
        const QDateTime &beginningDate,
        const QDateTime &endingDate
        ) const;

signals:
    void refreshLocalEvents() const;

private:
    // TODO: move to CharcoalDbHelpers!
    QString findOvenTypeId(const QString &ovenType) const;

    QString propertiesToString(const QVariantMap &properties) const;

    int scannedBagsForAction(const QString &transportId,
                             const Enums::SupplyChainAction action) const;
    const QString dateFormat = "dd-MM-yyyy";

    QString m_dbConnName;
    QPointer<PicturesManager> m_picturesManager;
};

