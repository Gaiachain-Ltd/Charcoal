#pragma once

#include "controllers/data/abstractdatamanager.h"

#include <QObject>

class CharcoalDataManager : public AbstractDataManager
{
    Q_OBJECT

public:
    CharcoalDataManager(QObject *parent = nullptr);

    Q_INVOKABLE QString generatePlotId(const QString &userId,
                                       const QString &parcelCode,
                                       const QDate &date);
    Q_INVOKABLE QString generateHarvestId(const QString &plotId,
                                          const QString &userId);
    Q_INVOKABLE QString generateTransportId(const QString &harvestId,
                                            const QString &licensePlate,
                                            const int transportNumber,
                                            const QDate &date);

private:
    const QString sep = "/";
    const QString dateFormat = "dd-MM-yyyy";
};
