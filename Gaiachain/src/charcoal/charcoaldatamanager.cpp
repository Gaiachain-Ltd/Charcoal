#include "charcoaldatamanager.h"

CharcoalDataManager::CharcoalDataManager(QObject *parent)
    : AbstractDataManager(parent)
{
}

QString CharcoalDataManager::generatePlotId(const QString &userId,
                                            const QString &parcelCode,
                                            const QDate &date)
{
    return userId + sep + parcelCode + sep + date.toString(dateFormat);
}

QString CharcoalDataManager::generateHarvestId(const QString &plotId,
                                               const QString &userId)
{
    return plotId + sep + userId;
}

QString CharcoalDataManager::generateTransportId(const QString &harvestId,
                                                 const QString &licensePlate,
                                                 const int transportNumber,
                                                 const QDate &date)
{
    return harvestId + sep + licensePlate
        + sep + "T" + QString::number(transportNumber)
        + sep + date.toString(dateFormat);
}
