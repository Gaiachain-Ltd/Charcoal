#include "datarequestsmanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../helpers/requestshelper.h"
#include "../common/tags.h"
#include "../common/dataglobals.h"
#include "../helpers/packagedataproperties.h"

const QMap<Enums::SupplyChainAction, DataRequestsManager::EventPropertyHandler> DataRequestsManager::sc_eventPropertyHandlers = {
    { Enums::SupplyChainAction::Unknown, &DataRequestsManager::processSimpleProperties },
    { Enums::SupplyChainAction::Harvest, &DataRequestsManager::processHarvestProperties },
    { Enums::SupplyChainAction::GrainProcessing, &DataRequestsManager::processSimpleProperties },
    { Enums::SupplyChainAction::SectionReception, &DataRequestsManager::processSimpleProperties },
    { Enums::SupplyChainAction::Bagging, &DataRequestsManager::processBaggingProperties },
    { Enums::SupplyChainAction::LotCreation, &DataRequestsManager::processLotCreationProperties },
    { Enums::SupplyChainAction::WarehouseTransport, &DataRequestsManager::processSimpleProperties },
    { Enums::SupplyChainAction::ExportReception, &DataRequestsManager::processSimpleProperties },
};

DataRequestsManager::DataRequestsManager(QObject *parent)
    : AbstractManager(parent)
{}

void DataRequestsManager::setupQmlContext(QQmlApplicationEngine &)
{}

void DataRequestsManager::processAdditionalData(const QJsonObject &additionalData)
{
    ProcessCounter p(this);

    auto additionalDataModels = QMap<Enums::AdditionalDataType, Gaia::ModelData>{};

    auto modelData = Gaia::ModelData{};
    auto array = QJsonArray{};

    // producers & parcels
    if (additionalData.contains(Tags::producers)) {
        modelData = Gaia::ModelData{};
        array = additionalData.value(Tags::producers).toArray();
        std::transform(array.constBegin(), array.constEnd(),
                       std::back_inserter(modelData), &DataRequestsManager::processProducer);
        additionalDataModels.insert(Enums::AdditionalDataType::ProducersData, modelData);

        modelData = Gaia::ModelData{};
        array = additionalData.value(Tags::producers).toArray();

        auto parcelsData = QList<Gaia::ModelData>{};
        std::transform(array.constBegin(), array.constEnd(),
                       std::back_inserter(parcelsData), &DataRequestsManager::processParcels);
        std::for_each(parcelsData.constBegin(), parcelsData.constEnd(),
                      [&modelData](const auto &entry) { modelData.append(entry); });

        additionalDataModels.insert(Enums::AdditionalDataType::ParcelsData, modelData);
    }

    // companies
    if (additionalData.contains(Tags::companies)) {
        modelData = Gaia::ModelData{};
        array = additionalData.value(Tags::companies).toArray();
        std::transform(array.constBegin(), array.constEnd(),
                       std::back_inserter(modelData), &DataRequestsManager::processCompany);
        additionalDataModels.insert(Enums::AdditionalDataType::CompaniesData, modelData);
    }

    // destinations
    if (additionalData.contains(Tags::destinations)) {
        modelData = Gaia::ModelData{};
        array = additionalData.value(Tags::destinations).toArray();
        std::transform(array.constBegin(), array.constEnd(),
                       std::back_inserter(modelData), &DataRequestsManager::processNameData);
        additionalDataModels.insert(Enums::AdditionalDataType::DestinationsData, modelData);
    }

    emit additionalDataProcessed(additionalDataModels);}

void DataRequestsManager::processEntitiesInfo(const QJsonArray &entitiesInfo)
{
    ProcessCounter p(this);

    auto eventsInfo = Gaia::ModelData{};
    std::transform(entitiesInfo.constBegin(), entitiesInfo.constEnd(),
                   std::back_inserter(eventsInfo), &DataRequestsManager::processEventInfo);

    emit entitiesInfoProcessed(eventsInfo);
}

void DataRequestsManager::processEntities(const QJsonArray &entities)
{
    ProcessCounter p(this);

    // events
    auto modelData = Gaia::ModelData{};
    std::transform(entities.constBegin(), entities.constEnd(),
                   std::back_inserter(modelData), &DataRequestsManager::processEvent);
    emit entitiesProcessed(modelData);

    // relations
    modelData = Gaia::ModelData{};
    auto modelDataArrays = QVector<Gaia::ModelData>{};
    std::transform(entities.constBegin(), entities.constEnd(),
                   std::back_inserter(modelDataArrays), &DataRequestsManager::processRelationsValue);
    std::for_each(modelDataArrays.begin(), modelDataArrays.end(),
                  [&modelData](const auto &item) { std::move(item.begin(), item.end(), std::back_inserter(modelData)); });
    emit relationsProcessed(modelData);
}

void DataRequestsManager::processUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    ProcessCounter p(this);

    auto modelData = Gaia::ModelData{};
    std::transform(idsArray.constBegin(), idsArray.constEnd(),
                   std::back_inserter(modelData), &DataRequestsManager::processUnusedLotId);

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

Gaia::ModelEntry DataRequestsManager::processNameData(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = RequestsHelper::checkAndValue(object, Tags::id).toInt();
    const auto name = RequestsHelper::checkAndValue(object, Tags::name).toString();

    return Gaia::ModelEntry { id, name };
}

Gaia::ModelEntry DataRequestsManager::processProducer(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = RequestsHelper::checkAndValue(object, Tags::id).toInt();
    const auto code = RequestsHelper::checkAndValue(object, Tags::pid).toString();
    const auto name = RequestsHelper::checkAndValue(object, Tags::name).toString();

    const auto villageObj = RequestsHelper::checkAndValue(object, Tags::village).toObject();
    const auto village = RequestsHelper::checkAndValue(villageObj, Tags::name).toString();

    return Gaia::ModelEntry {
        id,
        code,
        name,
        village
    };
}

Gaia::ModelData DataRequestsManager::processParcels(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = RequestsHelper::checkAndValue(object, Tags::id).toInt();

    auto parcelsData = Gaia::ModelData{};
    const auto parcelsArray = RequestsHelper::checkAndValue(object, Tags::parcels).toArray();
    std::transform(parcelsArray.constBegin(), parcelsArray.constEnd(), std::back_inserter(parcelsData),
                   [producerId = id](const auto &value) {
        auto object = value.toObject();

        const auto id = RequestsHelper::checkAndValue(object, Tags::id).toInt();
        const auto code = RequestsHelper::checkAndValue(object, Tags::pid).toString();

        return Gaia::ModelEntry { id, code, producerId };
    });

    return parcelsData;
}

Gaia::ModelEntry DataRequestsManager::processCompany(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto id = RequestsHelper::checkAndValue(object, Tags::id).toInt();
    const auto name = RequestsHelper::checkAndValue(object, Tags::name).toString();
    const auto code = RequestsHelper::checkAndValue(object, Tags::pid).toString();
    const auto type = RequestsHelper::companyTypeFromString(
                RequestsHelper::checkAndValue(object, Tags::type).toString());

    return Gaia::ModelEntry { id, name, code, QVariant::fromValue(type) };
}

Gaia::ModelEntry DataRequestsManager::processEventInfo(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = RequestsHelper::checkAndValue(object, Tags::pid).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                RequestsHelper::checkAndValue(object, Tags::action).toString());

    return Gaia::ModelEntry {
        packageId,
        QVariant::fromValue(action),
    };
}

Gaia::ModelEntry DataRequestsManager::processEvent(const QJsonValue &value)
{
    auto object = value.toObject();

    const auto packageId = RequestsHelper::checkAndValue(object, Tags::pid).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                RequestsHelper::checkAndValue(object, Tags::action).toString());

    const auto date = QDateTime::fromSecsSinceEpoch(
                RequestsHelper::checkAndValue(object, Tags::timestamp).toVariant().value<qint64>());
    const auto properties = processEventProperties(RequestsHelper::checkAndValue(object, Tags::properties).toObject(), action);

    const auto userObj = RequestsHelper::checkAndValue(object, Tags::user).toObject();
    const auto companyObj = RequestsHelper::checkAndValue(userObj, Tags::company).toObject();
    const auto cooperativeId = RequestsHelper::checkAndValue(companyObj, Tags::id).toInt();

    return Gaia::ModelEntry {
        packageId,
        QVariant::fromValue(action),
        date,
        cooperativeId,
        properties,
        0.0,    // location not handled yet
        0.0     // location not handled yet
    };
}

Gaia::ModelData DataRequestsManager::processRelationsValue(const QJsonValue &value)
{
    auto modelData = Gaia::ModelData{};

    auto object = value.toObject();
    const auto id = RequestsHelper::checkAndValue(object, Tags::pid).toString();

    if (object.contains(Tags::relations)) {
        const auto relatedIds = RequestsHelper::checkAndValue(object, Tags::relations).toVariant().toStringList();

        std::transform(relatedIds.begin(), relatedIds.end(), std::back_inserter(modelData),
                       [&id](const auto &relatedId) { return Gaia::ModelEntry{{ id, relatedId }, }; });
    }
    return modelData;
}

Gaia::ModelEntry DataRequestsManager::processUnusedLotId(const QJsonValue &value)
{
    auto object = value.toObject();
    const auto id = RequestsHelper::checkAndValue(object, Tags::pid).toString();

    return { id, QVariant::fromValue(Enums::PackageType::Lot) };
}

QVariantMap DataRequestsManager::processEventProperties(const QJsonObject &object, const Enums::SupplyChainAction &action)
{
    return sc_eventPropertyHandlers[action](object);
}

QVariantMap DataRequestsManager::processSimpleProperties(const QJsonObject &object)
{
    return object.toVariantMap();
}

QVariantMap DataRequestsManager::processHarvestProperties(const QJsonObject &object)
{
    return {
        { PackageDataProperties::ParcelId, object.value(Tags::parcel).toObject().value(Tags::id).toVariant() },
        { PackageDataProperties::HarvestDate, object.value(PackageDataProperties::HarvestDate).toVariant() }
    };
}

QVariantMap DataRequestsManager::processBaggingProperties(const QJsonObject &object)
{
    // TODO
    return object.toVariantMap();
}

QVariantMap DataRequestsManager::processLotCreationProperties(const QJsonObject &object)
{
    // TODO
    return object.toVariantMap();
}
