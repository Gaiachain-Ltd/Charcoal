#ifndef DATAREQUESTSMANAGER_H
#define DATAREQUESTSMANAGER_H

#include "../abstractmanager.h"

#include <QNetworkReply>

#include "../../common/globals.h"
#include "../../common/enums.h"

class DataRequestsManager : public AbstractManager
{
    Q_OBJECT
public:
    DataRequestsManager(QObject *parent = nullptr);

    int processingRequestsNumber() const;

    void processAdditionalData(const QJsonObject &additionalData);
    void processEntitiesInfo(const QJsonArray &entitiesInfo);
    void processEntities(const QJsonArray &entities);
    void processUnusedLotIdsLoaded(const QJsonArray &idsArray);

    void processOfflineActions(const Gaia::ModelData &offlineData);
    bool isOfflineActionProcessing(const QString &packageId, const Enums::SupplyChainAction &action) const;
    void processOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action);

    void offlineActionAdded(const QString &packageId, const Enums::SupplyChainAction &action);
    void offlineActionError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error);

signals:
    void processingRequestsNumberChanged(int processingRequestsNumber) const;

    void additionalDataProcessed(const QMap<Enums::AdditionalDataType, Gaia::ModelData> &additionalDataModels);
    void entitiesInfoProcessed(const Gaia::ModelData &modelData);
    void entitiesProcessed(const Gaia::ModelData &modelData);
    void relationsProcessed(const Gaia::ModelData &modelData);
    void unusedLotIdsProcessed(const Gaia::ModelData &modelData);

    void updateLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);
    void removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action);

    void sendOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action,
                           const QDateTime &timestamp, const QVariantMap &properties);

private:
    using EventPropertyHandler = std::function<QVariantMap(const QJsonObject &)>;
    static const QMap<Enums::SupplyChainAction, EventPropertyHandler> sc_eventPropertyHandlers;
    QMultiMap<QString, Enums::SupplyChainAction> m_offlineActionRequestsSent;

    static Gaia::ModelEntry processNameData(const QJsonValue &value);
    static Gaia::ModelEntry processProducer(const QJsonValue &value);
    static Gaia::ModelData processParcels(const QJsonValue &value);
    static Gaia::ModelEntry processCompany(const QJsonValue &value);
    static Gaia::ModelEntry processEventInfo(const QJsonValue &value);
    static Gaia::ModelEntry processEvent(const QJsonValue &value);
    static Gaia::ModelData processRelationsValue(const QJsonValue &value);
    static Gaia::ModelEntry processUnusedLotId(const QJsonValue &value);
};

#endif // DATAREQUESTSMANAGER_H
