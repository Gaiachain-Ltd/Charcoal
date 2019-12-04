#include "datarequestsmanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../helpers/requestshelper.h"
#include "../common/tags.h"
#include "../common/dataglobals.h"

DataRequestsManager::DataRequestsManager(QObject *parent)
    : AbstractManager(parent)
{}

void DataRequestsManager::setupQmlContext(QQmlApplicationEngine &)
{}

void DataRequestsManager::processAdditionalData(const QJsonObject &additionalData)
{
    ProcessCounter p(this);

    auto additionalDataModels = QMap<Enums::AdditionalDataType, Gaia::ModelData>{};

    // producers
    auto modelData = Gaia::ModelData{};
    auto array = additionalData.value(Tags::producers).toArray();
    std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataRequestsManager::processProducer, this, std::placeholders::_1));
    additionalDataModels.insert(Enums::AdditionalDataType::ProducersData, modelData);

    // other
    static const auto AdditionalDataTags = std::array<std::tuple<Enums::AdditionalDataType, QLatin1String>, 3> {
        std::make_tuple(Enums::AdditionalDataType::BuyersData, Tags::buyers),
        std::make_tuple(Enums::AdditionalDataType::TransportersData, Tags::transporters),
        std::make_tuple(Enums::AdditionalDataType::DestinationsData, Tags::destinations)
    };

    std::for_each(AdditionalDataTags.cbegin(), AdditionalDataTags.cend(),
                  [this, &additionalData, &additionalDataModels](const auto &additioanlDataTag) {
        auto [ type, tag ] = additioanlDataTag;

        auto modelData = Gaia::ModelData{};
        auto array = additionalData.value(tag).toArray();
        std::transform(array.constBegin(), array.constEnd(), std::back_inserter(modelData),
                       std::bind(&DataRequestsManager::processNameData, this, std::placeholders::_1));
        additionalDataModels.insert(type, modelData);
    });

    emit additionalDataProcessed(additionalDataModels);
}

void DataRequestsManager::processEntitiesInfo(const QJsonArray &entitiesInfo)
{
    ProcessCounter p(this);

    auto eventsInfo = Gaia::ModelData{};
    std::transform(entitiesInfo.constBegin(), entitiesInfo.constEnd(), std::back_inserter(eventsInfo),
                   std::bind(&DataRequestsManager::processEventInfo, this, std::placeholders::_1));

    emit entitiesInfoProcessed(eventsInfo);
}

void DataRequestsManager::processEntities(const QJsonArray &entities)
{
    ProcessCounter p(this);

    auto modelData = Gaia::ModelData{};
    std::transform(entities.constBegin(), entities.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataRequestsManager::processEvent, this, std::placeholders::_1));

    emit entitiesProcessed(modelData);
}

void DataRequestsManager::processRelations(const QJsonArray &relations)
{
    ProcessCounter p(this);

    auto modelData = Gaia::ModelData{};
    auto modelDataArrays = QVector<Gaia::ModelData>{};
    std::transform(relations.constBegin(), relations.constEnd(), std::back_inserter(modelDataArrays),
                   std::bind(&DataRequestsManager::processRelationsValue, this, std::placeholders::_1));
    std::for_each(modelDataArrays.begin(), modelDataArrays.end(),
                  [&modelData](const auto &item) { std::move(item.begin(), item.end(), std::back_inserter(modelData)); });

    emit relationsProcessed(modelData);
}

void DataRequestsManager::processUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    ProcessCounter p(this);

    auto modelData = Gaia::ModelData{};
    std::transform(idsArray.constBegin(), idsArray.constEnd(), std::back_inserter(modelData),
                   std::bind(&DataRequestsManager::processUnusedLotId, this, std::placeholders::_1));

    emit unusedLotIdsProcessed(modelData);
}

void DataRequestsManager::processOfflineActions(const Gaia::ModelData &offlineData)
{
    for (const auto &modelEntry : offlineData) {
        Q_ASSERT(modelEntry.size() >= 4);

        const auto packageId = modelEntry.at(0).toString();
        const auto action = modelEntry.at(1).value<Enums::SupplyChainAction>();

        if (!isOfflineActionProcessing(packageId, action)) {
            processOfflineAction(packageId, action);

            emit sendOfflineAction(packageId,
                                   action,
                                   modelEntry.at(2).toDateTime(),
                                   modelEntry.at(3).toMap());
        }
    }
}

bool DataRequestsManager::isOfflineActionProcessing(const QString &packageId, const Enums::SupplyChainAction &action) const
{
    return m_offlineActionRequestsSent.contains(packageId, action);
}

void DataRequestsManager::processOfflineAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    m_offlineActionRequestsSent.insert(packageId, action);
}

void DataRequestsManager::offlineActionAdded(const QString &packageId, const Enums::SupplyChainAction &action)
{
    emit updateLocalAction(packageId, action);

    m_offlineActionRequestsSent.remove(packageId, action);
}

void DataRequestsManager::offlineActionError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error)
{
    if (error == QNetworkReply::NetworkError::ContentConflictError) {
        emit removeLocalAction(packageId, action);
    }

    m_offlineActionRequestsSent.remove(packageId, action);
}

QJsonValue DataRequestsManager::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(dataRequests) << "Tag" << tag << "is missing in an object data!";
        return {};
    }
    return object.value(tag);
}

Gaia::ModelEntry DataRequestsManager::processProducer(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto producerId = checkAndValue(object, Tags::id).toString();
    const auto name = checkAndValue(object, Tags::name).toString();
    const auto village = checkAndValue(object, Tags::village).toString();
    const auto parcels = checkAndValue(object, Tags::parcels).toVariant().toStringList();

    return Gaia::ModelEntry {
        producerId,
                name,
                village,
                parcels
    };
}

Gaia::ModelEntry DataRequestsManager::processNameData(const QJsonValue &value)
{
    return Gaia::ModelEntry { value.toString() };
}

Gaia::ModelEntry DataRequestsManager::processEventInfo(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = checkAndValue(object, Tags::packageId).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(object, Tags::action).toString());

    return Gaia::ModelEntry {
        packageId,
                QVariant::fromValue(action),
    };
}

Gaia::ModelEntry DataRequestsManager::processEvent(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = checkAndValue(object, Tags::packageId).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(object, Tags::action).toString());

    const auto date = QDateTime::fromSecsSinceEpoch(
                checkAndValue(object, Tags::timestamp).toVariant().value<qint64>());
    const auto properties = checkAndValue(object, Tags::properties).toObject().toVariantMap();

    const auto agentObj = checkAndValue(object, Tags::agent).toObject();
    const auto cooperativeId = checkAndValue(agentObj, Tags::cooperativeId).toString();
    const auto agentRole = RequestsHelper::userTypeFromString(
                checkAndValue(agentObj, Tags::role).toString());

    return Gaia::ModelEntry {
        packageId,
                QVariant::fromValue(action),
                date,
                QVariant::fromValue(agentRole),
                cooperativeId,
                properties,
                0.0,    // location not handled yet
                0.0     // location not handled yet
    };
}

Gaia::ModelData DataRequestsManager::processRelationsValue(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = checkAndValue(object, Tags::packageId).toString();
    const auto relatedIds = checkAndValue(object, Tags::packageIds).toVariant().toStringList();

    auto modelData = Gaia::ModelData{};
    std::transform(relatedIds.begin(), relatedIds.end(), std::back_inserter(modelData),
                   [&id](const auto &relatedId) { return Gaia::ModelEntry{{ id, relatedId }, }; });
    return modelData;
}

Gaia::ModelEntry DataRequestsManager::processUnusedLotId(const QJsonValue &value)
{
    return { value.toString(), QVariant::fromValue(Enums::PackageType::Lot) };
}
