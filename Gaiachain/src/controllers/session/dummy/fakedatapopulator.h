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

    QVariantList getEventsInfo(int limit, const QDateTime &from, const QString &keyword) const;
    QVariantList getEventsInfo(const QDateTime &from, const QDateTime &to, const QString &keyword) const;
    QVariantList getLastActionEventsInfo(int cooperativeId, const Enums::SupplyChainAction &lastAction) const;
    QVariantList getEventsHistory(const QStringList &packagesId) const;

    QVariantList unusedLotIds(int cooperativeId) const;
    QString createUnusedLotId(int cooperativeId);

    bool addAction(const QString &packageId, const Enums::SupplyChainAction &action,
                   const QDateTime &timestamp, const QVariantMap &properties,
                   const Enums::UserType &user, int cooperativeId);
    bool addAction(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                   const QDateTime &timestamp, const QVariantMap &properties,
                   const Enums::UserType &user, int cooperativeId);
    QString addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                      const QDateTime &timestamp, const QVariantMap &properties,
                      const Enums::UserType &user, int cooperativeId);

    bool canAddAction(const QString &packageId, const Enums::SupplyChainAction &action, int cooperativeId);
    bool canAddAction(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action);
    bool canAddAction(const QByteArray &codeData, const Enums::SupplyChainAction &action, int cooperativeId);

    QVariantList getProducers() const;
    QVariantList getCompanies() const;
    QVariantList getDestinations() const;

private:
    static const QList<QVariantHash> sc_producers;
    static const QList<QVariantHash> sc_companies;
    static const QList<QVariantHash> sc_destinations;

    QMultiMap<QString, QVariantMap> m_eventsHistory;
    QMultiMap<QString, QString> m_packagesRelations;
    QMap<QByteArray, QString> m_packagesCodeData;
    QHash<int, QStringList> m_cooperativeUnusedLotIds;

    static const auto sc_minDayShift = static_cast<int>(Qt::DayOfWeek::Tuesday);

    static QList<QString> sortedLogins();

    QTime randomTime();

    QVariantHash randomCooperative() const;
    QVariantHash randomProducer() const;
    QVariantHash randomParcel(const QVariantHash &producer) const;
    QVariantHash randomBuyer() const;
    QVariantHash randomTransporter() const;
    QVariantHash randomDestination() const;

    QVariantHash companyById(int companyId) const;

    QString getEventId(const QByteArray &codeData) const;
    bool isNextPackageAction(const QString &packageId, const Enums::SupplyChainAction &action, int cooperativeId) const;

    QString generateHarvestId(const QString &parceleCode, const QDate &date) const;
    QString generateSackId(const QString &cooperativeCode, const QDate &date) const;
    QString generateLotId(const QString &cooperativeCode, const QDate &date) const;

    std::tuple<QString, QVariantMap> generateHarvestAction(const QDate &harvestDate) const;
    QVariantMap generateGrainProcessingProperties(const QDate &harvestDate, const QDate &actionDate) const;
    QVariantMap generateReceptionProperties(const QDate &processingDate, const QDate &actionDate) const;
    QVariantMap generateBaggingProperties(const QStringList &harvestIds) const;
    QVariantMap generateLotCreationProperties() const;
    QVariantMap generateWarehouseTransportProperties(const QDate &transportDate) const;
    QVariantMap generateExportReceptionProperties() const;

    void addHarvestRelation(const QString &sacId, const QStringList &harvestIds);
    void addLotRelation(const QString &sacId, const QString &lotId);

    void addEvent(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                  const QVariantMap &properties, const QVariantHash &cooperative, const QByteArray &codeData = {});

    void generateCooperativeData(const QVariantHash &cooperative, const QDate &startDate, const QDate &endDate);
};

#endif // FAKEDATAPOPULATOR_H
