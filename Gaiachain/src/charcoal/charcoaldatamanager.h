#pragma once

#include "controllers/data/abstractdatamanager.h"
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

#include <QObject>

class CharcoalDataManager : public AbstractDataManager
{
    Q_OBJECT

    Q_PROPERTY(TreeSpeciesModel* treeSpeciesModel READ treeSpeciesModel CONSTANT)
    Q_PROPERTY(VillagesModel* villagesModel READ villagesModel CONSTANT)
    Q_PROPERTY(ParcelsModel* parcelsModel READ parcelsModel CONSTANT)
    Q_PROPERTY(DestinationsModel* destinationsModel READ destinationsModel CONSTANT)
    Q_PROPERTY(OvenTypesModel* ovenTypesModel READ ovenTypesModel CONSTANT)
    Q_PROPERTY(ActionController* actionController READ actionController CONSTANT)
    Q_PROPERTY(UnusedPlotIdsModel* unusedPlotIdsModel READ unusedPlotIdsModel CONSTANT)
    Q_PROPERTY(UnusedHarvestIdsModel* unusedHarvestIdsModel READ unusedHarvestIdsModel CONSTANT)
    Q_PROPERTY(UnusedTransportIdsModel* unusedTransportIdsModel READ unusedTransportIdsModel CONSTANT)
    Q_PROPERTY(UnusedPlotIdsForReplantationModel* unusedPlotIdsForReplantationModel READ unusedPlotIdsForReplantationModel CONSTANT)
    Q_PROPERTY(OvensModel* ovensModel READ ovensModel CONSTANT)
    Q_PROPERTY(TrackingModel* trackingModel READ trackingModel CONSTANT)

public:
    CharcoalDataManager(QObject *parent = nullptr);

    void setupDatabase(const QString &dbPath) override;

    TreeSpeciesModel* treeSpeciesModel() const;
    VillagesModel* villagesModel() const;
    ParcelsModel* parcelsModel() const;
    DestinationsModel* destinationsModel() const;
    OvenTypesModel* ovenTypesModel() const;
    ActionController* actionController() const;
    UnusedPlotIdsModel* unusedPlotIdsModel() const;
    UnusedHarvestIdsModel* unusedHarvestIdsModel() const;
    UnusedTransportIdsModel* unusedTransportIdsModel() const;
    UnusedPlotIdsForReplantationModel* unusedPlotIdsForReplantationModel() const;
    OvensModel* ovensModel() const;
    TrackingModel* trackingModel() const;

private:
    const QString m_dbConnectionName = staticMetaObject.className();

    QString m_dbPath;
    TreeSpeciesModel* m_treeSpeciesModel = nullptr;
    VillagesModel* m_villagesModel = nullptr;
    ParcelsModel* m_parcelsModel = nullptr;
    DestinationsModel* m_destinationsModel = nullptr;
    OvenTypesModel* m_ovenTypesModel = nullptr;
    ActionController* m_actionController = nullptr;
    UnusedPlotIdsModel* m_unusedPlotIdsModel = nullptr;
    UnusedHarvestIdsModel* m_unusedHarvestIdsModel = nullptr;
    UnusedTransportIdsModel* m_unusedTransportIdsModel = nullptr;
    UnusedPlotIdsForReplantationModel* m_unusedPlotIdsForReplantationModel = nullptr;
    OvensModel* m_ovensModel = nullptr;
    TrackingModel* m_trackingModel = nullptr;

    bool checkModels() const;
};
