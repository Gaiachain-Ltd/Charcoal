#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"

#include "../models/daterangeproxymodel.h"
#include "../models/shipmenteventsproxy.h"
#include "../models/latesteventsproxy.h"
#ifdef FAKE_DATA
    #include "fakedatapopulator.h"
#endif

class DataManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

public slots:
    void clearModels();
    void onEntityLoaded(const QJsonObject &entity);
    void onEntitiesLoaded(const QJsonArray &entities);

private:
    void setupModels();

    EventModel m_eventModel;
    ShipmentModel m_shipmentModel;

    DateRangeProxyModel m_calendarRangeProxyModel;
    DateRangeProxyModel m_dateEventsRangeProxyModel;
    ShipmentEventsProxy m_shipmentEventsProxyModel;
    LatestEventsProxy m_latestEventsProxyModel;
#ifdef FAKE_DATA
    FakeDataPopulator m_fakeDataPopulator;
#endif
};

#endif // DATAMANAGER_H
