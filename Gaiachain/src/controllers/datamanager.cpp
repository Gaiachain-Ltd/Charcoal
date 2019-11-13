#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDateTime>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "../common/enums.h"
#include "../common/globals.h"
#include "../common/tags.h"
#include "../common/logs.h"
#include "../helpers/utility.h"
#include "../helpers/requestshelper.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

DataManager::DataManager(QObject *parent)
    : AbstractManager(parent)
{}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);

    engine.rootContext()->setContextProperty(QStringLiteral("producersModel"), &m_producersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("buyersModel"), &m_buyersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("transportersModel"), &m_transportersModel);
    engine.rootContext()->setContextProperty(QStringLiteral("destinationsModel"), &m_destinationsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("unusedLotIdsModel"), &m_unusedLotIdsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packageTypeCooperativeIdsModel"), &m_packageTypeCooperativeIdsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("cooperativeFilteringEvents"), &m_cooperativeFilteringEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("calendarModel"), &m_calendarModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesCalendarModel"), &m_packagesCalendarModel);

    engine.rootContext()->setContextProperty(QStringLiteral("dateEventsModel"), &m_dateEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("latestDateEventsModel"), &m_latestDateEventsModel);

    engine.rootContext()->setContextProperty(QStringLiteral("latestEventsModel"), &m_latestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("searchLatestEventsModel"), &m_searchLatestEventsModel);
    engine.rootContext()->setContextProperty(QStringLiteral("packagesTypeSearchLatestEventsModel"), &m_packagesTypeSearchLatestEventsModel);
}

void DataManager::updateCooperativeId(const QString &cooperativeId)
{
    m_cooperativeEventsModel.setCooperativeId(cooperativeId);
    m_cooperativeFilteringEventsModel.setCooperativeId(cooperativeId);
}

void DataManager::onAdditionalDataLoaded(const QJsonObject &additionalData)
{
    Q_ASSERT(m_existsQueryModel &&
             m_producersSourceModel && m_buyersSourceModel &&
             m_transportersSourceModel && m_destinationsSourceModel);

    auto modelData = Gaia::ModelData{};
    auto array = QJsonArray{};

    // producers
    static const auto ConditionsFieldsProducer = QStringList{
            m_producersSourceModel->roleNames().value(ProducerModel::Columns::ProducerId) };
    m_existsQueryModel->prepareQuery(m_producersSourceModel->tableName(), ConditionsFieldsProducer);

    array = additionalData.value(Tags::producers).toArray();
    auto end = std::remove_if(array.begin(), array.end(),
                              [this](const auto &value) {
        return m_existsQueryModel->exists({ this->checkAndValue(value.toObject(), Tags::id) });
    });
    std::transform(array.constBegin(), static_cast<QJsonArray::const_iterator>(end),
                   std::back_inserter(modelData), [this](const auto &value) {
        auto object = value.toObject();
        return QVariantList{
            this->checkAndValue(object, Tags::id).toString(),
            this->checkAndValue(object, Tags::name).toString(),
            this->checkAndValue(object, Tags::village).toString(),
            this->checkAndValue(object, Tags::parcels).toVariant().toStringList()
        };
    });
    m_producersSourceModel->appendData(modelData);
    modelData.clear();

    // other

    static const auto AdditionalDataModels = QList<QPair<QLatin1String, AbstractModel*>> {
        { Tags::buyers, m_buyersSourceModel.data() },
        { Tags::transporters, m_transportersSourceModel.data() },
        { Tags::destinations, m_destinationsSourceModel.data() }
    };

    std::for_each(AdditionalDataModels.constBegin(), AdditionalDataModels.constEnd(),
                  [this, &additionalData, &array, &modelData](const auto &dataModel) {
        Q_ASSERT(dataModel.second);
        static const auto ConditionsFields = QStringList{ NameModel::columnName() };
        m_existsQueryModel->prepareQuery(dataModel.second->tableName(), ConditionsFields);

        array = additionalData.value(dataModel.first).toArray();
        auto end = std::remove_if(array.begin(), array.end(),
                                  [this](const auto &value) {
            return m_existsQueryModel->exists({ value.toVariant() });
        });
        std::transform(array.constBegin(), static_cast<QJsonArray::const_iterator>(end),
                       std::back_inserter(modelData), [this](const auto &value) {
            return QVariantList{ value.toString() };
        });
        dataModel.second->appendData(modelData);
        modelData.clear();
    });
}

void DataManager::onRelationsLoaded(const QJsonArray &relations)
{
    Q_ASSERT(m_existsQueryModel && m_relationSourceModel);

    static const auto ConditionsFields = QStringList{
            m_relationSourceModel->roleNames().value(RelationModel::Columns::PackageId),
            m_relationSourceModel->roleNames().value(RelationModel::Columns::RelatedId) };
    m_existsQueryModel->prepareQuery(m_relationSourceModel->tableName(), ConditionsFields);

    auto modelData = Gaia::ModelData{};

    std::for_each(relations.constBegin(), relations.constEnd(),
                  [this, &modelData](const QJsonValue &value) {
        auto object = value.toObject();
        auto id = checkAndValue(object, Tags::id).toString();
        auto relatedIds = checkAndValue(object, Tags::ids).toVariant().toStringList();

        auto end = std::remove_if(relatedIds.begin(), relatedIds.end(),
                                  [this, &id](const auto &relatedId) { return m_existsQueryModel->exists({ id, relatedId }); });
        std::transform(relatedIds.constBegin(), static_cast<QStringList::const_iterator>(end), std::back_inserter(modelData),
                       [&id](const QString &relatedId) { return QVariantList{ id, relatedId }; });
    });

    m_relationSourceModel->appendData(modelData);
}

void DataManager::onEntitiesLoaded(const QJsonArray &entities)
{
    Q_ASSERT(m_existsQueryModel);

    static const auto ConditionsFields = QStringList{
            m_eventsSourceModel->roleNames().value(EventModel::Columns::PackageId),
            m_eventsSourceModel->roleNames().value(EventModel::Columns::Action) };
    m_existsQueryModel->prepareQuery(m_eventsSourceModel->tableName(), ConditionsFields);

    std::for_each(entities.constBegin(), entities.constEnd(),
                  [this](const QJsonValue &value) {
        loadEntity(value.toObject());
    });
}

void DataManager::onUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    Q_ASSERT(m_existsQueryModel && m_unusedIdsSourceModel);

    static const auto ConditionsFields = QStringList{
            m_unusedIdsSourceModel->roleNames().value(UnusedIdsModel::Columns::PackageId) };
    m_existsQueryModel->prepareQuery(m_unusedIdsSourceModel->tableName(), ConditionsFields);

    auto modelData = Gaia::ModelData{};
    auto ids = idsArray.toVariantList();

    auto end = std::remove_if(ids.begin(), ids.end(),
                              [this](const auto &id) { return m_existsQueryModel->exists({ id }); });
    std::transform(ids.constBegin(), static_cast<QVariantList::const_iterator>(end), std::back_inserter(modelData),
                   [](const QVariant &id) { return QVariantList{ id, QVariant::fromValue(Enums::PackageType::Lot) }; });   // only lots handled now
    m_unusedIdsSourceModel->appendData(modelData);
}

PackageData DataManager::getPackageData(const QString &packageId) const
{
    auto packageData = PackageData{};
    packageData.id = packageId;
    packageData.relatedPackages = m_relationsListModel.relatedPackages(packageId);
    m_packageDataModel.fillPackageData(packageData);

    return packageData;
}

void DataManager::setupModels(QSqlDatabase db)
{
    m_existsQueryModel.reset(new ExistsQueryModel(db));

    m_producersSourceModel.reset(new ProducerModel(db));
    m_buyersSourceModel.reset(new NameModel(QLatin1String("Buyers"), db));
    m_transportersSourceModel.reset(new NameModel(QLatin1String("Transporters"), db));
    m_destinationsSourceModel.reset(new NameModel(QLatin1String("Destinations"), db));

    m_producersModel.setSourceModel(m_producersSourceModel.data());
    m_buyersModel.setSourceModel(m_buyersSourceModel.data());
    m_transportersModel.setSourceModel(m_transportersSourceModel.data());
    m_destinationsModel.setSourceModel(m_destinationsSourceModel.data());

    // -------------------------------------------------------------

    m_eventsSourceModel.reset(new EventModel(db));
    m_relationSourceModel.reset(new RelationModel(db));
    m_unusedIdsSourceModel.reset(new UnusedIdsModel(db));

    m_unusedLotIdsModel.setSourceModel(m_unusedIdsSourceModel.data());

    m_cooperativeEventsModel.setSourceModel(m_eventsSourceModel.data());
    m_packageTypeCooperativeIdsModel.setSourceModel(&m_cooperativeEventsModel);

    m_cooperativeFilteringEventsModel.setSourceModel(m_eventsSourceModel.data());
    m_calendarModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_packagesCalendarModel.setSourceModel(&m_calendarModel);

    m_dateEventsModel.setSourceModel(&m_calendarModel);
    m_latestDateEventsModel.setSourceModel(&m_dateEventsModel);

    m_latestEventsModel.setSourceModel(&m_cooperativeFilteringEventsModel);
    m_searchLatestEventsModel.setSourceModel(&m_latestEventsModel);
    m_packagesTypeSearchLatestEventsModel.setSourceModel(&m_searchLatestEventsModel);

    m_packageDataModel.setSourceModel(m_eventsSourceModel.data());
    m_relationsListModel.setSourceModel(m_relationSourceModel.data());
}

QJsonValue DataManager::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(dataManager) << "Tag" << tag << "is missing in an object data!";
        return {};
    }
    return object.value(tag);
}

void DataManager::loadEntity(const QJsonObject &entityObj)
{
    Q_ASSERT(m_existsQueryModel && m_eventsSourceModel);

    Gaia::ModelData eventData;

    const auto packageId = checkAndValue(entityObj, Tags::id).toString();
    const auto action = RequestsHelper::supplyChainActionFromString(
                checkAndValue(entityObj, Tags::action).toString());

    if (m_existsQueryModel->exists(
                QVariantList{ packageId, static_cast<unsigned int>(action) })) {
        return;
    }

    const auto date = QDateTime::fromSecsSinceEpoch(
                checkAndValue(entityObj, Tags::timestamp).toVariant().value<qint64>());
    const auto properties = checkAndValue(entityObj, Tags::properties).toObject().toVariantMap();

    const auto agentObj = checkAndValue(entityObj, Tags::agent).toObject();
    const auto cooperativeId = checkAndValue(agentObj, Tags::cooperativeId).toString();
    const auto agentRole = RequestsHelper::userTypeFromString(
                checkAndValue(agentObj, Tags::role).toString());

    eventData.append(QVariantList({ packageId,
                                    QVariant::fromValue(action),
                                    date,
                                    QVariant::fromValue(agentRole),
                                    cooperativeId,
                                    properties,
                                    0.0,    // location not handled yet
                                    0.0     // location not handled yet
                                  } ));

    m_eventsSourceModel->appendData(eventData);
}
