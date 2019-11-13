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

#include "../models/daterangeproxymodel.h"
#include "../models/latesteventsproxymodel.h"
#include "../models/packagetypeeventsproxymodel.h"
#include "../models/searcheventsproxymodel.h"
#include "../models/cooperativeeventsproxymodel.h"
#include "../models/packagetypeproxymodel.h"
#include "../models/packagetypeidsproxymodel.h"

#include "../common/packagedata.h"
#include "../models/packagedataproxymodel.h"
#include "../models/relationslistproxymodel.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
public:
    DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupModels(QSqlDatabase db);

    void updateCooperativeId(const QString &cooperativeId);

    Q_INVOKABLE PackageData getPackageData(const QString &packageId) const;

public slots:
    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onRelationsLoaded(const QJsonArray &relations);
    void onEntitiesLoaded(const QJsonArray &entities);
    void onUnusedLotIdsLoaded(const QJsonArray &ids);

private:
    QSqlDatabase m_db;

    QScopedPointer<ExistsQueryModel> m_existsQueryModel;

    // source models
    QScopedPointer<ProducerModel> m_producersSourceModel;
    QScopedPointer<NameModel> m_buyersSourceModel;
    QScopedPointer<NameModel> m_transportersSourceModel;
    QScopedPointer<NameModel> m_destinationsSourceModel;

    QScopedPointer<EventModel> m_eventsSourceModel;
    QScopedPointer<RelationModel> m_relationSourceModel;
    QScopedPointer<UnusedIdsModel> m_unusedIdsSourceModel;

    // proxy models
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

    RelationsListProxyModel m_relationsListModel;
    QIdentityProxyModel m_unusedLotIdsModel;

    QIdentityProxyModel m_producersModel;
    QIdentityProxyModel m_buyersModel;
    QIdentityProxyModel m_transportersModel;
    QIdentityProxyModel m_destinationsModel;

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);
    void loadEntity(const QJsonObject &entityObj);
};

#endif // DATAMANAGER_H
