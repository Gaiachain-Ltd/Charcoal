#pragma once

#include "controllers/data/abstractdatamanager.h"

#include <QObject>

class CocoaDataManager : public AbstractDataManager
{
    Q_OBJECT

public:
    CocoaDataManager(QObject *parent = nullptr);

public slots:
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);
};
