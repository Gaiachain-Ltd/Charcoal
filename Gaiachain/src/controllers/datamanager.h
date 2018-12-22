#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"

#include "../models/commodityproxymodel.h"
#include "../models/daterangeproxymodel.h"
#include "../models/commoditydaterangeproxymodel.h"

class QQmlApplicationEngine;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);

signals:

public slots:

private:
    void setupModels();
    void populateModels(); //TO_DO temporary for feeding models with artificial data

    EventModel m_eventModel;
    ShipmentModel m_shipmentModel;

    CommodityProxyModel m_commodityProxyModel;
    DateRangeProxyModel m_dateRangeProxyModel;
    CommodityDateRangeProxyModel m_commodityDateRangeProxyModel;
};

#endif // DATAMANAGER_H
