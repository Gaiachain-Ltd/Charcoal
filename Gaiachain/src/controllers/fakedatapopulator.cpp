#include "fakedatapopulator.h"

#include <QDateTime>

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"
#include "../common/tags.h"
#include "../common/location.h"
#include "../common/logs.h"

#define ID_LENGTH 12

FakeDataPopulator::FakeDataPopulator(EventModel &eventModel, ShipmentModel &shipmentModel)
    : m_eventModel(eventModel)
    , m_shipmentModel(shipmentModel)
{

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

void FakeDataPopulator::populateFakeData(const int count)
{
    QVector<Enums::UserType> userType = {
        Enums::UserType::Producer,
        Enums::UserType::BaggingPerson,
        Enums::UserType::Storekeeper,
        Enums::UserType::Exporter
    };

    const QVector<Enums::PlaceAction> actions {
        Enums::PlaceAction::Arrived,
        Enums::PlaceAction::Departed
    };

    const QDateTime startDate(QDate(2019,1,1));
    const QDateTime endDate = QDateTime::currentDateTime();
    const auto dateDiff = startDate.daysTo(endDate) / 4;

    // Timber
    int i = 0;
    for (const QString &shipmentId : generateRandomIdList(count)) {
        ++i;
        Gaia::ModelData eventData;

        QDateTime sd = startDate;

        int stepCount = 0;
        const int maxStepCount = qrand() % (userType.size() * actions.size());
        for (const auto &user : userType) {
            const QStringList &companyNameList = m_companyNames[user];
            const QString &companyName = companyNameList[rand() % companyNameList.size()];

            for (const auto &action : actions) {
                int daysElapsed = (rand() % dateDiff + 1);
                sd = sd.addDays(daysElapsed);

                eventData.append({shipmentId,
                                  sd,
                                  QVariant::fromValue(Location { 9.665645 + i, 7.324432 + i }),
                                  companyName,
                                  QVariant::fromValue(user),
                                  QVariant::fromValue(action)
                                 });
                ++stepCount;
                if (stepCount > maxStepCount)
                    break;
            }
            if (stepCount > maxStepCount)
                break;
        }

        m_eventModel.appendData(eventData);

        Gaia::ModelData shipmentData;
        shipmentData.append({shipmentId, });
        m_shipmentModel.appendData(shipmentData);
    }
}

void FakeDataPopulator::addId(const QVariantMap &attributes)
{
    const QString id = attributes.value(Tags::shipmentId).toString();
    const Enums::PlaceAction newAction = static_cast<Enums::PlaceAction>(attributes.value(Tags::action).toInt());
    const Enums::PlaceType newPlace = static_cast<Enums::PlaceType>(attributes.value(Tags::place).toInt());
    const QString companyName = attributes.value(Tags::company).toString();

    bool exists = false;
    // Last action is last action inserted for this shipmentId.
    for (int i = 0 ; i < m_shipmentModel.rowCount() && !exists ; i++) {
        const QString &s = m_shipmentModel.data(m_shipmentModel.index(i), ShipmentModel::ShipmentId).toString();
        if (s == id) {
            exists = true;
        }
    }

    if (!exists) {
        Gaia::ModelData shipmentData;
        shipmentData.append({id, });
        m_shipmentModel.appendData(shipmentData);
    }

    Gaia::ModelData eventData;
    eventData.append({id,
                      QDateTime::currentDateTime(),
                      QVariant::fromValue(Location { 9.665645, 7.324432 }),
                      companyName,
                      QVariant::fromValue(newPlace),
                      QVariant::fromValue(newAction)
                     });

    m_eventModel.appendData(eventData);
}

Enums::PlaceType FakeDataPopulator::getNextPlaceType(const Enums::PlaceType place) const
{
    const int idx = m_cocoaPlaceList.indexOf(place);
    if (idx < 0) {
        return m_cocoaPlaceList.at(0);
    } else if (idx < m_cocoaPlaceList.size()) {
        return m_cocoaPlaceList.at(idx + 1);
    }

    return place;
}

bool FakeDataPopulator::canAddId(const QString &id) const
{
    // Last action is last action inserted for this shipmentId.
    for (int i = m_eventModel.rowCount() ; i >= 0 ; i--) {
        const QString &shipmentId = m_eventModel.data(m_eventModel.index(i), EventModel::ShipmentId).toString();
        if (id == shipmentId) {
            const Enums::PlaceAction lastAction = static_cast<Enums::PlaceAction>(m_eventModel.data(m_eventModel.index(i), EventModel::PlaceAction).toInt());
            const Enums::PlaceType lastPlace = static_cast<Enums::PlaceType>(m_eventModel.data(m_eventModel.index(i), EventModel::Place).toInt());

            // Nowhere to go, return error value.
            if (lastAction == Enums::PlaceAction::Departed && lastPlace == Enums::PlaceType::Export)
                return false;

            break;
        }
    }
    return true;
}

QVariantMap FakeDataPopulator::getIdNextData(const QString &id) const
{
    QVariantMap result;

    Enums::PlaceAction newAction = Enums::PlaceAction::Arrived;
    Enums::PlaceType newPlace = Enums::PlaceType::InvalidPlace;
    QString companyName;

    // Last action is last action inserted for this shipmentId.
    for (int i = m_eventModel.rowCount() ; i >= 0 ; i--) {
        const QString &shipmentId = m_eventModel.data(m_eventModel.index(i), EventModel::ShipmentId).toString();
        if (id == shipmentId) {
            const Enums::PlaceAction lastAction = static_cast<Enums::PlaceAction>(m_eventModel.data(m_eventModel.index(i), EventModel::PlaceAction).toInt());
            const Enums::PlaceType lastPlace = static_cast<Enums::PlaceType>(m_eventModel.data(m_eventModel.index(i), EventModel::Place).toInt());

            // Calculate new action and place for shipment.
            newAction = lastAction == Enums::PlaceAction::Arrived ? Enums::PlaceAction::Departed : Enums::PlaceAction::Arrived;
            newPlace = newAction == Enums::PlaceAction::Departed ? lastPlace : getNextPlaceType(lastPlace);

            companyName = newAction == Enums::PlaceAction::Departed ? m_eventModel.data(m_eventModel.index(i), EventModel::Company).toString() : QString();
            break;
        }
    }

    // This statment checks if shipment was not found in database.
    if (newPlace == Enums::PlaceType::InvalidPlace) {
        newPlace = getNextPlaceType(newPlace);

        const QStringList &companyNameList = m_companyNames[static_cast<Enums::UserType>(newPlace)];
        companyName = companyNameList[rand() % companyNameList.size()];
    }

    result.insert(Tags::company, companyName);
    result.insert(Tags::action, static_cast<int>(newAction));
    result.insert(Tags::place, static_cast<int>(newPlace));

    return result;
}

