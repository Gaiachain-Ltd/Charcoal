#ifndef DATAMODELSMANAGER_H
#define DATAMODELSMANAGER_H

#include "abstractmanager.h"

#include <QSqlDatabase>

#include "../common/enums.h"
#include "../common/packagedata.h"

#include "../models/existsquerymodel.h"
#include "../models/sqltablemodel.h"

#include "../models/producermodel.h"
#include "../models/parcelmodel.h"
#include "../models/companymodel.h"
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
#include "../models/packagelastactionproxymodel.h"
#include "../models/localonlyproxymodel.h"

#include "../models/packagedataproxymodel.h"
#include "../models/relationslistproxymodel.h"

#include "../models/views/parcelviewmodel.h"

class QQmlApplicationEngine;

class DataModelsManager : public AbstractManager
{
    Q_OBJECT
public:
    DataModelsManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupModels(QSqlDatabase db);

    void updateCooperativeId(quint32 cooperativeId);

    void addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                        int cooperativeId, const QVariantMap &properties);
    void updateLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);
    void removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);

    void getPackageData(const QString &packageId) const;
    void processAdditionalData(const QMap<Enums::AdditionalDataType, Gaia::ModelData> &modelsData);
    void processEntitiesInfo(const Gaia::ModelData &eventsInfo);
    void processEntities(const Gaia::ModelData &modelData);
    void processRelations(const Gaia::ModelData &modelData);
    void processUnusedLotIds(const Gaia::ModelData &modelData);

    void getOfflineActions() const;

signals:
    void limitEventsNeeded(int count, int offset, const QString &keyword);
    void eventsNeeded(const QStringList &packageIds);

    void packageData(const PackageData &packageData) const;

    void offlineActions(const Gaia::ModelData &offlineData) const;
    void localActionAdded(const QString &packageId, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties);
    void localActionDuplicated(const QString &packageId, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties);

private:
    QSqlDatabase m_db;

    // database models
    QScopedPointer<SqlTableModel> m_eventsDatabaseModel;
    QScopedPointer<SqlTableModel> m_relationsDatabaseModel;
    QScopedPointer<SqlTableModel> m_unusedIdsDatabaseModel;

    QScopedPointer<SqlTableModel> m_producersDatabaseModel;
    QScopedPointer<SqlTableModel> m_parcelsDatabaseModel;
    QScopedPointer<SqlTableModel> m_companiesDatabaseModel;
    QScopedPointer<SqlTableModel> m_destinationsDatabaseModel;

    ExistsQueryModel m_existsQueryModel;

    // source
    EventModel m_eventsSourceModel;
    RelationModel m_relationsSourceModel;

    UnusedIdsModel m_unusedLotIdsSourceModel;

    ProducerModel m_producersSourceModel;
    ParcelModel m_parcelsSourceModel;
    CompanyModel m_companiesSourceModel;
    NameModel m_destinationsSourceModel;

    // view models
    UnusedIdsModel m_unusedLotIdsViewModel;

    ProducerModel m_producersViewModel;
    ParcelViewModel m_parcelsViewModel;
    NameModel m_cooperativesViewModel;
    NameModel m_buyersViewModel;
    NameModel m_transportersViewModel;
    NameModel m_destinationsViewModel;

    // proxy models
    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    PackageLastActionProxyModel m_lastActionHarvestModel{ Enums::SupplyChainAction::Harvest };
    PackageLastActionProxyModel m_lastActionGrainProcessingModel{ Enums::SupplyChainAction::GrainProcessing };
    PackageLastActionProxyModel m_lastActionSectionReceptionModel{ Enums::SupplyChainAction::SectionReception };
    PackageTypeIdsProxyModel m_packageTypeCooperativeIdsModel;

    LocalOnlyProxyModel m_localOnlyEventsModel;

    // TODO: if still needed in final implementation compose as extensions instead of single models
    CooperativeEventsProxyModel m_cooperativeFilteringEventsModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxyModel m_latestDateEventsModel;

    SearchEventsProxyModel m_searchEventsModel;
    PackageTypeProxyModel m_packagesTypeSearchEventsModel;
    LatestRangeEventsProxyModel m_latestRangePackagesTypeSearchEventsModel;

    PackageDataProxyModel m_packageDataModel;
    RelationsListProxyModel m_relationsListModel;

    // models handling
    QList<QPointer<SqlQueryModel>> m_modelsToUpdate;

    void setupUpdateConnections();

    void scheduleModelUpdate(SqlQueryModel *model);
    void updateModels();

    static bool isInvalidAction(const Gaia::ModelEntry &entityEntry);

    void removeExistingById(Gaia::ModelData &modelData, const QLatin1String &tableName);
    void removeExistingEvents(Gaia::ModelData &modelData);
    void removeExistingRelations(Gaia::ModelData &modelData);
    void removeExistingUnusedLotIds(Gaia::ModelData &modelData);
};

#endif // DATAMODELSMANAGER_H
