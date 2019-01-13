#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"

#include "../models/commodityproxymodel.h"
#include "../models/commoditydaterangeproxymodel.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

signals:

public slots:

private:
    void setupModels();
    void populateModels(); //TO_DO temporary for feeding models with artificial data

    EventModel m_eventModel;
    ShipmentModel m_shipmentModel;

    CommodityProxyModel m_commodityProxyModel;
    CommodityDateRangeProxyModel m_calendarRangeProxyModel;
    CommodityDateRangeProxyModel m_dateEventsRangeProxyModel;
};

#endif // DATAMANAGER_H
