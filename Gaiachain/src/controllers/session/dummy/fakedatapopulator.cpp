#include "fakedatapopulator.h"

#include <QDateTime>
#include <QUuid>

#include "../../../common/dummy/commondummydata.h"
#include "../../../common/dataglobals.h"
#include "../../../common/globals.h"
#include "../../../common/tags.h"
#include "../../../helpers/utility.h"
#include "../../../helpers/requestshelper.h"
#include "../../../helpers/packagedataproperties.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataFake, "data.fake")

const QList<QVariantHash> FakeDataPopulator::sc_producers = {
    {
        { Tags::id, 1 },
        { Tags::pid, "DIA_001" },
        { Tags::name, "ACHO NEHUIE GREGOIRE" },
        { Tags::village, QVariantHash{ { Tags::name, "Diason" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 1 },
                  { Tags::pid, "PDIA_001" },
              },
              QVariantHash{
                  { Tags::id, 3 },
                  { Tags::pid, "PDIA_002" },
              }
          } }
    },
    {
        { Tags::id, 4 },
        { Tags::pid, "DIA_002" },
        { Tags::name, "ADON  KOFFI PIERRE CLAVER " },
        { Tags::village, QVariantHash{ { Tags::name, "Diason" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 2 },
                  { Tags::pid, "PDIA_003" },
              },
              QVariantHash{
                  { Tags::id, 5 },
                  { Tags::pid, "PDIA_005" },
              },
              QVariantHash{
                  { Tags::id, 8 },
                  { Tags::pid, "PDIA_006" },
              }
          } }
    },
    {
        { Tags::id, 6 },
        { Tags::pid, "MEB_001" },
        { Tags::name, "Abeu Jean Jacques" },
        { Tags::village, QVariantHash{ { Tags::name, "Mebifon" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 4 },
                  { Tags::pid, "PMEB_001" },
              },
              QVariantHash{
                  { Tags::id, 9 },
                  { Tags::pid, "PMEB_003" },
              }
          } }
    },
    {
        { Tags::id, 2 },
        { Tags::pid, "MEB_002" },
        { Tags::name, "Abeu Yebi Joël" },
        { Tags::village, QVariantHash{ { Tags::name, "Mebifon" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 6 },
                  { Tags::pid, "PMEB_007" },
              }
          } }
    },
    {
        { Tags::id, 5 },
        { Tags::pid, "MEB_003" },
        { Tags::name, "Abeu Yeffe Georges" },
        { Tags::village, QVariantHash{ { Tags::name, "Mebifon" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 7 },
                  { Tags::pid, "PMEB_004" },
              },
              QVariantHash{
                  { Tags::id, 10 },
                  { Tags::pid, "PMEB_005" },
              },
              QVariantHash{
                  { Tags::id, 14 },
                  { Tags::pid, "PMEB_008" },
              }
          } }
    },
    {
        { Tags::id, 3 },
        { Tags::pid, "BIE_001" },
        { Tags::name, "N'cho N'cho Roger" },
        { Tags::village, QVariantHash{ { Tags::name, "Bieby" } } },
        { Tags::parcels, QVariantList{
              QVariantHash{
                  { Tags::id, 11 },
                  { Tags::pid, "PBIE_001" },
              },
              QVariantHash{
                  { Tags::id, 12 },
                  { Tags::pid, "PBIE_002" },
              },
              QVariantHash{
                  { Tags::id, 13 },
                  { Tags::pid, "PBIE_003" },
              },
              QVariantHash{
                  { Tags::id, 15 },
                  { Tags::pid, "PBIE_004" },
              }
          } }
    }
};

const QList<QVariantHash> FakeDataPopulator::sc_companies = {
    {
        { Tags::id, 1 },
        { Tags::pid, "PCMB" },
        { Tags::name, "Coop MB" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Cooperative) }
    },
    {
        { Tags::id, 2 },
        { Tags::pid, "NEST" },
        { Tags::name, "Nestle" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Buyer) }
    },
    {
        { Tags::id, 3 },
        { Tags::pid, "DECO" },
        { Tags::name, "Cacao DecoMoreno" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Buyer) }
    },
    {
        { Tags::id, 4 },
        { Tags::pid, "PCXD" },
        { Tags::name, "Coop XD" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Cooperative) }
    },
    {
        { Tags::id, 5 },
        { Tags::pid, "TRAN" },
        { Tags::name, "TransTrans" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Transporter) }
    },
    {
        { Tags::id, 6 },
        { Tags::pid, "MOVE" },
        { Tags::name, "Move4You" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Transporter) }
    },
    {
        { Tags::id, 6 },
        { Tags::pid, "TWIT" },
        { Tags::name, "TransportWithUs" },
        { Tags::type, RequestsHelper::companyTypeToString(Enums::CompanyType::Transporter) }
    }
};

const QList<QVariantHash> FakeDataPopulator::sc_destinations = {
    {
        { Tags::id, 1 },
        { Tags::name, "Switzerland" }
    },
    {
        { Tags::id, 2 },
        { Tags::name, "Germany" }
    },
    {
        { Tags::id, 3 },
        { Tags::name, "Poland" }
    }
};

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
    return (password == CommonDummyData::commonPassword()) && CommonDummyData::usersData().contains(email);
}

Enums::UserType FakeDataPopulator::userType(const QString &email)
{
    return CommonDummyData::usersData().value(email);
}

QVariantMap FakeDataPopulator::generateUserData(const QString &email)
{
    if (!CommonDummyData::usersData().contains(email)) {
        return {};
    }

    auto userType = CommonDummyData::usersData().value(email);
    auto cooperative = userType == Enums::UserType::SuperUser ? companyById(sc_companies.first().value(Tags::id).toInt())
                                                              : randomCooperative();

    auto obj = QVariantMap{};
    obj.insert(Tags::login, email);
    obj.insert(Tags::company, cooperative);
    obj.insert(Tags::role, RequestsHelper::userTypeToString(userType));
    return obj;
}

void FakeDataPopulator::populateFakeData(const QDate &startDate)
{
    static const auto unusedLotIdsCount = qrand() % 2 + 3;
    auto unusedIds = QVector<QString>(unusedLotIdsCount).toList();

    const QDate endDate = QDate::currentDate().addDays(-1);
    for (const auto &cooperative : sc_companies) {
        if (cooperative.value(Tags::type).value<Enums::CompanyType>() != Enums::CompanyType::Cooperative) {
            continue;
        }

        // populate events
        generateCooperativeData(cooperative, startDate, endDate);

        // populate lot ids
        std::generate(unusedIds.begin(), unusedIds.end(), std::bind(&FakeDataPopulator::generateLotId, this, cooperative.value(Tags::pid).toString(), QDate::currentDate()));
        m_cooperativeUnusedLotIds.insert(cooperative.value(Tags::id).toInt(), unusedIds);
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

QVariantList FakeDataPopulator::getEventsInfo(int limit, const QDateTime &from, const QString &keyword) const
{
    auto sortedHistory = m_eventsHistory.values();
    std::sort(sortedHistory.begin(), sortedHistory.end(),
              [](const auto &left, const auto &right) {
        return left.value(Tags::timestamp) > right.value(Tags::timestamp);
    });

    auto searchedHistory = QList<QVariantMap>{};
    std::copy_if(sortedHistory.constBegin(), sortedHistory.constEnd(), std::back_inserter(searchedHistory),
                 [&keyword](const auto &event) {
        auto packageId = event.value(Tags::pid).toString();
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
    std::advance(end, limit);
    std::transform(filteredHistory.constBegin(), filteredHistory.size() > limit ? end : filteredHistory.constEnd(),
                   std::back_inserter(eventsInfo), [](const auto &event) {

        auto eventDate = QDateTime::fromSecsSinceEpoch(event.value(Tags::timestamp).toLongLong());
        return QVariantMap{
            { Tags::pid, event.value(Tags::pid) },
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
        auto packageId = event.value(Tags::pid).toString();
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
            { Tags::pid, event.value(Tags::pid) },
            { Tags::action, event.value(Tags::action) } };
    });
    return eventsInfo;
}

QVariantList FakeDataPopulator::getLastActionEventsInfo(int cooperativeId, const Enums::SupplyChainAction &lastAction) const
{
    const auto &history = m_eventsHistory.values();

    auto cooperativeHistory = QList<QVariantMap>{};
    std::copy_if(history.constBegin(), history.constEnd(), std::back_inserter(cooperativeHistory),
                 [&cooperativeId](const auto &event) {
        auto eventCooperativeId = event.value(Tags::company).toMap().value(Tags::id).toInt();
        return (eventCooperativeId == cooperativeId);
    });

    auto packageType = DataGlobals::packageType(lastAction);
    auto packageTypeHistory = QList<QVariantMap>{};
    std::copy_if(history.constBegin(), history.constEnd(), std::back_inserter(cooperativeHistory),
                 [&packageType](const auto &event) {
        auto eventPackageType = DataGlobals::packageType(event.value(Tags::action).template value<Enums::SupplyChainAction>());
        return (eventPackageType == packageType);
    });

    auto idActionMap = QMap<QString, QList<Enums::SupplyChainAction>>{};
    std::for_each(packageTypeHistory.constBegin(), packageTypeHistory.constEnd(),
                  [&idActionMap](const auto &event) {
        return idActionMap[event.value(Tags::pid).toString()].append(event.value(Tags::action).template value<Enums::SupplyChainAction>());
    });

    auto validHistory = QList<QVariantMap>{};
    std::copy_if(packageTypeHistory.constBegin(), packageTypeHistory.constEnd(),
                 std::back_inserter(validHistory), [&idActionMap, &lastAction](const auto &event) {
        auto packageId = event.value(Tags::pid).toString();
        auto actions = idActionMap.value(packageId);
        return (std::find_if(actions.constBegin(), actions.constEnd(), [&lastAction](const auto &action) {
            return (action > lastAction);
        }) == actions.constEnd());
    });

    auto eventsInfo = QVariantList{};
    std::transform(validHistory.constBegin(), validHistory.constEnd(),
                   std::back_inserter(eventsInfo), [](const auto &event) {
        return QVariantMap{
            { Tags::pid, event.value(Tags::pid) },
            { Tags::action, event.value(Tags::action) } };
    });

    return eventsInfo;
}

QVariantList FakeDataPopulator::getEventsHistory(const QStringList &packagesId) const
{
    auto history = QList<QVariantMap>{};
    for (const auto &packageId : packagesId) {
        history.append(m_eventsHistory.values(packageId));
    }

    return Utility::toVariantList(history);
}

QVariantList FakeDataPopulator::unusedLotIds(int cooperativeId) const
{
    return Utility::toVariantList<QList<QString>>(m_cooperativeUnusedLotIds.value(cooperativeId));
}

QString FakeDataPopulator::createUnusedLotId(int cooperativeId)
{
    auto cooperative = companyById(cooperativeId);
    auto lotId = generateLotId(cooperative.value(Tags::pid).toString(), QDate::currentDate());
    m_cooperativeUnusedLotIds[cooperativeId].append(lotId);

    return lotId;
}

bool FakeDataPopulator::addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const Enums::UserType &user, int cooperativeId)
{
    if (DataGlobals::userActions(user).contains(action))
        return false;

    if (!canAddAction(packageId, action, cooperativeId)) {
        return false;
    }

    addEvent(packageId, action, timestamp, properties, companyById(cooperativeId), {});
    return true;
}

bool FakeDataPopulator::addAction(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const Enums::UserType &user, int cooperativeId)
{
    if (DataGlobals::userActions(user).contains(action))
        return false;

    if (!canAddAction(packageId, codeData, action)) {
        return false;
    }

    addEvent(packageId, action, timestamp, properties, companyById(cooperativeId), codeData);
    return true;
}

QString FakeDataPopulator::addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const Enums::UserType &user, int cooperativeId)
{
    if (DataGlobals::userActions(user).contains(action))
        return {};

    if (!canAddAction(codeData, action, cooperativeId)) {
        return {};
    }

    auto packageId = QString{};
    if (action == Enums::SupplyChainAction::Bagging) {
        auto cooperative = companyById(cooperativeId);
        packageId = generateSackId(cooperative.value(Tags::pid).toString(), timestamp.date());
    } else {
        packageId = getEventId(codeData);
    }

    addEvent(packageId, action, timestamp, properties, companyById(cooperativeId), codeData);

    if (action == Enums::SupplyChainAction::Bagging) {
        const auto lotId = properties.value("lot_pid").toString();
        m_packagesRelations.insert(packageId, lotId);

        const auto harvestIds = properties.value("harvest_pids").toMap().keys();
        for (const auto &harvestId : harvestIds) {
            m_packagesRelations.insert(harvestId, packageId);
        }
    }
    return packageId;
}

bool FakeDataPopulator::canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, int cooperativeId)
{
    if (action != Enums::SupplyChainAction::Harvest &&
            action != Enums::SupplyChainAction::GrainProcessing &&
            action != Enums::SupplyChainAction::SectionReception) {
        return false;
    }

    return isNextPackageAction(packageId, action, cooperativeId);
}

bool FakeDataPopulator::canAddAction(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action)
{
    if (action != Enums::SupplyChainAction::LotCreation) {
        return false;
    }

    return !m_eventsHistory.contains(packageId) && !m_packagesCodeData.contains(codeData);
}

bool FakeDataPopulator::canAddAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, int cooperativeId)
{
    if (action != Enums::SupplyChainAction::Bagging &&
            action < Enums::SupplyChainAction::WarehouseTransport) {
        return false;
    }

    if (action == Enums::SupplyChainAction::Bagging) {
        return !m_packagesCodeData.contains(codeData);
    } else {
        auto packageId = m_packagesCodeData.value(codeData);
        return !packageId.isNull() && isNextPackageAction(packageId, action, cooperativeId);
    }
}

QVariantList FakeDataPopulator::getProducers() const
{
    static const auto list = Utility::toVariantList(sc_producers, QMetaType::QVariantMap);
    return list;
}

QVariantList FakeDataPopulator::getCompanies() const
{
    static const auto list = Utility::toVariantList(sc_companies, QMetaType::QVariantMap);
    return list;
}

QVariantList FakeDataPopulator::getDestinations() const
{
    static const auto list = Utility::toVariantList(sc_destinations, QMetaType::QVariantMap);
    return list;
}

QList<QString> FakeDataPopulator::sortedLogins()
{
    auto logins = CommonDummyData::usersData().keys();
    std::sort(logins.begin(), logins.end(),
          [](const QString &l, const QString &r) {
                  return CommonDummyData::usersData().value(l)
                      < CommonDummyData::usersData().value(r);
              });

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
    auto cooperatives = QList<QVariantHash>{};
    std::copy_if(sc_companies.constBegin(), sc_companies.constEnd(), std::back_inserter(cooperatives),
                 [](const auto &company) { return company.value(Tags::type).template value<Enums::CompanyType>() == Enums::CompanyType::Cooperative; });

    return cooperatives.at(qrand() % cooperatives.count());
}

QVariantHash FakeDataPopulator::randomProducer() const
{
    return sc_producers.at(qrand() % sc_producers.count());
}

QVariantHash FakeDataPopulator::randomParcel(const QVariantHash &producer) const
{
    const auto &parcels = producer.value("parcels").toList();
    return parcels.at(qrand() % parcels.count()).toHash();
}

QVariantHash FakeDataPopulator::randomBuyer() const
{
    auto buyers = QList<QVariantHash>{};
    std::copy_if(sc_companies.constBegin(), sc_companies.constEnd(), std::back_inserter(buyers),
                 [](const auto &company) { return company.value(Tags::type).template value<Enums::CompanyType>() == Enums::CompanyType::Buyer; });

    return buyers.at(qrand() % buyers.count());
}

QVariantHash FakeDataPopulator::randomTransporter() const
{
    auto transporters = QList<QVariantHash>{};
    std::copy_if(sc_companies.constBegin(), sc_companies.constEnd(), std::back_inserter(transporters),
                 [](const auto &company) { return company.value(Tags::type).template value<Enums::CompanyType>() == Enums::CompanyType::Transporter; });

    return transporters.at(qrand() % transporters.count());
}

QVariantHash FakeDataPopulator::randomDestination() const
{
    return sc_destinations.at(qrand() % sc_destinations.count());
}

QVariantHash FakeDataPopulator::companyById(int companyId) const
{
    auto result = std::find_if(sc_companies.constBegin(), sc_companies.constEnd(),
                               [=](const QVariantHash &company) { return company.value(Tags::id).toInt() == companyId; });
    return (result == sc_companies.constEnd()) ? QVariantHash{} : *result;
}

QString FakeDataPopulator::getEventId(const QByteArray &codeData) const
{
    return m_packagesCodeData.value(codeData);
}

bool FakeDataPopulator::isNextPackageAction(const QString &packageId, const Enums::SupplyChainAction &action, int cooperativeId) const
{
    auto actionsForPackage = m_eventsHistory.values(packageId);
    if (actionsForPackage.isEmpty()) {
        return (action == DataGlobals::availableActions().constFirst());
    }

    std::sort(actionsForPackage.begin(), actionsForPackage.end(),
          [](const QVariantMap &left, const QVariantMap &right) {
        auto leftAction = RequestsHelper::supplyChainActionFromString(left.value(Tags::action).toString());
        auto rightAction = RequestsHelper::supplyChainActionFromString(right.value(Tags::action).toString());
        return leftAction < rightAction;
    });

    auto historyCooperativeId = actionsForPackage.constFirst().value(Tags::user).toMap().value(Tags::company).toMap().value(Tags::id).toInt();
    if (historyCooperativeId != cooperativeId) {
        qCWarning(dataFake) << "Trying to add action for different cooperative:" << "history" << historyCooperativeId << "new" << cooperativeId;
        return false;
    }

    auto lastAction = RequestsHelper::supplyChainActionFromString(actionsForPackage.constLast().value(Tags::action).toString());
    return (static_cast<int>(lastAction) + 1 == static_cast<int>(action));
}

QString FakeDataPopulator::generateHarvestId(const QString &parceleCode, const QDate &date) const
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parceleCode);
}

QString FakeDataPopulator::generateSackId(const QString &cooperativeCode, const QDate &date) const
{
    static int counter = 1;
    return date.toString(QStringLiteral("%1-yyyy-S%2")).arg(cooperativeCode).arg(counter++, 3, 10, QChar('0'));
}

QString FakeDataPopulator::generateLotId(const QString &cooperativeCode, const QDate &date) const
{
    static int counter = 1;
    return date.toString(QStringLiteral("%1-yyyy-L%2")).arg(cooperativeCode).arg(counter++, 3, 10, QChar('0'));
}

std::tuple<QString, QVariantMap> FakeDataPopulator::generateHarvestAction(const QDate &harvestDate) const
{
    auto producer = randomProducer();
    auto parcel = randomParcel(producer);
    auto harvestId = generateHarvestId(parcel.value(Tags::pid).toString(), harvestDate);

    auto properties = QVariantMap{
    { PackageDataProperties::ParcelId, parcel.value(Tags::id).toInt() },
    { PackageDataProperties::HarvestDate, harvestDate }
};

    return std::make_tuple(harvestId, properties);
}

QVariantMap FakeDataPopulator::generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const
{
    auto dayDiff = harvestDate.daysTo(actionDate);
    auto breakingDate = qMin(harvestDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);
    auto fermentationEndDate = qMin(breakingDate.addDays((qrand() % dayDiff) + sc_minDayShift), actionDate);

    return {
        { PackageDataProperties::BreakingDate, QDateTime(breakingDate) },
        { PackageDataProperties::FermentationEndDate, QDateTime(fermentationEndDate) },
        { PackageDataProperties::EstimatedBeansVolume, qrand() % 100 + 50 }
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

QVariantMap FakeDataPopulator::generateWarehouseTransportProperties(const QDate &transportDate) const
{
    return {
        { PackageDataProperties::TransporterId, randomTransporter() },
        { PackageDataProperties::DestinationId, randomDestination() },
        { PackageDataProperties::TransportDate, transportDate }
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
    auto agentInfo = QVariantMap({ { Tags::role, -1 },
                                   { Tags::company, QVariantMap{ { Tags::id, cooperative.value("id") },
                                                                 { Tags::name, cooperative.value("name") } } } });

    auto eventData = QVariantMap({ { Tags::pid, packageId },
                                   { Tags::timestamp, timestamp.toSecsSinceEpoch() },
                                   { Tags::user, agentInfo },
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

    auto lotId = generateLotId(cooperative.value(Tags::pid).toString(), actionDate);
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

        id = generateSackId(cooperative.value(Tags::pid).toString(), actionDate);
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
            lotId = generateLotId(cooperative.value(Tags::pid).toString(), actionDate);
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
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateWarehouseTransportProperties(actionDate), cooperative);

        // export reception
        action = Enums::SupplyChainAction::ExportReception;
        actionDate = idData.second.addDays((qrand() % actionsDayShift) + sc_minDayShift);
        if (actionDate > endDate) {
            continue;
        }
        addEvent(id, action, QDateTime(actionDate, randomTime()), generateExportReceptionProperties(), cooperative);
    }
}
