#pragma once

#include "controllers/data/abstractdatamanager.h"

#include <QObject>

#include "charcoal/models/treespeciesmodel.h"
#include "charcoal/models/villagesmodel.h"
#include "charcoal/models/parcelsmodel.h"
#include "charcoal/models/destinationsmodel.h"
#include "charcoal/models/oventypesmodel.h"
#include "charcoal/models/actioncontroller.h"
#include "charcoal/models/unusedplotidsmodel.h"
#include "charcoal/models/unusedharvestidsmodel.h"
#include "charcoal/models/unusedtransportidsmodel.h"
#include "charcoal/models/unusedplotidsforreplantationmodel.h"
#include "charcoal/models/ovensmodel.h"
#include "charcoal/models/trackingmodel.h"
#include "charcoal/models/minimumdatemodel.h"
#include "charcoal/models/localeventsmodel.h"

class CharcoalDataManager : public AbstractDataManager
{
    Q_OBJECT

    Q_PROPERTY(ActionController* actionController READ actionController CONSTANT)
    Q_PROPERTY(TreeSpeciesModel* treeSpeciesModel READ treeSpeciesModel CONSTANT)
    Q_PROPERTY(VillagesModel* villagesModel READ villagesModel CONSTANT)
    Q_PROPERTY(ParcelsModel* parcelsModel READ parcelsModel CONSTANT)
    Q_PROPERTY(DestinationsModel* destinationsModel READ destinationsModel CONSTANT)
    Q_PROPERTY(OvenTypesModel* ovenTypesModel READ ovenTypesModel CONSTANT)
    Q_PROPERTY(UnusedPlotIdsModel* unusedPlotIdsModel READ unusedPlotIdsModel CONSTANT)
    Q_PROPERTY(UnusedHarvestIdsModel* unusedHarvestIdsModel READ unusedHarvestIdsModel CONSTANT)
    Q_PROPERTY(UnusedTransportIdsModel* unusedTransportIdsModel READ unusedTransportIdsModel CONSTANT)
    Q_PROPERTY(UnusedPlotIdsForReplantationModel* unusedPlotIdsForReplantationModel READ unusedPlotIdsForReplantationModel CONSTANT)
    Q_PROPERTY(OvensModel* ovensModel READ ovensModel CONSTANT)
    Q_PROPERTY(TrackingModel* trackingModel READ trackingModel CONSTANT)
    Q_PROPERTY(MinimumDateModel* minimumDateModel READ minimumDateModel CONSTANT)
    Q_PROPERTY(LocalEventsModel* localEventsModel READ localEventsModel CONSTANT)

public:
    CharcoalDataManager(const QSharedPointer<RestSessionManager> &sessionManager,
                        const QSharedPointer<UserManager> &userManager,
                        QObject *parent = nullptr);

    void setupDatabase(const QString &dbPath) override;
    void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE void refreshQueuedWebRequests();

    ActionController* actionController() const;
    TreeSpeciesModel* treeSpeciesModel() const;
    VillagesModel* villagesModel() const;
    ParcelsModel* parcelsModel() const;
    DestinationsModel* destinationsModel() const;
    OvenTypesModel* ovenTypesModel() const;
    UnusedPlotIdsModel* unusedPlotIdsModel() const;
    UnusedHarvestIdsModel* unusedHarvestIdsModel() const;
    UnusedTransportIdsModel* unusedTransportIdsModel() const;
    UnusedPlotIdsForReplantationModel* unusedPlotIdsForReplantationModel() const;
    OvensModel* ovensModel() const;
    TrackingModel* trackingModel() const;
    MinimumDateModel* minimumDateModel() const;
    LocalEventsModel* localEventsModel() const;

private:
    bool checkModels() const;
    void setupModel(QueryModel *model) const;

    const QString m_dbConnectionName = staticMetaObject.className();

    QString m_dbPath;

    QSharedPointer<RestSessionManager> m_sessionManager;
    QSharedPointer<UserManager> m_userManager;

    ActionController* m_actionController = nullptr;

    TreeSpeciesModel* m_treeSpeciesModel = nullptr;
    VillagesModel* m_villagesModel = nullptr;
    ParcelsModel* m_parcelsModel = nullptr;
    DestinationsModel* m_destinationsModel = nullptr;
    OvenTypesModel* m_ovenTypesModel = nullptr;
    UnusedPlotIdsModel* m_unusedPlotIdsModel = nullptr;
    UnusedHarvestIdsModel* m_unusedHarvestIdsModel = nullptr;
    UnusedTransportIdsModel* m_unusedTransportIdsModel = nullptr;
    UnusedPlotIdsForReplantationModel* m_unusedPlotIdsForReplantationModel = nullptr;
    OvensModel* m_ovensModel = nullptr;
    TrackingModel* m_trackingModel = nullptr;
    MinimumDateModel* m_minimumDateModel = nullptr;
    LocalEventsModel* m_localEventsModel = nullptr;
};
