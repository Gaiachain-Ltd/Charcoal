#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include "../models/eventmodel.h"

#include "../models/daterangeproxymodel.h"
#include "../models/shipmenteventsproxy.h"
#include "../models/latesteventsproxy.h"
#include "../models/packagetypeeventsproxymodel.h"

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

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_calendarPackagesTypesModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxy m_latestDateEventsModel;

    ShipmentEventsProxy m_shipmentEventsModel;
    LatestEventsProxy m_latestEventsModel;

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);
};

#endif // DATAMANAGER_H
