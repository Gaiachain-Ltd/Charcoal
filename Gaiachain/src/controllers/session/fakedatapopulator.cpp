#include "fakedatapopulator.h"

#include <QDateTime>

#include "../../helpers/utility.h"
#include "../../common/globals.h"
#include "../../common/tags.h"
#include "../../common/logs.h"

const QString FakeDataPopulator::sc_loginPassword = QStringLiteral("test1234");

const QHash<QString, Enums::UserType> FakeDataPopulator::sc_usersLogin = {
    { QStringLiteral("producer@gaiachain.io"), Enums::UserType::Producer },
    { QStringLiteral("bagging@gaiachain.io"), Enums::UserType::BaggingPerson },
    { QStringLiteral("storekeeper@gaiachain.io"), Enums::UserType::Storekeeper },
    { QStringLiteral("exporter@gaiachain.io"), Enums::UserType::Exporter }
};

const QHash<Enums::UserType, int> FakeDataPopulator::sc_companiesCount = {
    { Enums::UserType::Producer, 2 },
    { Enums::UserType::BaggingPerson, 3 },
    { Enums::UserType::Storekeeper, 2 },
    { Enums::UserType::Exporter, 3 }
};
const QHash<Enums::UserType, QStringList> FakeDataPopulator::sc_companiesName = {
    { Enums::UserType::Producer, {"VillageInc", "VillagesAndBrothers"} },
    { Enums::UserType::BaggingPerson, {"BaggingInc", "BagsAndRoses", "RisingBags"} },
    { Enums::UserType::Storekeeper, {"StoreInc", "StoringAll"} },
    { Enums::UserType::Exporter, {"ExportingInc", "ShipsInc", "XentExports"} }
};

const QHash<Enums::UserType, QList<Location>> FakeDataPopulator::sc_companiesLocation = {
    { Enums::UserType::Producer, { Location { 9.665645, 7.324432 }, Location { 9.665245, 7.224432 } } },
    { Enums::UserType::BaggingPerson, { Location { 9.665635, 7.324532 }, Location { 9.665445, 7.324432 }, Location { 9.665665, 7.355432 } } },
    { Enums::UserType::Storekeeper, { Location { 9.665245, 7.324632 }, Location { 9.665645, 7.314432 } } },
    { Enums::UserType::Exporter, { Location { 9.665145, 7.324432 }, Location { 9.665643, 7.324432 }, Location { 9.666645, 7.325432 } } }
};


const QVector<Enums::SupplyChainAction> FakeDataPopulator::sc_availableActions = {
    Enums::SupplyChainAction::Harvest,
    Enums::SupplyChainAction::Fermentation,
    Enums::SupplyChainAction::Bagging,
    Enums::SupplyChainAction::StorageArrival,
    Enums::SupplyChainAction::StorageLot,
    Enums::SupplyChainAction::Transport,
    Enums::SupplyChainAction::Reception
};

const QVector<Enums::ActionProgress> FakeDataPopulator::sc_availableActionProgresses = {
    Enums::ActionProgress::Started,
    Enums::ActionProgress::Finished
};

const QHash<Enums::SupplyChainAction, Enums::UserType> FakeDataPopulator::sc_actionsUser = {
    { Enums::SupplyChainAction::Harvest, Enums::UserType::Producer },
    { Enums::SupplyChainAction::Fermentation, Enums::UserType::Producer },
    { Enums::SupplyChainAction::Bagging, Enums::UserType::BaggingPerson },
    { Enums::SupplyChainAction::StorageArrival, Enums::UserType::Storekeeper },
    { Enums::SupplyChainAction::StorageLot, Enums::UserType::Storekeeper },
    { Enums::SupplyChainAction::Transport, Enums::UserType::Exporter },
    { Enums::SupplyChainAction::Reception, Enums::UserType::Exporter }
};

FakeDataPopulator::FakeDataPopulator()
{}

bool FakeDataPopulator::checkLogin(const QString &email, const QString &password)
{
    return (password == sc_loginPassword) && sc_usersLogin.contains(email);
}

QVariantMap FakeDataPopulator::generateUserData(const QString &email)
{
    if (!sc_usersLogin.contains(email)) {
        return {};
    }

    auto userType = sc_usersLogin.value(email);
    auto company = randomCompany(userType);

    auto obj = QVariantMap{};
    obj.insert(Tags::email, email);
    obj.insert(Tags::companyName, company.first);
    obj.insert(Tags::location, QVariantList{ company.second.lat, company.second.lon });
    obj.insert(Tags::role, Utility::instance()->userTypeToString(userType));
    return obj;
}

void FakeDataPopulator::populateFakeData(const int shipmentsCount, const QDateTime &startDate)
{
    const QDateTime endDate = QDateTime::currentDateTime();
    const auto dateDiff = startDate.daysTo(endDate);
    const auto shipmentsDayShift = qMax(1ll, dateDiff / shipmentsCount);
    const auto actionsDayShift = static_cast<int>(Qt::DayOfWeek::Sunday);

    auto shipmentHistory = QVariantList{};
    for (const auto &shipmentId : generateRandomIdList(shipmentsCount)) {
        shipmentHistory.clear();

        auto actionStartDate = startDate.addDays(qrand() % shipmentsDayShift);

        auto lastAction = static_cast<Enums::SupplyChainAction>(
                    static_cast<int>(sc_availableActions.first()) + qrand() % sc_availableActions.size());
        auto lastActionProgress = static_cast<Enums::ActionProgress>(
                    static_cast<int>(sc_availableActionProgresses.first()) + qrand() % sc_availableActionProgresses.size());

        auto userType = sc_actionsUser.value(sc_availableActions.first());
        auto company = randomCompany(userType);
        for (const auto &currentAction : sc_availableActions) {
            if (currentAction > lastAction) {
                break;
            }

            auto newUserType = sc_actionsUser.value(currentAction);
            if (newUserType != userType) {
                userType = newUserType;
                company = randomCompany(userType);
            }

            for (const auto &currentActionProgress : sc_availableActionProgresses) {
                if (actionStartDate > endDate ||
                        (currentAction == lastAction && currentActionProgress > lastActionProgress)) {
                    break;
                }

                auto agentInfo = QVariantMap({ { Tags::role, Utility::instance()->userTypeToString(userType) },
                                               { Tags::companyName, company.first } });

                auto eventData = QVariantMap({ { Tags::timestamp, actionStartDate.toSecsSinceEpoch() },
                                               { Tags::agent, agentInfo },
                                               { Tags::action, Utility::instance()->supplyChainActionToString(currentAction) },
                                               { Tags::actionProgress, Utility::instance()->actionProgressToString(currentActionProgress) },
                                               { Tags::location, QVariantList({ company.second.lat, company.second.lon }) } });
                shipmentHistory.append(eventData);

                auto daysElapsed = (qrand() % actionsDayShift);
                actionStartDate = actionStartDate.addDays(daysElapsed);
            }
        }
        m_shipmentsHistory.insert(shipmentId, shipmentHistory);
    }
}

QMap<QVariant, QVariantList> FakeDataPopulator::getShipmentsHistory() const
{
    return m_shipmentsHistory;
}

bool FakeDataPopulator::canAddAction(const QString &shipmentId, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress) const
{
    auto nextAction = getNextAction(shipmentId);
    if (nextAction.first > sc_availableActions.last()) {
        return false;   // all actions already used
    }
    return (nextAction.first == action) && (nextAction.second == actionProgress);
}

bool FakeDataPopulator::addAction(const QString &shipmentId, const Enums::SupplyChainAction &action, const Enums::ActionProgress &actionProgress)
{
    if (!canAddAction(shipmentId, action, actionProgress)) {
        return false;
    }

    auto userType = sc_actionsUser.value(action);
    auto company = randomCompany(userType);

    const auto &shipmentHistory = m_shipmentsHistory[shipmentId];
    if (!shipmentHistory.isEmpty()) {
        auto lastHistoryItem = shipmentHistory.last().toMap();
        auto lastAgent = lastHistoryItem.value(Tags::agent).toMap();
        auto newUserType = Utility::instance()->userTypeFromString(lastAgent.value(Tags::role).toString());
        if (newUserType == userType) {
            auto companyName = lastAgent.value(Tags::companyName).toString();
            company = { companyName, companyLocation(userType, companyName) };
        }
    }

    auto agentInfo = QVariantMap({ { Tags::role, Utility::instance()->userTypeToString(userType) },
                                   { Tags::companyName, company.first } });

    auto eventData = QVariantMap({ { Tags::timestamp, QDateTime::currentDateTime().toMSecsSinceEpoch() },
                                   { Tags::agent, agentInfo },
                                   { Tags::action, Utility::instance()->supplyChainActionToString(action) },
                                   { Tags::actionProgress, Utility::instance()->actionProgressToString(actionProgress) },
                                   { Tags::location, QVariantList({ company.second.lat, company.second.lon }) } });

    m_shipmentsHistory[shipmentId].append(eventData);
    return true;
}

QPair<QString, Location> FakeDataPopulator::randomCompany(const Enums::UserType &userType) const
{
    auto idx = qrand() & sc_companiesCount.value(userType) - 1;
    return { sc_companiesName.value(userType).at(idx), sc_companiesLocation.value(userType).at(idx) };
}

Location FakeDataPopulator::companyLocation(const Enums::UserType &userType, const QString &companyName) const
{
    auto idx = sc_companiesName.value(userType).indexOf(companyName);
    if (idx < 0 || sc_companiesCount.value(userType)) {
        return {};
    }
    return sc_companiesLocation.value(userType).at(idx);
}

QString FakeDataPopulator::generateRandomId() const
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i = 0; i < ID_LENGTH; ++i) {
        randomString.append(possibleCharacters.at(qrand() % possibleCharacters.length()));
        if (i == 3 || i == 7)
            randomString.append('-');
    }
    return randomString;
}

QStringList FakeDataPopulator::generateRandomIdList(const int count) const
{
    QStringList result;

    for (int i = 0 ; i < count ; i++) {
        QString newId;
        while (newId.isEmpty() || result.contains(newId)) {
            newId = generateRandomId();
        }
        result.push_back(newId);
    }

    return result;
}

QPair<Enums::SupplyChainAction, Enums::ActionProgress> FakeDataPopulator::getNextAction(const QString &shipmentId) const
{
    const auto &shipmentHistory = m_shipmentsHistory[shipmentId];
    if (shipmentHistory.isEmpty()) {
        return { sc_availableActions.first(), sc_availableActionProgresses.first() };
    }

    auto lastHistoryItem = shipmentHistory.last().toMap();
    auto lastAction = lastHistoryItem.value(Tags::action).toInt();
    auto lastActionProgress = lastHistoryItem.value(Tags::actionProgress).toInt();

    if (lastActionProgress != static_cast<int>(sc_availableActionProgresses.last()) ) {
        return { static_cast<Enums::SupplyChainAction>(lastAction), static_cast<Enums::ActionProgress>(lastActionProgress + 1) };
    }
    return { static_cast<Enums::SupplyChainAction>(lastAction + 1), static_cast<Enums::ActionProgress>(lastActionProgress) };
}
