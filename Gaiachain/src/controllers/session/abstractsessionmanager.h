#ifndef ABSTRACTSESSIONMANAGER_H
#define ABSTRACTSESSIONMANAGER_H

#include "../abstractmanager.h"
#include "../../common/enums.h"

#include <QNetworkReply>
#include <QLoggingCategory>

class QGeoCoordinate;

Q_DECLARE_LOGGING_CATEGORY(sessionManager)

class AbstractSessionManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(Enums::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    explicit AbstractSessionManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    bool enabled() const;
    Enums::ConnectionState connectionState() const;

    Q_INVOKABLE virtual void ping() = 0;

    Q_INVOKABLE virtual void login(const QString &login, const QString &password) = 0;

    Q_INVOKABLE virtual void getAdditionalData() = 0;
    Q_INVOKABLE virtual void getProducers() = 0;
    Q_INVOKABLE virtual void getCompanies() = 0;
    Q_INVOKABLE virtual void getDestinations() = 0;

    Q_INVOKABLE virtual void getEntitiesInfo(const QDateTime &from, const QDateTime &to) = 0;
    Q_INVOKABLE virtual void getEntitiesInfo(int limit, int offset, const QDateTime &from, const QDateTime &to) = 0;
    Q_INVOKABLE virtual void getEntitiesInfo(int limit, int offset, const QString &keyword,
                                             const QSet<Enums::PackageType> &filteredPackages, int cooperativeId) = 0;
    Q_INVOKABLE virtual void getLastActionEntitiesInfo(const Enums::SupplyChainAction &lastAction) = 0;

    Q_INVOKABLE virtual void getEntities(const QStringList &ids) = 0;

    Q_INVOKABLE virtual void postNewEntity(const QString &packageId, const Enums::SupplyChainAction &action,
                                           const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) = 0;
    Q_INVOKABLE virtual void postNewEntity(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                           const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) = 0;
    Q_INVOKABLE virtual void postNewEntity(const QByteArray &codeData, const Enums::SupplyChainAction &action,
                                           const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) = 0;

    Q_INVOKABLE virtual void getUnusedLotIds() = 0;
    Q_INVOKABLE virtual void postUnusedLotId() = 0;

public slots:
    void setEnabled(bool enabled);
    void updateToken(const QString &token);

signals:
    void enabledChanged(bool enabled) const;
    void connectionStateChanged(Enums::ConnectionState connectionState) const;

    void pingError(const QNetworkReply::NetworkError &code) const;
    void pingSuccess() const;

    void loginAttempt(const QString &login, const QString &password) const;
    void loginError(const QString &login, const int &code) const;
    void loginFinished(const QString &login, const QJsonObject &userDataObj) const;

    void additionalDataLoadError(const QNetworkReply::NetworkError &code) const;
    void additionalDataLoaded(const QJsonObject &data) const;

    void entitiesLoadError(const QNetworkReply::NetworkError &code) const;
    void entitiesInfoLoaded(const QJsonArray &entitiesInfo) const;
    void entitiesLoaded(const QJsonArray &entities) const;

    void entitySaveError(const QString &packageId, const QByteArray &codeData,
                         const Enums::SupplyChainAction &action, const QNetworkReply::NetworkError &code) const;
    void entitySaved(const QString &packageId, const QByteArray &codeData, const Enums::SupplyChainAction &action) const;

    void unusedLotIdsLoadError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdsLoaded(const QJsonArray &packageIds) const;
    void unusedLotIdCreateError(const QNetworkReply::NetworkError &code) const;
    void unusedLotIdCreated(const QString &packageId) const;

protected:
    bool m_enabled = true;
    Enums::ConnectionState m_connectionState = Enums::ConnectionState::Unknown;
    QString m_token;

    bool checkValidToken() const;

    void updateConnectionStateBeforeRequest();
    void updateConnectionStateAfterRequest(const QNetworkReply::NetworkError &errorCode = QNetworkReply::NoError);
};

#endif // ABSTRACTSESSIONMANAGER_H
