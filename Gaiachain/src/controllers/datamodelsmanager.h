#ifndef DATAMODELSMANAGER_H
#define DATAMODELSMANAGER_H

#include "abstractmanager.h"

#include <QSqlDatabase>

#include "../common/enums.h"
#include "../common/packagedata.h"

#include "../models/existsquerymodel.h"
#include "../models/sqltablemodel.h"

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
#include "../models/packagelastactionproxymodel.h"
#include "../models/localonlyproxymodel.h"

#include "../models/packagedataproxymodel.h"
#include "../models/relationslistproxymodel.h"

class QQmlApplicationEngine;

class DataModelsManager : public AbstractManager
{
    Q_OBJECT
public:
    DataModelsManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void setupModels(QSqlDatabase db);

    void updateCooperativeId(const QString &cooperativeId);

    void addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                        const Enums::UserType &userType, const QString &cooperativeId, const QVariantMap &properties);
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
    void countEventsNeeded(int count, const QDateTime &from, const QString &keyword);
    void eventsNeeded(const QStringList &packageIds);
    void relationsNeeded(const QStringList &packageIds);

    void packageData(const PackageData &packageData) const;
    void offlineActions(const Gaia::ModelData &offlineData) const;

private:
    QSqlDatabase m_db;

    // database models
    QScopedPointer<SqlTableModel> m_eventsDatabaseModel;
    QScopedPointer<SqlTableModel> m_relationsDatabaseModel;
    QScopedPointer<SqlTableModel> m_unusedIdsDatabaseModel;

    QScopedPointer<SqlTableModel> m_producersDatabaseModel;
    QScopedPointer<SqlTableModel> m_buyersDatabaseModel;
    QScopedPointer<SqlTableModel> m_transportersDatabaseModel;
    QScopedPointer<SqlTableModel> m_destinationsDatabaseModel;

    ExistsQueryModel m_existsQueryModel;

    // source
    EventModel m_eventsSourceModel;
    RelationModel m_relationsSourceModel;

    UnusedIdsModel m_unusedLotIdsModel;

    ProducerModel m_producersModel;
    NameModel m_buyersModel;
    NameModel m_transportersModel;
    NameModel m_destinationsModel;

    // proxy models
    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    PackageLastActionProxyModel m_lastActionHarvestModel{ Enums::SupplyChainAction::Harvest };
    PackageLastActionProxyModel m_lastActionGrainProcessingModel{ Enums::SupplyChainAction::GrainProcessing };
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

    void removeExistingProducers(Gaia::ModelData &modelData);
    void removeExistingNameData(Gaia::ModelData &modelData, AbstractModel* model, const QLatin1String &tableName);
    void removeExistingEvents(Gaia::ModelData &modelData);
    void removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck);
    void removeExistingUnusedLotIds(Gaia::ModelData &modelData);
};

#endif // DATAMODELSMANAGER_H
