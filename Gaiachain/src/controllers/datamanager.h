#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include "../models/eventmodel.h"

#include "../models/daterangeproxymodel.h"
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
    void onEntitiesLoaded(const QJsonArray &entities);

private:
    EventModel m_eventModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_calendarPackagesTypesModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxy m_latestDateEventsModel;

    LatestEventsProxy m_latestEventsModel;

    void setupModels();

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);
    void loadEntity(const QJsonObject &entityObj);
};

#endif // DATAMANAGER_H
