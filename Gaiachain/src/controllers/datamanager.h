#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include <QThread>

#include "datamodelsmanager.h"
#include "datarequestsmanager.h"

#include "../common/userdata.h"

class DataManager : public AbstractManager
{
    Q_OBJECT

public:
    DataManager(QObject *parent = nullptr);
    ~DataManager() override;

    void setupQmlContext(QQmlApplicationEngine &engine) override;
    void setupModels(QSqlDatabase db);

    void updateUserData(const UserData &userData);

    bool processing() const override;

    Q_INVOKABLE void getPackageData(const QString &packageId) const;

    Q_INVOKABLE void addHarvestAction(const QString &parcelCode, const QDateTime &timestamp, const QVariantMap &properties);
    Q_INVOKABLE void addAction(const QString &packageId, const Enums::SupplyChainAction &action,
                               const QDateTime &timestamp, const QVariantMap &properties);
    Q_INVOKABLE void addAction(const QString &packageId, const Enums::SupplyChainAction &action, const QByteArray &codeData,
                               const QDateTime &timestamp, const QVariantMap &properties);
    Q_INVOKABLE void addAction(const Enums::SupplyChainAction &action, const QByteArray &codeData,
                               const QDateTime &timestamp, const QVariantMap &properties);

    Q_INVOKABLE void sendOfflineActions();

    Q_INVOKABLE void fetchEventData(const QString &packageId, const Enums::PackageType &type);
    Q_INVOKABLE void fetchRangeEvents(const QDateTime &from, const QDateTime &to, const QString &keyword = {});
    Q_INVOKABLE void fetchLimitEvents(int limit, int offset, const QString &keyword = {});
    Q_INVOKABLE void fetchLastActionPackageEvents(const Enums::SupplyChainAction &lastAction);

signals:
    void collectingDataChanged(bool processing) const;

    void addActionRequest(const QString &packageId, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties);
    void addActionRequest(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties);
    void addActionRequest(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                          const QDateTime &timestamp, const QVariantMap &properties);

    void eventsInfoNeeded(int limit, int offset, const QString &keyword) const;
    void eventsInfoNeeded(const QDateTime &from, const QDateTime &to, const QString &keyword) const;
    void lastActionEventsInfoNeeded(const Enums::SupplyChainAction &lastAction) const;
    void eventsNeeded(const QStringList &ids) const;

    void packageData(const PackageData &packageData) const;

public slots:
    void onActionAdded(const QString &packageId, const Enums::SupplyChainAction &action);
    void onActionAddError(const QString &packageId, const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &error);

    void onAdditionalDataLoaded(const QJsonObject &additionalData);
    void onEntitiesInfoLoaded(const QJsonArray &entitiesInfo);
    void onEntitiesLoaded(const QJsonArray &entities);
    void onUnusedLotIdsLoaded(const QJsonArray &idsArray);

private:
    QThread m_processingThread;

    DataModelsManager m_modelsHandler;
    DataRequestsManager m_requestsHandler;

    UserData m_userData;

    void setupHandlersConnections();

    QString generateHarvestId(const QDate &date, const QString &parcelCode);
};

#endif // DATAMANAGER_H
