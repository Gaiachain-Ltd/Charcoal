#ifndef FAKEDATAPOPULATOR_H
#define FAKEDATAPOPULATOR_H

#include <QObject>
#include <QVariantHash>
#include <QVector>
#include <QDateTime>
#include <tuple>

#include "../../../common/enums.h"
#include "../../../common/location.h"

class EventModel;

class FakeDataPopulator : public QObject
{
    Q_OBJECT
public:
    explicit FakeDataPopulator();

    static QStringList availableLogins();

    bool checkLogin(const QString &email, const QString &password);
    Enums::UserType userType(const QString &email);
    QVariantMap generateUserData(const QString &email);

    void populateFakeData(const QDate &startDate);

    QVariantList getEventsHistory() const;
    QVariantList getEventHistory(const QString &packageId) const;
    bool canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, const Enums::UserType &user) const;
    bool addAction(const QString &packageId, const Enums::SupplyChainAction &action);

private:
    static const QString sc_loginPassword;
    static const QHash<QString, Enums::UserType> sc_usersLogin;
    static const QHash<Enums::PlaceType, Enums::UserType> sc_placesUser;

    static const QList<QVariantHash> sc_producers;
    static const QList<QVariantHash> sc_cooperatives;
    static const QList<QVariantHash> sc_buyers;
    static const QList<QVariantHash> sc_transporter;

    QMultiMap<QString, QVariantMap> m_eventsHistory;
    QMultiMap<QString, QString> m_packagesRelations;

    static const auto sc_minDayShift = static_cast<int>(Qt::DayOfWeek::Tuesday);

    static QList<QString> sortedLogins();

    QTime randomTime();

    QVariantHash randomCooperative() const;
    QVariantHash randomProducer() const;
    QString randomParcel(const QVariantHash &producer) const;
    QVariantHash randomBuyer() const;

    QString generateHarvestId(const QString &parceleCode, const QDate &date) const;
    QString generateSackId(const QString &cooperativeId, const QDate &date) const;
    QString generateLotId(const QString &cooperativeId, const QDate &date) const;

    std::tuple<QString, QVariantHash> generateHarvestAction(const QDate &harvestDate) const;
    QVariantHash generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const;
    QVariantHash generateReceptionProperties(const QDate &processingDate, const QDate &actionDate) const;
    QVariantHash generateBaggingProperties(const QStringList &harvestIds) const;
    QVariantHash generateLotCreationProperties() const;
    QVariantHash generateWarehouseTransportProperties() const;
    QVariantHash generateExportReceptionProperties() const;

    void addHarvestRelation(const QString &sacId, const QStringList &harvestIds);
    void addLotRelation(const QString &sacId, const QString &lotId);

    void addEvent(const QString &id, const QDate &actionDate, const Enums::SupplyChainAction &action,
                  const QVariantHash &cooperative, const QVariantHash &properties);

    void generateCooperativeData(const QVariantHash &cooperative, const QDate &startDate, const QDate &endDate);
};

#endif // FAKEDATAPOPULATOR_H
