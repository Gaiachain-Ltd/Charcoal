#pragma once

#include "common/enums.h"

#include <QSqlQueryModel>

class QGeoCoordinate;

class EntitiesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit EntitiesModel(QObject *parent = nullptr);

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

    Q_INVOKABLE QString getPlotId(const QString &harvestId) const;

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
        const QVariantMap &ovenDimensions
        ) const;

    Q_INVOKABLE void registerCarbonizationEnding(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &userId,
        const QString &harvestId,
        const QString &plotId,
        const QVariantList &ovenIds
        ) const;

private:
    const QString sep = "/";
    const QString dateFormat = "dd-MM-yyyy";

    QString m_dbConnName;

    QString findEntityId(const QString &name) const;
    QString findEntityTypeId(const Enums::PackageType type) const;
    QString findEventTypeId(const Enums::SupplyChainAction action) const;

    QString actionAbbreviation(const Enums::SupplyChainAction action) const;
};

