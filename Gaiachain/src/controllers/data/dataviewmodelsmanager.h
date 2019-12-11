#ifndef DATAVIEWMODELSMANAGER_H
#define DATAVIEWMODELSMANAGER_H

#include "abstractdatamodelsmanager.h"

#include <QSqlDatabase>
#include <QTimer>

#include "../../common/enums.h"
#include "../../common/packagedata.h"

#include "../../models/producermodel.h"
#include "../../models/parcelmodel.h"
#include "../../models/companymodel.h"
#include "../../models/namemodel.h"

#include "../../models/eventmodel.h"
#include "../../models/relationmodel.h"
#include "../../models/unusedidsmodel.h"

#include "../../models/selectedidsproxymodel.h"
#include "../../models/daterangeproxymodel.h"
#include "../../models/latestrangeeventsproxymodel.h"
#include "../../models/latesteventsproxymodel.h"
#include "../../models/packagetypeeventsproxymodel.h"
#include "../../models/searcheventsproxymodel.h"
#include "../../models/cooperativeeventsproxymodel.h"
#include "../../models/packagetypeproxymodel.h"
#include "../../models/packagetypeidsproxymodel.h"
#include "../../models/packagelastactionproxymodel.h"
#include "../../models/localonlyproxymodel.h"

#include "../../models/packagedataproxymodel.h"
#include "../../models/relationslistproxymodel.h"

#include "../../models/views/parcelviewmodel.h"

class QQmlApplicationEngine;

class DataViewModelsManager : public AbstractDataModelsManager
{
    Q_OBJECT
public:
    DataViewModelsManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void updateCooperativeId(quint32 cooperativeId);

    PackageData getPackageData(const QString &packageId) const;
    Gaia::ModelData getOfflineActions() const;

public slots:
    void onModelUpdated(const AbstractDataModelsManager::ModelType &type);

signals:
    void limitKeywordEventsNeeded(int count, int offset, const QString &keyword) const;
    void limitRangeEventsNeeded(int count, int offset, const QDateTime &from, const QDateTime &to) const;

private:
    // models handling
    QMap<ModelType, std::function<void()>> m_modelUpdateHandler;
    QList<QPointer<SqlQueryModel>> m_modelsToUpdate;
    QTimer m_updateTimer;

    // view models
    ProducerModel m_producersViewModel;
    ParcelViewModel m_parcelsViewModel;
    NameModel m_cooperativesViewModel;
    NameModel m_buyersViewModel;
    NameModel m_transportersViewModel;
    NameModel m_destinationsViewModel;

    EventModel m_eventsViewModel;
    RelationModel m_relationsViewModel;
    UnusedIdsModel m_unusedLotIdsViewModel;

    // proxy models
    // TODO change to always use view models (#82779)
    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    PackageLastActionProxyModel m_lastActionHarvestModel{ Enums::SupplyChainAction::Harvest };
    PackageLastActionProxyModel m_lastActionGrainProcessingModel{ Enums::SupplyChainAction::GrainProcessing };
    PackageLastActionProxyModel m_lastActionSectionReceptionModel{ Enums::SupplyChainAction::SectionReception };
    PackageTypeIdsProxyModel m_packageTypeCooperativeIdsModel;

    LocalOnlyProxyModel m_localOnlyEventsModel;

    CooperativeEventsProxyModel m_cooperativeFilteringEventsModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestRangeEventsProxyModel m_latestRangeDateEventsModel;

    SearchEventsProxyModel m_searchEventsModel;
    PackageTypeProxyModel m_packagesTypeSearchEventsModel;
    LatestRangeEventsProxyModel m_latestRangePackagesTypeSearchEventsModel;

    PackageDataProxyModel m_packageDataModel;
    RelationsListProxyModel m_relationsListModel;

    void setupModels() override;
    void setupUpdateConnections() override;

    void scheduleModelUpdate(SqlQueryModel *model);
    void updateModels();
};

#endif // DATAVIEWMODELSMANAGER_H
