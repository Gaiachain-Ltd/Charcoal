#include "fakedatapopulator.h"

#include <QDateTime>
#include <QUuid>

#include "../../../common/dataglobals.h"
#include "../../../common/globals.h"
#include "../../../common/tags.h"
#include "../../../helpers/utility.h"
#include "../../../helpers/requestshelper.h"
#include "../../../helpers/packagedataproperties.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataFake, "data.fake")

const QString FakeDataPopulator::sc_loginPassword = QStringLiteral("milo1024");

const QHash<QString, Enums::UserType> FakeDataPopulator::sc_usersLogin = {
    { QStringLiteral("god@gaiachain.io"), Enums::UserType::SuperUser },
    { QStringLiteral("inspector@gaiachain.io"), Enums::UserType::Inspector },
    { QStringLiteral("pca@gaiachain.io"), Enums::UserType::PCA },
    { QStringLiteral("warehouseman@gaiachain.io"), Enums::UserType::Warehouseman },
    { QStringLiteral("cooperative@gaiachain.io"), Enums::UserType::CooperativeRepresentative }
};

const QHash<Enums::PlaceType, Enums::UserType> FakeDataPopulator::sc_placesUser = {
    { Enums::PlaceType::Parcele, Enums::UserType::Inspector },
    { Enums::PlaceType::Section, Enums::UserType::Warehouseman },
    { Enums::PlaceType::CooperativeHQ, Enums::UserType::CooperativeRepresentative }
};

const QList<QVariantHash> FakeDataPopulator::sc_producers = {
    {
        { Tags::id, "DIA_001" },
        { Tags::name, "ACHO NEHUIE GREGOIRE" },
        { Tags::village, "Diason" },
        { Tags::parcels, QStringList{ "PDIA_001", "PDIA_002" } },
    },
    {
        { Tags::id, "DIA_002" },
        { Tags::name, "ADON  KOFFI PIERRE CLAVER " },
        { Tags::village, "Diason" },
        { Tags::parcels, QStringList{ "PDIA_003", "PDIA_003", "PDIA_005" } },
    },
    {
        { Tags::id, "MEB_001" },
        { Tags::name, "Abeu Jean Jacques" },
        { Tags::village, "Mebifon" },
        { Tags::parcels, QStringList{ "PMEB_001", "PMEB_003" } },
    },
    {
        { Tags::id, "MEB_002" },
        { Tags::name, "Abeu Yebi Joël" },
        { Tags::village, "Mebifon" },
        { Tags::parcels, QStringList{ "PMEB_007" } },
    },
    {
        { Tags::id, "MEB_003" },
        { Tags::name, "Abeu Yeffe Georges" },
        { Tags::village, "Mebifon" },
        { Tags::parcels, QStringList{ "PMEB_004", "PMEB_005", "PMEB_008" } },
    },
    {
        { Tags::id, "BIE_001" },
        { Tags::name, "N'cho N'cho Roger" },
        { Tags::village, "Bieby" },
        { Tags::parcels, QStringList{ "PBIE_001", "PBIE_002", "PBIE_003", "PBIE_004" } },
    }
};

const QList<QVariantHash> FakeDataPopulator::sc_cooperatives = {
    {
        { "id", "PCMB" },
        { "name", "Coop MB" }
    },
    {
        { "id", "PCXD" },
        { "name", "Coop XD" }
    }
};

const QStringList FakeDataPopulator::sc_buyers = { "Nestle", "Cacao decoMoreno" };
const QStringList FakeDataPopulator::sc_transporters = { "TransportWithUs", "TranTrans", "Move4You" };
const QStringList FakeDataPopulator::sc_destinations = { "Switzerland", "Germany", "Poland" };

FakeDataPopulator::FakeDataPopulator()
{
    // use constant number to keep fake data consistent between runs
    static const uint MAGIC_NUMBER = 42;
    qsrand(MAGIC_NUMBER);
}

QStringList FakeDataPopulator::availableLogins()
{
    const static auto logins = sortedLogins();
    return logins;
}

bool FakeDataPopulator::checkLogin(const QString &email, const QString &password)
{
    return (password == sc_loginPassword) && sc_usersLogin.contains(email);
}

Enums::UserType FakeDataPopulator::userType(const QString &email)
{
    return sc_usersLogin.value(email);
}

QVariantMap FakeDataPopulator::generateUserData(const QString &email)
{
    if (!sc_usersLogin.contains(email)) {
        return {};
    }

    auto userType = sc_usersLogin.value(email);
    auto cooperative = userType == Enums::UserType::SuperUser ? cooperativeById(sc_cooperatives.first().value("id").toString())
                                                              : randomCooperative();

    auto obj = QVariantMap{};
    obj.insert(Tags::login, email);
    obj.insert(Tags::cooperativeId, cooperative.value("id"));
    obj.insert(Tags::cooperativeName, cooperative.value("name"));
    obj.insert(Tags::role, RequestsHelper::userTypeToString(userType));
    return obj;
}

void FakeDataPopulator::populateFakeData(const QDate &startDate)
{
    static const auto unusedLotIdsCount = qrand() % 2 + 3;
    auto unusedIds = QVector<QString>(unusedLotIdsCount).toList();

    const QDate endDate = QDate::currentDate().addDays(-1);
    for (const auto &cooperative : sc_cooperatives) {
        // populate events
        generateCooperativeData(cooperative, startDate, endDate);

        auto cooperativeId = cooperative.value("id").toString();
        // populate lot ids
        std::generate(unusedIds.begin(), unusedIds.end(), std::bind(&FakeDataPopulator::generateLotId, this, cooperativeId, QDate::currentDate()));
        m_cooperativeUnusedLotIds.insert(cooperativeId, unusedIds);
    }
}

QVariantMap FakeDataPopulator::getRelations(const QStringList &packagesIds) const
{
    auto filteredRelations = QMultiMap<QString, QString>{};
    std::for_each(m_packagesRelations.constKeyValueBegin(), m_packagesRelations.constKeyValueEnd(),
                  [&packagesIds, &filteredRelations](const auto &relation) {
        if (packagesIds.contains(relation.first)) {
            filteredRelations.insert(relation.first, relation.second);
        }
    });
    return Utility::toVariantsMap(filteredRelations);
}

void FakeDataPopulator::addPackageRelation(const QString &packageId, const QStringList &relatedIds)
{
    for (const auto &relatedId : relatedIds) {
        m_packagesRelations.insert(packageId, relatedId);
    }
}

QVariantList FakeDataPopulator::getEventsInfo(int count, const QDateTime &from, const QString &keyword) const
{
    auto sortedHistory = m_eventsHistory.values();
    std::sort(sortedHistory.begin(), sortedHistory.end(),
              [](const auto &left, const auto &right) {
        return left.value(Tags::timestamp) > right.value(Tags::timestamp);
    });

    auto searchedHistory = QList<QVariantMap>{};
    std::copy_if(sortedHistory.constBegin(), sortedHistory.constEnd(), std::back_inserter(searchedHistory),
                 [&keyword](const auto &event) {
        auto packageId = event.value(Tags::packageId).toString();
        return packageId.contains(keyword);
    });

    auto filteredHistory = QList<QVariantMap>{};
    std::copy_if(searchedHistory.constBegin(), searchedHistory.constEnd(), std::back_inserter(filteredHistory),
                 [&from](const auto &event) {
        auto eventDate = QDateTime::fromSecsSinceEpoch(event.value(Tags::timestamp).toLongLong());
        return (from >= eventDate);
    });

    auto eventsInfo = QVariantList{};
    auto end = filteredHistory.constBegin();
    std::advance(end, count);
    std::transform(filteredHistory.constBegin(), filteredHistory.size() > count ? end : filteredHistory.constEnd(),
                   std::back_inserter(eventsInfo), [](const auto &event) {

        auto eventDate = QDateTime::fromSecsSinceEpoch(event.value(Tags::timestamp).toLongLong());
        return QVariantMap{
            { Tags::packageId, event.value(Tags::packageId) },
            { Tags::action, event.value(Tags::action) } };
    });
    return eventsInfo;
}

QVariantList FakeDataPopulator::getEventsInfo(const QDateTime &from, const QDateTime &to, const QString &keyword) const
{
    const auto &history = m_eventsHistory.values();

    auto searchedHistory = QList<QVariantMap>{};
    std::copy_if(history.constBegin(), history.constEnd(), std::back_inserter(searchedHistory),
                 [&keyword](const auto &event) {
        auto packageId = event.value(Tags::packageId).toString();
        return packageId.contains(keyword);
    });

    auto filteredHistory = QList<QVariantMap>{};
    std::copy_if(searchedHistory.constBegin(), searchedHistory.constEnd(), std::back_inserter(filteredHistory),
                 [&from, &to](const auto &event) {
        auto eventDate = QDateTime::fromSecsSinceEpoch(event.value(Tags::timestamp).toLongLong());
        return (from <= eventDate) && (eventDate <= to);
    });

    auto eventsInfo = QVariantList{};
    std::transform(filteredHistory.constBegin(), filteredHistory.constEnd(),
                   std::back_inserter(eventsInfo), [](const auto &event) {
        return QVariantMap{
            { Tags::packageId, event.value(Tags::packageId) },
            { Tags::action, event.value(Tags::action) } };
    });
    return eventsInfo;
}

QString FakeDataPopulator::getEventId(const QByteArray &codeData) const
{
    return m_packagesCodeData.value(codeData);
}

QVariantList FakeDataPopulator::getEventHistory(const QStringList &packagesId) const
{
    auto history = QList<QVariantMap>{};
    for (const auto &packageId : packagesId) {
        history.append(m_eventsHistory.values(packageId));
    }

    return Utility::toVariantList(history);
}

QVariantList FakeDataPopulator::createdHarvestIds(const QString &cooperativeId) const
{
    auto ids = QStringList{};
    std::copy_if(m_eventsHistory.keyBegin(), m_eventsHistory.keyEnd(),
                 std::back_inserter(ids), [this, &cooperativeId](const auto &id) {
        const auto entry = m_eventsHistory.value(id);
        const auto entryCooperativeId = entry.value(Tags::agent).toMap().value(Tags::cooperativeId).toString();
        const auto entryAction = RequestsHelper::supplyChainActionFromString(entry.value(Tags::action).toString());
        auto isCooperativeHarvestAction = (entryCooperativeId == cooperativeId) && (entryAction == Enums::SupplyChainAction::Harvest);
        if (isCooperativeHarvestAction) {
            auto otherIdActions = std::find_if(m_eventsHistory.keyBegin(), m_eventsHistory.keyEnd(),
                                               [this, id](const auto &searchId) {
                if (searchId == id) {
                    const auto entry = m_eventsHistory.value(id);
                    const auto entryAction = RequestsHelper::supplyChainActionFromString(entry.value(Tags::action).toString());
                    return (entryAction != Enums::SupplyChainAction::Harvest);
                }
                return false;
            });
            return otherIdActions == m_eventsHistory.keyEnd();  // no other actions
        }
        return false;
    });

    return Utility::toVariantList<QList<QString>>(ids);
}

QVariantList FakeDataPopulator::unusedLotIds(const QString &cooperativeId) const
{
    return Utility::toVariantList<QList<QString>>(m_cooperativeUnusedLotIds.value(cooperativeId));
}

QString FakeDataPopulator::createUnusedLotId(const QString &cooperativeId)
{
    auto lotId = generateLotId(cooperativeId, QDate::currentDate());
    m_cooperativeUnusedLotIds[cooperativeId].append(lotId);

    return lotId;
}

bool FakeDataPopulator::canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, const Enums::UserType &user, const QString &cooperativeId) const
{
    if (!DataGlobals::userActions(user).contains(action)) {
        qCWarning(dataFake) << "Trying to add action for wrong user:" << action << user;
        return false;
    }

    auto actionsForPackage = m_eventsHistory.values(packageId);
    if (actionsForPackage.isEmpty()) {
        return (action == DataGlobals::availableActions().constFirst());
    }

    qSort(actionsForPackage.begin(), actionsForPackage.end(),
          [](const QVariantMap &left, const QVariantMap &right) {
        auto leftAction = RequestsHelper::supplyChainActionFromString(left.value(Tags::action).toString());
        auto rightAction = RequestsHelper::supplyChainActionFromString(right.value(Tags::action).toString());
        return leftAction < rightAction;
    });

    auto historyCooperativeId = actionsForPackage.constFirst().value(Tags::agent).toMap().value(Tags::cooperativeId).toString();
    if (historyCooperativeId != cooperativeId) {
        qCWarning(dataFake) << "Trying to add action for different cooperative:" << "history" << historyCooperativeId << "new" << cooperativeId;
        return false;
    }

    auto lastAction = RequestsHelper::supplyChainActionFromString(actionsForPackage.constLast().value(Tags::action).toString());
    return (static_cast<int>(lastAction) + 1 == static_cast<int>(action));
}

bool FakeDataPopulator::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                  const QVariantMap &properties,const QByteArray &codeData, const Enums::UserType &user, const QString &cooperativeId)
{
    auto _packageId = packageId;
    if (_packageId.isEmpty()) {
        if (!m_packagesCodeData.contains(codeData)) {
            qCWarning(dataFake) << "Trying to add action but unknown code data.";
            return false;
        }
        _packageId = m_packagesCodeData.value(codeData);
    }

    if (!canAddAction(_packageId, action, user, cooperativeId)) {
        return false;
    }

    addEvent(_packageId, action, timestamp, properties, cooperativeById(cooperativeId), codeData);
    return true;
}

bool FakeDataPopulator::canAddNewAction(const Enums::SupplyChainAction &action, const QByteArray &codeData, const Enums::UserType &user) const
{
    if (!DataGlobals::userActions(user).contains(action)) {
        qCWarning(dataFake) << "Trying to add action for wrong user:" << action << user;
        return false;
    }

    if (action != DataGlobals::packageTypeCreationAction(Enums::PackageType::Sac)) {
        // currently only create sac can be created without an id
        qCWarning(dataFake) << "Trying to add new package action with wrong action:" << action;
        return false;
    }

    return !codeData.isEmpty();
}

QString FakeDataPopulator::addNewAction(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties,
                                        const QByteArray &codeData, const Enums::UserType &user, const QString &cooperativeId)
{
    if (!canAddNewAction(action, codeData, user)) {
        return {};
    }

    auto packageId = generateSackId(cooperativeId, timestamp.date());
    addEvent(packageId, action, timestamp, properties, cooperativeById(cooperativeId), codeData);
    return packageId;
}

QVariantList FakeDataPopulator::getProducers() const
{
    static const auto list = Utility::toVariantList(sc_producers, QMetaType::QVariantMap);
    return list;
}

QVariantList FakeDataPopulator::getBuyers() const
{
    static const auto list = Utility::toVariantList<QList<QString>>(sc_buyers);
    return list;
}

QVariantList FakeDataPopulator::getTransporters() const
{
    static const auto list = Utility::toVariantList<QList<QString>>(sc_transporters);
    return list;
}

QVariantList FakeDataPopulator::getDestinations() const
{
    static const auto list = Utility::toVariantList<QList<QString>>(sc_destinations);
    return list;
}

QList<QString> FakeDataPopulator::sortedLogins()
{
    auto logins = sc_usersLogin.keys();
    qSort(logins.begin(), logins.end(),
          [](const QString &l, const QString &r) { return sc_usersLogin.value(l) < sc_usersLogin.value(r); });

    return logins;
}

QTime FakeDataPopulator::randomTime()
{
    return QTime((qrand() % 12) + 6,    // hour between 6 AM and 6 PM
                 qrand() % 60,          // random minutes
                 qrand() % 60);         // random seconds
}

QVariantHash FakeDataPopulator::randomCooperative() const
{
    return sc_cooperatives.at(qrand() % sc_cooperatives.count());
}

QVariantHash FakeDataPopulator::randomProducer() const
{
    return sc_producers.at(qrand() % sc_producers.count());
}

QString FakeDataPopulator::randomParcel(const QVariantHash &producer) const
{
    const auto &parcels = producer.value("parcels").toStringList();
    return parcels.at(qrand() % parcels.count());
}

QString FakeDataPopulator::randomBuyer() const
{
    return sc_buyers.at(qrand() % sc_buyers.count());
}

QString FakeDataPopulator::randomTransporter() const
{
    return sc_transporters.at(qrand() % sc_transporters.count());
}

QString FakeDataPopulator::randomDestination() const
{
    return sc_destinations.at(qrand() % sc_destinations.count());
}

QVariantHash FakeDataPopulator::cooperativeById(const QString &cooperativeId) const
{
    auto result = std::find_if(sc_cooperatives.constBegin(), sc_cooperatives.constEnd(),
                               [=](const QVariantHash &cooperative) { return cooperative.value("id").toString() == cooperativeId; });
    return result == sc_cooperatives.constEnd() ? QVariantHash{} : *result;
}

QString FakeDataPopulator::generateHarvestId(const QString &parceleCode, const QDate &date) const
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parceleCode);
}

QString FakeDataPopulator::generateSackId(const QString &cooperativeId, const QDate &date) const
{
    static int counter = 1;
    return date.toString(QStringLiteral("%1-yyyy-S%2")).arg(cooperativeId).arg(counter++, 3, 10, QChar('0'));
}

QString FakeDataPopulator::generateLotId(const QString &cooperativeId, const QDate &date) const
{
    static int counter = 1;
    return date.toString(QStringLiteral("%1-yyyy-L%2")).arg(cooperativeId).arg(counter++, 3, 10, QChar('0'));
}

std::tuple<QString, QVariantMap> FakeDataPopulator::generateHarvestAction(const QDate &harvestDate) const
{
    auto producer = randomProducer();
    auto parcel = randomParcel(producer);
    auto harvestId = generateHarvestId(parcel, harvestDate);

    auto properties = QVariantMap{
        { PackageDataProperties::ProducerId, producer.value(Tags::id) },
        { PackageDataProperties::ProducerName, producer.value(Tags::name) },
        { PackageDataProperties::Village, producer.value(Tags::village) },
        { PackageDataProperties::ParcelCode, parcel }
    };

    return std::make_tuple(harvestId, properties);
}

QVariantMap FakeDataPopulator::generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const
{
    auto dayDiff = harvestDate.daysTo(actionDate);
    auto breakingDate = qMin(harvestDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);
    auto dryingDate = qMin(breakingDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);

    return {
        { PackageDataProperties::BreakingDate, QDateTime(breakingDate) },
        { PackageDataProperties::DryingDate, QDateTime(dryingDate) },
        { PackageDataProperties::EstimatedVolume, qrand() % 100 + 50 }
    };
}

QVariantMap FakeDataPopulator::generateReceptionProperties(const QDate &processingDate, const QDate &actionDate) const
{
    auto dayDiff = processingDate.daysTo(actionDate);
    auto departureDate = qMax(processingDate, actionDate.addDays(-((qrand() % dayDiff) + sc_minDayShift)));

    return {
        { PackageDataProperties::DepartureDate, QDateTime(departureDate) },
        { PackageDataProperties::DeparturePlace, {} },
        { PackageDataProperties::Buyer, randomBuyer() },
    };
}

QVariantMap FakeDataPopulator::generateBaggingProperties(const QStringList &harvestIds) const
{
    auto weights = QVariantMap{};
    for (const auto &harvestId : harvestIds) {
        weights.insert(harvestId, qrand() % 100 + 50);
    }

    return {
        { PackageDataProperties::Weights, weights },
    };
}

QVariantMap FakeDataPopulator::generateLotCreationProperties() const
{
    return {};
}

QVariantMap FakeDataPopulator::generateWarehouseTransportProperties() const
{
    return {
        { PackageDataProperties::Transporter, randomTransporter() },
        { PackageDataProperties::Destination, randomDestination() },
    };
}

QVariantMap FakeDataPopulator::generateExportReceptionProperties() const
{
    return {
        { PackageDataProperties::Weight, qrand() % 1000 + 2000 }
    };
}

void FakeDataPopulator::addHarvestRelation(const QString &sacId, const QStringList &harvestIds)
{
    for (const auto &harvestId : harvestIds) {
        m_packagesRelations.insert(sacId, harvestId);
    }
}

void FakeDataPopulator::addLotRelation(const QString &sacId, const QString &lotId)
{
    m_packagesRelations.insert(lotId, sacId);
}

void FakeDataPopulator::addEvent(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                                 const QVariantMap &properties, const QVariantHash &cooperative, const QByteArray &codeData)
{
    auto placeType = DataGlobals::placeType(action);
    auto userType = sc_placesUser.value(placeType);
    auto agentInfo = QVariantMap({ { Tags::role, RequestsHelper::userTypeToString(userType) },
                                   { Tags::cooperativeId, cooperative.value("id") },
                                   { Tags::cooperativeName, cooperative.value("name") } });

    auto eventData = QVariantMap({ { Tags::packageId, packageId },
                                   { Tags::timestamp, timestamp.toSecsSinceEpoch() },
                                   { Tags::agent, agentInfo },
                                   { Tags::action, RequestsHelper::supplyChainActionToString(action) },
                                   { Tags::properties, properties } });
    m_eventsHistory.insert(packageId, eventData);

    if (!codeData.isNull()) {
        m_packagesCodeData.insert(codeData, packageId);
    }
}

void FakeDataPopulator::generateCooperativeData(const QVariantHash &cooperative, const QDate &startDate, const QDate &endDate)
{
    const auto harvestsDayShift = static_cast<int>(Qt::DayOfWeek::Wednesday);
    const auto actionsDayShift = static_cast<int>(Qt::DayOfWeek::Sunday);

    auto action = Enums::SupplyChainAction::Unknown;
    auto actionDate = QDate{};
    auto id = QString{};
    auto properties = QVariantMap{};
    auto idData = QPair<QString, QDate>{};

    auto harvestsData = QList<QPair<QString, QDate>>{};     // harvest ids with processing date
    auto sacsData = QList<QPair<QString, QDate>>{};         // sacs ids with bagging date
    auto lotsData = QList<QPair<QString, QDate>>{};         // lot ids with creation date

    // HARVESTS
    for (auto harvestDate = startDate.addDays(qrand() % harvestsDayShift); harvestDate <= endDate;
         harvestDate = harvestDate.addDays((qrand() % harvestsDayShift) + sc_minDayShift)) {

        action = Enums::SupplyChainAction::Harvest;
        std::tie(id, properties) = generateHarvestAction(harvestDate);
        if (m_eventsHistory.contains(id)) {
            break;  // other cooperative done harvest that day on that parcel
        }
        addEvent(id, action, QDateTime(harvestDate, randomTime()), properties, cooperative);

        action = Enums::SupplyChainAction::GrainProcessing;
        actionDate = harvestDate.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            harvestsData.append({ id, QDate{} });
            continue;
        }
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateGrainProcessingProperties(harvestDate, actionDate), cooperative);

        harvestsData.append({ id, actionDate });
    }

    // reception + SACS + LOT creation
    auto harvestsCount = 0;
    auto sacsCount = qrand() % 5 + 2;            // in reality much more;
    auto lastReceptionDate = QDate{};
    auto lastBaggingDate = QDate{};
    auto ids = QStringList{};

    auto lotId = generateLotId(cooperative.value("id").toString(), actionDate);
    while (!harvestsData.isEmpty()) {
        harvestsCount = (qrand() % 100) <= 95   // for most cases a sac contains
                ? 1                             // only one harvest
                : qrand() % 3 + 2;              // sometimes few


        lastReceptionDate = {};

        // reception
        for (auto harvestIdx = 0; (harvestIdx < harvestsCount) && !harvestsData.isEmpty(); ++harvestIdx) {
            idData = harvestsData.takeFirst();
            if (!idData.second.isValid()) {
                // processing not done
                lastReceptionDate = {};
                continue;
            }

            action = Enums::SupplyChainAction::SectionReception;
            actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
            if (actionDate > endDate) {
                continue;
            }
            addEvent(idData.first, action, QDateTime(actionDate, randomTime()), generateReceptionProperties(idData.second, actionDate), cooperative);

            ids.append(idData.first);
            if (!lastReceptionDate.isValid() || lastReceptionDate < actionDate) {
                lastReceptionDate = actionDate;
            }
        }

        if (!lastReceptionDate.isValid()) {
            // processing not done
            continue;
        }

        // SACS
        action = Enums::SupplyChainAction::Bagging;
        actionDate = lastReceptionDate.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            continue;
        }

        id = generateSackId(cooperative.value("id").toString(), actionDate);
        addHarvestRelation(id, ids);
        addLotRelation(id, lotId);
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateBaggingProperties(ids), cooperative, QUuid::createUuid().toByteArray());

        // LOT creation
        sacsData.append({ id, actionDate });
        if (!lastBaggingDate.isValid() || lastBaggingDate < actionDate) {
            lastBaggingDate = actionDate;
        }
        if (sacsData.count() >= sacsCount) {
            action = Enums::SupplyChainAction::LotCreation;
            actionDate = lastBaggingDate.addDays((qrand() % actionsDayShift) + sc_minDayShift);
            if (actionDate <= endDate) {
                addEvent(lotId, action, QDateTime(actionDate, randomTime()), generateLotCreationProperties(), cooperative, QUuid::createUuid().toByteArray());
                lotsData.append({ lotId, actionDate });
            }

            // new lot
            lastBaggingDate = {};
            lotId = generateLotId(cooperative.value("id").toString(), actionDate);
            sacsData.clear();
        }

        ids.clear();
    }

    // LOTS
    while (!lotsData.isEmpty()) {
        idData = lotsData.takeFirst();
        id = idData.first;

        // warehouse transport
        action = Enums::SupplyChainAction::WarehouseTransport;
        actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            continue;
        }
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateWarehouseTransportProperties(), cooperative);

        // export reception
        action = Enums::SupplyChainAction::ExportReception;
        actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            continue;
        }
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateExportReceptionProperties(), cooperative);
    }
}
