#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include <QStringListModel>

#include "../models/eventmodel.h"
#include "../models/daterangeproxymodel.h"
#include "../models/latesteventsproxymodel.h"
#include "../models/packagetypeeventsproxymodel.h"
#include "../models/searcheventsproxymodel.h"
#include "../models/cooperativeeventsproxymodel.h"
#include "../models/packagetypeproxymodel.h"
#include "../models/packagetypeidsproxymodel.h"
#include "../models/packagedataproxymodel.h"
#include "../common/packagedata.h"

#include "../models/producermodel.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void updateCooperativeId(const QString &cooperativeId);

    Q_INVOKABLE PackageData getPackageData(const QString &packageId) const;

public slots:
    void clearModels();
    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onRelationsLoaded(const QJsonObject &relations);
    void onEntitiesLoaded(const QJsonArray &entities);
    void onUnusedLotIdsLoaded(const QJsonArray &ids);

private:
    EventModel m_eventModel;
    PackageDataProxyModel m_packageDataModel;

    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    PackageTypeIdsProxyModel m_packageTypeCooperativeIdsModel;

    // TODO: if still needed in final implementation compose as extensions instead of single models
    CooperativeEventsProxyModel m_cooperativeFilteringEventsModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxyModel m_latestDateEventsModel;

    LatestEventsProxyModel m_latestEventsModel;
    SearchEventsProxyModel m_searchLatestEventsModel;
    PackageTypeProxyModel m_packagesTypeSearchLatestEventsModel;

    // INFO in final implementation will be a real model
    QMultiHash<QString, QString> m_relationsModel;
    QStringListModel m_unusedLotIdsModel;

    ProducerModel m_producersModel;
    // INFO in final implementation will be a real model
    QStringListModel m_buyersModel;
    QStringListModel m_transportersModel;
    QStringListModel m_destinationsModel;

    void setupModels();

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);
    void loadEntity(const QJsonObject &entityObj);
};

#endif // DATAMANAGER_H
