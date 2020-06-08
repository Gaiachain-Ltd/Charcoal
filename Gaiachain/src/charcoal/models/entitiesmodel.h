#pragma once

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

    Q_INVOKABLE void registerLoggingBeginning(
        const QGeoCoordinate &coordinate,
        const QDateTime &timestamp,
        const QString &parcel,
        const QString &malebiRepsId,
        const QString &village,
        const QString &treeSpecies
        );

private:
    const QString sep = "/";
    const QString dateFormat = "dd-MM-yyyy";

    QString m_dbConnName;
};

