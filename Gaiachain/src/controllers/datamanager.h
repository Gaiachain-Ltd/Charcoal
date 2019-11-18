#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include <QSqlDatabase>
#include <QStringListModel>

#include "../models/existsquerymodel.h"

#include "../models/producermodel.h"
#include "../models/namemodel.h"

#include "../models/eventmodel.h"
#include "../models/relationmodel.h"
#include "../models/unusedidsmodel.h"

#include "../models/selectedidsproxymodel.h"
#include "../models/daterangeproxymodel.h"
#include "../models/latestrangeeventsproxymodel.h"
#include "../models/latesteventsproxymodel.h"
#include "../models/packagetypeeventsproxymodel.h"
#include "../models/searcheventsproxymodel.h"
#include "../models/cooperativeeventsproxymodel.h"
#include "../models/packagetypeproxymodel.h"
#include "../models/packagetypeidsproxymodel.h"
#include "../models/createdharvestidsproxymodel.h"

#include "../common/packagedata.h"
#include "../models/packagedataproxymodel.h"
#include "../models/relationslistproxymodel.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool collectingData READ collectingData NOTIFY collectingDataChanged)

public:
    DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupModels(QSqlDatabase db);

    void updateCooperativeId(const QString &cooperativeId);

    Q_INVOKABLE PackageData getPackageData(const QString &packageId) const;

    bool collectingData() const;

    Q_INVOKABLE void fetchEventData(const QString &packageId, const Enums::PackageType &type);
    Q_INVOKABLE void fetchRangeEvents(const QDateTime &from, const QDateTime &to);
    Q_INVOKABLE void fetchCountEvents(int count, const QDateTime &from);

signals:
    void collectingDataChanged(bool collectingData) const;

    void eventsInfoNeeded(const QDateTime &from, const QDateTime &to) const;
    void eventsInfoNeeded(int count, const QDateTime &from) const;
    void eventsNeeded(const QStringList &ids) const;
    void relationsNeeded(const QStringList &ids) const;

public slots:
    void onDataRequestError();

    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onEntitiesInfoLoaded(const QJsonArray &entitiesInfo);
    void onEntitiesLoaded(const QJsonArray &entities);
    void onRelationsLoaded(const QJsonArray &relations);
    void onCreatedHarvestIdsLoaded(const QJsonArray &idsArray);
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);

private:
    QSqlDatabase m_db;
    int m_dataRequestsCount = 0;

    QScopedPointer<ExistsQueryModel> m_existsQueryModel;

    // source models
    QScopedPointer<ProducerModel> m_producersSourceModel;
    QScopedPointer<NameModel> m_buyersSourceModel;
    QScopedPointer<NameModel> m_transportersSourceModel;
    QScopedPointer<NameModel> m_destinationsSourceModel;

    QScopedPointer<EventModel> m_eventsSourceModel;
    QScopedPointer<RelationModel> m_relationsSourceModel;
    QScopedPointer<UnusedIdsModel> m_unusedIdsSourceModel;

    // proxy models
    PackageDataProxyModel m_packageDataModel;

    CreatedHarvestIdsProxyModel m_createdHarvestIdsModel;
    QIdentityProxyModel m_unusedLotIdsModel;

    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    PackageTypeIdsProxyModel m_packageTypeCooperativeIdsModel;

    // TODO: if still needed in final implementation compose as extensions instead of single models
    CooperativeEventsProxyModel m_cooperativeFilteringEventsModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxyModel m_latestDateEventsModel;

    LatestRangeEventsProxyModel m_latestRangeEventsModel;
    SearchEventsProxyModel m_searchLatestEventsModel;
    PackageTypeProxyModel m_packagesTypeSearchLatestEventsModel;

    RelationsListProxyModel m_relationsListModel;

    QIdentityProxyModel m_producersModel;
    QIdentityProxyModel m_buyersModel;
    QIdentityProxyModel m_transportersModel;
    QIdentityProxyModel m_destinationsModel;

    void dataRequestSent();
    void dataRequestProcessed();

    void fetchMissingEvents(const Gaia::ModelData &eventsInfo);

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);

    Gaia::ModelEntry processProducer(const QJsonValue &value);
    Gaia::ModelEntry processNameData(const QJsonValue &value);
    Gaia::ModelEntry processEventInfo(const QJsonValue &value);
    Gaia::ModelEntry processEvent(const QJsonValue &value);
    Gaia::ModelData processRelations(const QJsonValue &value);
    Gaia::ModelEntry processCreatedHarvestId(const QJsonValue &value);
    Gaia::ModelEntry processUnusedLotId(const QJsonValue &value);

    void removeExistingProducers(Gaia::ModelData &modelData);
    void removeExistingNameData(Gaia::ModelData &modelData, AbstractModel* model);
    void removeExistingEvents(Gaia::ModelData &modelData);
    void removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck);
    void removeExistingUnusedLotIds(Gaia::ModelData &modelData);
};

#endif // DATAMANAGER_H
