#ifndef FAKEDATAPOPULATOR_H
#define FAKEDATAPOPULATOR_H

#include <QObject>
#include <QVariantHash>
#include <QVector>
#include <QDateTime>
#include <tuple>

#include "../../../common/enums.h"

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

    QVariantMap getRelations(const QStringList &packagesIds) const;
    void addPackageRelation(const QString &packageId, const QStringList &relatedIds);

    QVariantList getEventsInfo(int count, const QDateTime &from, const QString &keyword) const;
    QVariantList getEventsInfo(const QDateTime &from, const QDateTime &to, const QString &keyword) const;
    QString getEventId(const QByteArray &codeData) const;
    QVariantList getEventHistory(const QStringList &packagesId) const;

    QVariantList createdHarvestIds(const QString &cooperativeId) const;
    QVariantList unusedLotIds(const QString &cooperativeId) const;
    QString createUnusedLotId(const QString &cooperativeId);

    bool canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, const Enums::UserType &user, const QString &cooperativeId) const;
    bool addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData,
                   const Enums::UserType &user, const QString &cooperativeId);

    bool canAddNewAction(const Enums::SupplyChainAction &action, const QByteArray &codeData, const Enums::UserType &user) const;
    QString addNewAction(const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties, const QByteArray &codeData,
                         const Enums::UserType &user, const QString &cooperativeId);

    QVariantList getProducers() const;
    QVariantList getBuyers() const;
    QVariantList getTransporters() const;
    QVariantList getDestinations() const;

private:
    static const QString sc_loginPassword;
    static const QHash<QString, Enums::UserType> sc_usersLogin;
    static const QHash<Enums::PlaceType, Enums::UserType> sc_placesUser;

    static const QList<QVariantHash> sc_producers;
    static const QList<QVariantHash> sc_cooperatives;
    static const QStringList sc_buyers;
    static const QStringList sc_transporters;
    static const QStringList sc_destinations;

    QMultiMap<QString, QVariantMap> m_eventsHistory;
    QMultiMap<QString, QString> m_packagesRelations;
    QMap<QByteArray, QString> m_packagesCodeData;
    QHash<QString, QStringList> m_cooperativeUnusedLotIds;

    static const auto sc_minDayShift = static_cast<int>(Qt::DayOfWeek::Tuesday);

    static QList<QString> sortedLogins();

    QTime randomTime();

    QVariantHash randomCooperative() const;
    QVariantHash randomProducer() const;
    QString randomParcel(const QVariantHash &producer) const;
    QString randomBuyer() const;
    QString randomTransporter() const;
    QString randomDestination() const;

    QVariantHash cooperativeById(const QString &cooperativeId) const;

    QString generateHarvestId(const QString &parceleCode, const QDate &date) const;
    QString generateSackId(const QString &cooperativeId, const QDate &date) const;
    QString generateLotId(const QString &cooperativeId, const QDate &date) const;

    std::tuple<QString, QVariantMap> generateHarvestAction(const QDate &harvestDate) const;
    QVariantMap generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const;
    QVariantMap generateReceptionProperties(const QDate &processingDate, const QDate &actionDate) const;
    QVariantMap generateBaggingProperties(const QStringList &harvestIds) const;
    QVariantMap generateLotCreationProperties() const;
    QVariantMap generateWarehouseTransportProperties() const;
    QVariantMap generateExportReceptionProperties() const;

    void addHarvestRelation(const QString &sacId, const QStringList &harvestIds);
    void addLotRelation(const QString &sacId, const QString &lotId);

    void addEvent(const QString &id, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                  const QVariantMap &properties, const QVariantHash &cooperative, const QByteArray &codeData = {});

    void generateCooperativeData(const QVariantHash &cooperative, const QDate &startDate, const QDate &endDate);
};

#endif // FAKEDATAPOPULATOR_H
