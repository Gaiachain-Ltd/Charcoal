#pragma once

#include "controllers/data/abstractdatamanager.h"
#include "charcoal/models/treespeciesmodel.h"
#include "charcoal/models/villagesmodel.h"
#include "charcoal/models/parcelsmodel.h"
#include "charcoal/models/destinationsmodel.h"
#include "charcoal/models/oventypesmodel.h"
#include "charcoal/models/entitiesmodel.h"
#include "charcoal/models/unusedplotidsmodel.h"
#include "charcoal/models/unusedharvestidsmodel.h"
#include "charcoal/models/unusedtransportidsmodel.h"

#include <QObject>

class CharcoalDataManager : public AbstractDataManager
{
    Q_OBJECT

    Q_PROPERTY(TreeSpeciesModel* treeSpeciesModel READ treeSpeciesModel CONSTANT)
    Q_PROPERTY(VillagesModel* villagesModel READ villagesModel CONSTANT)
    Q_PROPERTY(ParcelsModel* parcelsModel READ parcelsModel CONSTANT)
    Q_PROPERTY(DestinationsModel* destinationsModel READ destinationsModel CONSTANT)
    Q_PROPERTY(OvenTypesModel* ovenTypesModel READ ovenTypesModel CONSTANT)
    Q_PROPERTY(EntitiesModel* entitiesModel READ entitiesModel CONSTANT)
    Q_PROPERTY(UnusedPlotIdsModel* unusedPlotIdsModel READ unusedPlotIdsModel CONSTANT)
    Q_PROPERTY(UnusedHarvestIdsModel* unusedHarvestIdsModel READ unusedHarvestIdsModel CONSTANT)
    Q_PROPERTY(UnusedTransportIdsModel* unusedTransportIdsModel READ unusedTransportIdsModel CONSTANT)

public:
    CharcoalDataManager(QObject *parent = nullptr);

    void setupDatabase(const QString &dbPath) override;

    TreeSpeciesModel* treeSpeciesModel() const;
    VillagesModel* villagesModel() const;
    ParcelsModel* parcelsModel() const;
    DestinationsModel* destinationsModel() const;
    OvenTypesModel* ovenTypesModel() const;
    EntitiesModel* entitiesModel() const;
    UnusedPlotIdsModel* unusedPlotIdsModel() const;
    UnusedHarvestIdsModel* unusedHarvestIdsModel() const;
    UnusedTransportIdsModel* unusedTransportIdsModel() const;

private:
    const QString m_dbConnectionName = staticMetaObject.className();

    QString m_dbPath;
    TreeSpeciesModel* m_treeSpeciesModel = nullptr;
    VillagesModel* m_villagesModel = nullptr;
    ParcelsModel* m_parcelsModel = nullptr;
    DestinationsModel* m_destinationsModel = nullptr;
    OvenTypesModel* m_ovenTypesModel = nullptr;
    EntitiesModel* m_entitiesModel = nullptr;
    UnusedPlotIdsModel* m_unusedPlotIdsModel = nullptr;
    UnusedHarvestIdsModel* m_unusedHarvestIdsModel = nullptr;
    UnusedTransportIdsModel* m_unusedTransportIdsModel = nullptr;
};
