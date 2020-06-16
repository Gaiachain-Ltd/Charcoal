#pragma once

#include "common/enums.h"

#include <QObject>

class QGeoCoordinate;

class ActionController : public QObject
{
    Q_OBJECT

public:
    explicit ActionController(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);

    Q_INVOKABLE QString generatePlotId(const QString &userId,
                                       const QString &parcelCode,
                                       const QDate &date) const;
    Q_INVOKABLE QString generateHarvestId(const QString &plotId,
                                          const QString &userId) const;
    Q_INVOKABLE QString generateTransportId(const QString &harvestId,
                                            const QString &licensePlate,
                                            const int transportNumber,
                                            const QDate &date) const;

    Q_INVOKABLE QString getPlotId(const QString &id) const;
    Q_INVOKABLE QString getTransportIdFromBags(const QVariantList &scannedQrs) const;
    Q_INVOKABLE int nextTransportNumber(const QString &harvestId) const;
    Q_INVOKABLE int bagCountInTransport(const QString &transportId) const;
    Q_INVOKABLE QString plateNumberInTransport(const QString &transportId) const;
    Q_INVOKABLE QString nextOvenNumber(const QString &plotId) const;

    Q_INVOKABLE void registerLoggingBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &parcel,
        const QString &village,
        const QString &treeSpecies
        ) const;

    Q_INVOKABLE void registerLoggingEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &plotId,
        const int numberOfTrees
        ) const;

    Q_INVOKABLE void registerCarbonizationBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &plotId,
        const QString &ovenId,
        const QString &ovenType,
        const QVariantList &ovenDimensions
        ) const;

    Q_INVOKABLE void registerCarbonizationEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &harvestId,
        const QString &plotId,
        const QString &ovenIds
        ) const;

    Q_INVOKABLE void registerTransportAndLoading(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
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
        const QDateTime &beginningDate
        ) const;

private:
    const QString sep = "/";
    const QString dateFormat = "dd-MM-yyyy";

    QString m_dbConnName;

    QString findEntityId(const QString &name) const;
    QString findEntityTypeId(const Enums::PackageType type) const;
    QString findEventTypeId(const Enums::SupplyChainAction action) const;
    QString findTreeSpeciesId(const QString &species) const;
    QString findOvenTypeId(const QString &ovenType) const;

    QString actionAbbreviation(const Enums::SupplyChainAction action) const;

    QString propertiesToString(const QVariantMap &properties) const;
};

