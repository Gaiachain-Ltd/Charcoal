#pragma once

#include "controllers/data/abstractdatamanager.h"

#include <QObject>

class CocoaDataManager : public AbstractDataManager
{
    Q_OBJECT

public:
    CocoaDataManager(QObject *parent = nullptr);

    Q_INVOKABLE QString generateHarvestId(const QDate &date,
                                          const QString &parcelCode);

public slots:
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);
};
