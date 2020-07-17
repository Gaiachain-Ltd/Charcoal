#ifndef DATAVIEWMANAGER_H
#define DATAVIEWMANAGER_H

#include "abstractdatamodelsmanager.h"

#include <QTimer>

#include "common/globals.h"
#include "common/enums.h"
#include "cocoa/common/packagedata.h"

#include "cocoa/models/producermodel.h"
#include "cocoa/models/parcelmodel.h"
#include "cocoa/models/companymodel.h"
#include "cocoa/models/namemodel.h"

#include "cocoa/models/eventmodel.h"
#include "cocoa/models/relationmodel.h"
#include "cocoa/models/unusedidsmodel.h"

#include "cocoa/models/views/parcelviewmodel.h"
#include "cocoa/models/views/transactionsviewmodel.h"
#include "cocoa/models/views/calendarviewmodel.h"
#include "cocoa/models/views/cooperativeviewmodel.h"
#include "cocoa/models/views/localviewmodel.h"
#include "cocoa/models/views/packageviewmodel.h"
#include "cocoa/models/views/packagerelationsviewmodel.h"

#include "cocoa/models/proxy/latestrangeeventsproxymodel.h"
#include "cocoa/models/proxy/packagetypeeventsproxymodel.h"
#include "cocoa/models/proxy/packagelastactionproxymodel.h"

class QQmlApplicationEngine;

class DataViewManager : public AbstractDataModelsManager
{
    Q_OBJECT
public:
    DataViewManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    void updateCooperativeId(quint32 cooperativeId);

    PackageData preparePackageData(const QString &packageId);
    Gaia::ModelData getOfflineActions() const;

public slots:
    void onModelUpdated(const AbstractDataModelsManager::ModelType &type);

signals:
    void packagesEventsNeeded(const Gaia::ModelData &modelData) const;

    void limitKeywordEventsNeeded(int count, int offset, const QString &keyword,
                                  const QSet<Enums::PackageType> &filteredPackages, int cooperativeId) const;
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

    TransactionsViewModel m_transactionsViewModel;
    LatestRangeEventsProxyModel m_latestRangeTransactionsModel;

    CalendarViewModel m_calendarMonthViewModel;
    CalendarViewModel m_calendarDateViewModel;
    LatestRangeEventsProxyModel m_latestRangeDateModel;

    LocalViewModel m_localEventsViewModel;
    CooperativeViewModel m_cooperativeEventsViewModel;

    PackageViewModel m_packageViewModel;
    PackageRelationsViewModel m_packageRelationsViewModel;

    UnusedIdsModel m_unusedLotIdsViewModel;

    // proxy models
    // TODO: change this to clever sql query too
    PackageTypeEventsProxyModel m_packagesCalendarMonthModel;

    PackageLastActionProxyModel m_lastActionHarvestModel{
#ifdef COCOA
        Enums::SupplyChainAction::Harvest
#elif CHARCOAL
        Enums::SupplyChainAction::LoggingBeginning
#endif
    };

    PackageLastActionProxyModel m_lastActionGrainProcessingModel{
#ifdef COCOA
        Enums::SupplyChainAction::GrainProcessing
#elif CHARCOAL
        Enums::SupplyChainAction::LoggingBeginning
#endif
    };

    PackageLastActionProxyModel m_lastActionSectionReceptionModel{
#ifdef COCOA
        Enums::SupplyChainAction::SectionReception
#elif CHARCOAL
        Enums::SupplyChainAction::LoggingBeginning
#endif
    };

    void setupModels() override;
    void setupUpdateConnections() override;

    void scheduleModelUpdate(SqlQueryModel *model);
    void updateModels();
};

#endif // DATAVIEWMANAGER_H
