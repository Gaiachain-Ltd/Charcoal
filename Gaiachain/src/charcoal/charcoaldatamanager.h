#pragma once

#include "controllers/data/abstractdatamanager.h"
#include "charcoal/models/treespeciesmodel.h"
#include "charcoal/models/villagesmodel.h"
#include "charcoal/models/parcelsmodel.h"

#include <QObject>

class CharcoalDataManager : public AbstractDataManager
{
    Q_OBJECT

    Q_PROPERTY(TreeSpeciesModel* treeSpeciesModel READ treeSpeciesModel CONSTANT)
    Q_PROPERTY(VillagesModel* villagesModel READ villagesModel CONSTANT)
    Q_PROPERTY(ParcelsModel* parcelsModel READ parcelsModel CONSTANT)

public:
    CharcoalDataManager(QObject *parent = nullptr);

    void setupDatabase(const QString &dbPath) override;

    Q_INVOKABLE QString generatePlotId(const QString &userId,
                                       const QString &parcelCode,
                                       const QDate &date);
    Q_INVOKABLE QString generateHarvestId(const QString &plotId,
                                          const QString &userId);
    Q_INVOKABLE QString generateTransportId(const QString &harvestId,
                                            const QString &licensePlate,
                                            const int transportNumber,
                                            const QDate &date);

    TreeSpeciesModel* treeSpeciesModel() const;
    VillagesModel* villagesModel() const;
    ParcelsModel* parcelsModel() const;

private:
    const QString sep = "/";
    const QString dateFormat = "dd-MM-yyyy";
    const QString m_dbConnectionName = staticMetaObject.className();

    QString m_dbPath;
    TreeSpeciesModel* m_treeSpeciesModel = nullptr;
    VillagesModel* m_villagesModel = nullptr;
    ParcelsModel* m_parcelsModel = nullptr;
};
