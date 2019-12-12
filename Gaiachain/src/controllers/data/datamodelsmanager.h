#ifndef DATAMODELSMANAGER_H
#define DATAMODELSMANAGER_H

#include "abstractdatamodelsmanager.h"

#include <QSqlDatabase>

#include "../../common/enums.h"

#include "../../models/existsquerymodel.h"
#include "../../models/sqltablemodel.h"

#include "../../models/producermodel.h"
#include "../../models/parcelmodel.h"
#include "../../models/companymodel.h"
#include "../../models/namemodel.h"

#include "../../models/eventmodel.h"
#include "../../models/relationmodel.h"
#include "../../models/unusedidsmodel.h"

class DataModelsManager : public AbstractDataModelsManager
{
    Q_OBJECT
public:
    DataModelsManager(QObject *parent = nullptr);

    void updateThread();

    void addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                        int cooperativeId, const QVariantMap &properties);
    void updateLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);
    void removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);

    void processPackageData(const QString &packageId, const Enums::PackageType &packageType);
    void processPackagesInfo(const Gaia::ModelData &modelData);

    void processAdditionalData(const QMap<Enums::AdditionalDataType, Gaia::ModelData> &modelsData);
    void processEntitiesInfo(const Gaia::ModelData &modelData);
    void processEntities(const Gaia::ModelData &modelData);
    void processRelations(const Gaia::ModelData &modelData);
    void processUnusedLotIds(const Gaia::ModelData &modelData);

signals:
    void modelUpdated(const AbstractDataModelsManager::ModelType &type) const;

    void eventsNeeded(const QStringList &packageIds) const;
    void eventInserted(const Gaia::ModelEntry &entryData) const;
    void relationInserted(const Gaia::ModelEntry &entryData) const;

    void localActionAdded(const QString &packageId, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties) const;
    void localActionDuplicated(const QString &packageId, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties) const;

private:
    ExistsQueryModel m_existsQueryModel;

    // source models
    ProducerModel m_producersSourceModel;
    ParcelModel m_parcelsSourceModel;
    CompanyModel m_companiesSourceModel;
    NameModel m_destinationsSourceModel;

    EventModel m_eventsSourceModel;
    RelationModel m_relationsSourceModel;
    UnusedIdsModel m_unusedLotIdsSourceModel;

    void setupModels() override;
    void setupUpdateConnections() override;

    static bool isInvalidAction(const Gaia::ModelEntry &entityEntry);
    static Gaia::ModelData allPackageEvents(const QString &packageId, const Enums::PackageType &packageType);

    void processLastActionInfo(const Gaia::ModelData &modelData);

    void removeExistingById(Gaia::ModelData &modelData, const QLatin1String &tableName);
    void removeExistingEvents(Gaia::ModelData &modelData);
    void removeExistingRelations(Gaia::ModelData &modelData);
    void removeExistingUnusedLotIds(Gaia::ModelData &modelData);
};

#endif // DATAMODELSMANAGER_H
