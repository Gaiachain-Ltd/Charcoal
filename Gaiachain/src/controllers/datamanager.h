#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include <QSqlDatabase>
#include <QNetworkReply>
#include <QStringListModel>

#include "../models/existsquerymodel.h"
#include "../models/sqltablemodel.h"

#include "../models/producermodel.h"
#include "../models/namemodel.h"

#include "../models/eventmodel.h"
#include "../models/relationmodel.h"
#include "../models/unusedidsmodel.h"

#include "../models/selectedidsproxymodel.h"
#include "../models/daterangeproxymodel.h"
#include "../models/latestrangeeventsproxymodel.h"
#include "../models/latesteventsproxymodel.h"
#include "../models/packagetypeeventsproxymodel.h"
#include "../models/searcheventsproxymodel.h"
#include "../models/cooperativeeventsproxymodel.h"
#include "../models/packagetypeproxymodel.h"
#include "../models/packagetypeidsproxymodel.h"
#include "../models/createdharvestidsproxymodel.h"
#include "../models/localonlyproxymodel.h"

#include "../common/userdata.h"
#include "../common/packagedata.h"
#include "../models/packagedataproxymodel.h"
#include "../models/relationslistproxymodel.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool collectingData READ collectingData NOTIFY collectingDataChanged)

public:
    DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void updateUserData(const UserData &userData);

    bool collectingData() const;

    void setupModels(QSqlDatabase db);

    Q_INVOKABLE PackageData getPackageData(const QString &packageId) const;

    Q_INVOKABLE void addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                               const QVariantMap &properties, const QByteArray &codeData = {});
    Q_INVOKABLE void addAction(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties);
    Q_INVOKABLE void addAction(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                               const QVariantMap &properties, const QByteArray &codeData = {});

    Q_INVOKABLE void sendOfflineActions();

    Q_INVOKABLE void fetchEventData(const QString &packageId, const Enums::PackageType &type);
    Q_INVOKABLE void fetchRangeEvents(const QDateTime &from, const QDateTime &to, const QString &keyword = {});
    Q_INVOKABLE void fetchCountEvents(int count, const QDateTime &from, const QString &keyword = {});
    Q_INVOKABLE void fetchCreatedHarvestIdEvents();

signals:
    void collectingDataChanged(bool collectingData) const;

    void addActionRequest(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                          const QVariantMap &properties, const QByteArray &codeData = {});
    void addActionRequest(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties);
    void addActionRequest(const Enums::SupplyChainAction &action, const QDateTime &timestamp,
                          const QVariantMap &properties, const QByteArray &codeData);

    void eventsInfoNeeded(const QDateTime &from, const QDateTime &to, const QString &keyword) const;
    void eventsInfoNeeded(int count, const QDateTime &from, const QString &keyword) const;
    void eventsNeeded(const QStringList &ids) const;
    void relationsNeeded(const QStringList &ids) const;

    void createdHarvestIdEventsNeeded() const;

public slots:
    void onActionAdded(const QString &packageId, const Enums::SupplyChainAction &action);
    void onActionAddError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error);

    void onDataRequestError();
    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onEntitiesInfoLoaded(const QJsonArray &entitiesInfo);
    void onEntitiesLoaded(const QJsonArray &entities);
    void onRelationsLoaded(const QJsonArray &relations);
    void onCreatedHarvestIdsLoaded(const QJsonArray &idsArray);
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);

private:
    QSqlDatabase m_db;
    UserData m_userData;

    int m_dataRequestsCount = 0;
    QMultiMap<QString, Enums::SupplyChainAction> m_addActionRequestSent;

    // database models
    QScopedPointer<SqlTableModel> m_eventsDatabaseModel;
    QScopedPointer<SqlTableModel> m_relationsDatabaseModel;
    QScopedPointer<SqlTableModel> m_unusedIdsDatabaseModel;

    QScopedPointer<SqlTableModel> m_producersDatabaseModel;
    QScopedPointer<SqlTableModel> m_buyersDatabaseModel;
    QScopedPointer<SqlTableModel> m_transportersDatabaseModel;
    QScopedPointer<SqlTableModel> m_destinationsDatabaseModel;

    ExistsQueryModel m_existsQueryModel;

    // source
    EventModel m_eventsSourceModel;
    RelationModel m_relationsSourceModel;

    UnusedIdsModel m_unusedLotIdsModel;

    ProducerModel m_producersModel;
    NameModel m_buyersModel;
    NameModel m_transportersModel;
    NameModel m_destinationsModel;

    // proxy models
    CooperativeEventsProxyModel m_cooperativeEventsModel;   // always active
    CreatedHarvestIdsProxyModel m_createdHarvestIdsModel;
    PackageTypeIdsProxyModel m_packageTypeCooperativeIdsModel;

    LocalOnlyProxyModel m_localOnlyEventsModel;

    // TODO: if still needed in final implementation compose as extensions instead of single models
    CooperativeEventsProxyModel m_cooperativeFilteringEventsModel;

    DateRangeProxyModel m_calendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxyModel m_latestDateEventsModel;

    SearchEventsProxyModel m_searchEventsModel;
    PackageTypeProxyModel m_packagesTypeSearchEventsModel;
    LatestRangeEventsProxyModel m_latestRangePackagesTypeSearchEventsModel;

    PackageDataProxyModel m_packageDataModel;
    RelationsListProxyModel m_relationsListModel;

    void updateCooperativeId();

    void dataRequestSent();
    void dataRequestProcessed();

    void fetchMissingEvents(const Gaia::ModelData &eventsInfo);

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);

    void handleActionAdd(const QString &packageId, const Enums::SupplyChainAction &action, const QDateTime &timestamp, const QVariantMap &properties);
    QString generateHarvestId(const QDate &date, const QString &parcelCode);

    Gaia::ModelEntry processProducer(const QJsonValue &value);
    Gaia::ModelEntry processNameData(const QJsonValue &value);
    Gaia::ModelEntry processEventInfo(const QJsonValue &value);
    Gaia::ModelEntry processEvent(const QJsonValue &value);
    Gaia::ModelData processRelations(const QJsonValue &value);
    Gaia::ModelEntry processCreatedHarvestId(const QJsonValue &value);
    Gaia::ModelEntry processUnusedLotId(const QJsonValue &value);

    void removeExistingProducers(Gaia::ModelData &modelData);
    void removeExistingNameData(Gaia::ModelData &modelData, AbstractModel* model, const QLatin1String &tableName);
    void removeExistingEvents(Gaia::ModelData &modelData);
    void removeExistingRelations(Gaia::ModelData &modelData, bool fullCheck);
    void removeExistingUnusedLotIds(Gaia::ModelData &modelData);
};

#endif // DATAMANAGER_H
