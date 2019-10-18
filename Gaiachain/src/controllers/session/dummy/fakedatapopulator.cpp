#include "fakedatapopulator.h"

#include <QDateTime>
#include <QUuid>

#include "../../../common/dataglobals.h"
#include "../../../common/globals.h"
#include "../../../common/tags.h"
#include "../../../helpers/utility.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataFake, "data.fake")

const QString FakeDataPopulator::sc_loginPassword = QStringLiteral("test1234");

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
        { "id", "DIA_001" },
        { "name", "ACHO NEHUIE GREGOIRE" },
        { "village", "Diason" },
        { "parcels", QStringList{ "PDIA_001", "PDIA_002" } },
    },
    {
        { "id", "DIA_002" },
        { "name", "ADON  KOFFI PIERRE CLAVER " },
        { "village", "Diason" },
        { "parcels", QStringList{ "PDIA_003", "PDIA_003", "PDIA_005" } },
    },
    {
        { "id", "MEB_001" },
        { "name", "Abeu Jean Jacques" },
        { "village", "Mebifon" },
        { "parcels", QStringList{ "PMEB_001", "PMEB_003" } },
    },
    {
        { "id", "MEB_002" },
        { "name", "Abeu Yebi Joël" },
        { "village", "Mebifon" },
        { "parcels", QStringList{ "PMEB_007" } },
    },
    {
        { "id", "MEB_003" },
        { "name", "Abeu Yeffe Georges" },
        { "village", "Mebifon" },
        { "parcels", QStringList{ "PMEB_004", "PMEB_005", "PMEB_008" } },
    },
    {
        { "id", "BIE_001" },
        { "name", "N'cho N'cho Roger" },
        { "village", "Bieby" },
        { "parcels", QStringList{ "PBIE_001", "PBIE_002", "PBIE_003", "PBIE_004" } },
    }
};

const QList<QVariantHash> FakeDataPopulator::sc_cooperatives = {
    {
        { "id", "PCMB" },
        { "name", "Coop MB" },
        { "location", QVariant::fromValue(Location { 9.665145, 7.324432 }) }
    },
    {
        { "id", "PCXD" },
        { "name", "Coop XD" },
        { "location", QVariant::fromValue(Location { 9.665643, 7.324432 }) }
    }
};

const QList<QVariantHash> FakeDataPopulator::sc_buyers = {
    {
        { "name", "Nestle" },
        { "destination", "Switzerland" }
    },
    {
        { "name", "Cacao decoMoreno" },
        { "destination", "Wadowice" }
    }
};

FakeDataPopulator::FakeDataPopulator()
{}

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
    auto cooperative = randomCooperative();
    auto location = cooperative.value("location").value<Location>();

    auto obj = QVariantMap{};
    obj.insert(Tags::email, email);
    obj.insert(Tags::companyId, cooperative.value("id"));
    obj.insert(Tags::companyName, cooperative.value("name"));
    obj.insert(Tags::companyLocation, QVariantList{ location.lat, location.lon });
    obj.insert(Tags::role, DataGlobals::userTypeToString(userType));
    return obj;
}

void FakeDataPopulator::populateFakeData(const QDate &startDate)
{
    const QDate endDate = QDate::currentDate();
    for (const auto &cooperative : sc_cooperatives) {
        generateCooperativeData(cooperative, startDate, endDate);
    }
}

QVariantList FakeDataPopulator::getEventsHistory() const
{
    return Utility::toVariantList(m_eventsHistory.values());
}

QVariantList FakeDataPopulator::getEventHistory(const QString &packageId) const
{
    return Utility::toVariantList(m_eventsHistory.values(packageId));
}

bool FakeDataPopulator::canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, const Enums::UserType &user) const
{
    Q_UNUSED(packageId)
    Q_UNUSED(action)
    Q_UNUSED(user)

    // TODO implement later
    return false;
}

bool FakeDataPopulator::addAction(const QString &packageId, const Enums::SupplyChainAction &action)
{
    Q_UNUSED(packageId)
    Q_UNUSED(action)

    // TODO implement later
    return false;
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

QVariantHash FakeDataPopulator::randomBuyer() const
{
    return sc_buyers.at(qrand() % sc_buyers.count());
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

std::tuple<QString, QVariantHash> FakeDataPopulator::generateHarvestAction(const QDate &harvestDate) const
{
    auto producer = randomProducer();
    auto parcel = randomParcel(producer);
    auto harvestId = generateHarvestId(parcel, harvestDate);

    auto properties = QVariantHash{ { "parcelCode", parcel } };
    properties.unite(producer);
    properties.remove("parcels");

    return std::make_tuple(harvestId, properties);
}

QVariantHash FakeDataPopulator::generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const
{
    auto dayDiff = harvestDate.daysTo(actionDate);
    auto breakingDate = qMin(harvestDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);
    auto dryingDate = qMin(breakingDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);

    return {
        { "breaking_date", breakingDate },
        { "drying_date", dryingDate },
        { "estimatedVolume", qrand() % 100 + 50 }
    };
}

QVariantHash FakeDataPopulator::generateReceptionProperties(const QDate &processingDate, const QDate &actionDate) const
{
    auto dayDiff = processingDate.daysTo(actionDate);
    auto departureDate = qMax(processingDate, actionDate.addDays(-((qrand() % dayDiff) + sc_minDayShift)));
    auto buyer = randomBuyer().value("name").toString();

    return {
        { "departure_date", departureDate },
        { "departure_place", {} },
        { "buyer", buyer },
    };
}

QVariantHash FakeDataPopulator::generateBaggingProperties(const QStringList &harvestIds) const
{
    auto weights = QVariantMap{};
    for (const auto &harvestId : harvestIds) {
        weights.insert(harvestId, qrand() % 100 + 50);
    }

    return {
        { "scanned_code", QUuid::createUuid() },
        { "weights", weights }
    };
}

QVariantHash FakeDataPopulator::generateLotCreationProperties() const
{
    return {
        { "scanned_code", QUuid::createUuid() }
    };
}

QVariantHash FakeDataPopulator::generateWarehouseTransportProperties() const
{
    auto destination = randomBuyer().value("destination").toString();

    return {
        { "transporter", QStringLiteral("transporter-%1").arg((qrand() % 10) + 1) },
        { "destination", destination },
    };
}

QVariantHash FakeDataPopulator::generateExportReceptionProperties() const
{
    return {
        { "weight", qrand() % 1000 + 2000 }
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

void FakeDataPopulator::addEvent(const QString &id, const QDate &actionDate, const Enums::SupplyChainAction &action, const QVariantHash &cooperative, const QVariantHash &properties)
{
    auto placeType = DataGlobals::placeType(action);
    auto userType = sc_placesUser.value(placeType);
    auto location = cooperative.value("location").value<Location>();
    auto agentInfo = QVariantMap({ { Tags::role, DataGlobals::userTypeToString(userType) },
                                   { Tags::companyId, cooperative.value("id") },
                                   { Tags::companyName, cooperative.value("name") },
                                   { Tags::companyLocation, QVariantList({ location.lat, location.lon }) }});

    auto timestamp = QDateTime(actionDate, randomTime());
    auto eventData = QVariantMap({ { Tags::id, id },
                                   { Tags::timestamp, timestamp.toSecsSinceEpoch() },
                                   { Tags::agent, agentInfo },
                                   { Tags::action, DataGlobals::supplyChainActionToString(action) },
                                   { Tags::properties, properties } });
    m_eventsHistory.insert(id, eventData);
}

void FakeDataPopulator::generateCooperativeData(const QVariantHash &cooperative, const QDate &startDate, const QDate &endDate)
{
    const auto harvestsDayShift = static_cast<int>(Qt::DayOfWeek::Wednesday);
    const auto actionsDayShift = static_cast<int>(Qt::DayOfWeek::Sunday);

    auto action = Enums::SupplyChainAction::Unknown;
    auto actionDate = QDate{};
    auto id = QString{};
    auto properties = QVariantHash{};
    auto idData = QPair<QString, QDate>{};

    auto harvestsData = QList<QPair<QString, QDate>>{};     // harvest ids with processing date
    auto sacsData = QList<QPair<QString, QDate>>{};         // sacs ids with bagging date
    auto lotsData = QList<QPair<QString, QDate>>{};         // lot ids with creation date

    // HARVESTS
    for (auto harvestDate = startDate.addDays(qrand() % harvestsDayShift); harvestDate <= endDate;
         harvestDate = harvestDate.addDays((qrand() % harvestsDayShift) + sc_minDayShift)) {

        action = Enums::SupplyChainAction::Harvest;
        std::tie(id, properties) = generateHarvestAction(harvestDate);
        addEvent(id, harvestDate, action, cooperative, properties);

        action = Enums::SupplyChainAction::GrainProcessing;
        actionDate = harvestDate.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            harvestsData.append({ id, QDate{} });
            continue;
        }
        addEvent(id, actionDate, action, cooperative, generateGrainProcessingProperties(harvestDate, actionDate));

        harvestsData.append({ id, actionDate });
    }

    // reception + SACS + LOT creation
    auto harvestsCount = 0;
    auto sacsCount = qrand() % 5 + 2;            // in reality much more;
    auto lastReceptionDate = QDate::fromJulianDay(0);
    auto lastBaggingDate = QDate::fromJulianDay(0);
    auto ids = QStringList{};

    auto lotId = generateLotId(cooperative.value("id").toString(), actionDate);
    while (!harvestsData.isEmpty()) {
        harvestsCount = (qrand() % 100) <= 95   // for most cases a sac contains
                ? 1                             // only one harvest
                : qrand() % 3 + 2;              // sometimes few


        lastReceptionDate = QDate::fromJulianDay(0);

        // reception
        for (auto harvestIdx = 0; (harvestIdx < harvestsCount) && !harvestsData.isEmpty(); ++harvestIdx) {
            idData = harvestsData.takeFirst();
            if (!idData.second.isValid()) {
                // processing not done
                continue;
            }

            action = Enums::SupplyChainAction::SectionReception;
            actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
            if (actionDate > endDate) {
                continue;
            }
            addEvent(idData.first, actionDate, action, cooperative, generateReceptionProperties(idData.second, actionDate));

            ids.append(idData.first);
            if (lastReceptionDate < actionDate) {
                lastReceptionDate = actionDate;
            }
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
        addEvent(id, actionDate, action, cooperative, generateBaggingProperties(ids));

        // LOT creation
        sacsData.append({ id, actionDate });
        if (lastBaggingDate < actionDate) {
            lastBaggingDate = actionDate;
        }
        if (sacsData.count() <= sacsCount) {
            action = Enums::SupplyChainAction::LotCreation;
            actionDate = lastBaggingDate.addDays((qrand() % actionsDayShift) + sc_minDayShift);
            if (actionDate <= endDate) {
                addEvent(lotId, actionDate, action, cooperative, generateLotCreationProperties());
                lotsData.append({ lotId, actionDate });
            }

            // new lot
            lastBaggingDate = QDate::fromJulianDay(0);
            lotId = generateLotId(cooperative.value("id").toString(), actionDate);
            sacsData.clear();
        }
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
        addEvent(id, actionDate, action, cooperative, generateWarehouseTransportProperties());

        // export reception
        action = Enums::SupplyChainAction::ExportReception;
        actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            continue;
        }
        addEvent(id, actionDate, action, cooperative, generateExportReceptionProperties());
    }
}
