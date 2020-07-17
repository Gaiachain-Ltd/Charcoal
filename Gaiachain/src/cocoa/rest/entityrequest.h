#ifndef ENTITYREQUEST_H
#define ENTITYREQUEST_H

#include "rest/baserequest.h"

#include <QGeoCoordinate>

#include "common/enums.h"

class EntityRequest : public BaseRequest
{
    Q_OBJECT
public:
    enum class RequestType {
        Invalid = -1,
        GetBatch,
        GetFilterRange,
        GetFilterLimitRange,
        GetFilterLimit,
        GetFilterLastAction,
        GetUnusedLots,
        PostNewAction,
        PostUnusedLot
    };
    Q_ENUM(RequestType)

    struct EntityData {
        Enums::SupplyChainAction action;
        QGeoCoordinate coordinate;
        QDateTime timestamp;
        QVariantMap properties;
    };

    EntityRequest(const RequestType &requestType, const QString &token = {});

    EntityRequest(const QStringList &packageIds = {});
    EntityRequest(const QDateTime &from, const QDateTime &to);
    EntityRequest(int limit, int offset, const QDateTime &from, const QDateTime &to);
    EntityRequest(int limit, int offset, const QString &keyword, const QSet<Enums::PackageType> &filteredPackages, int cooperativeId);
    EntityRequest(const QString &token, const Enums::SupplyChainAction &lastAction);

    EntityRequest(const QString &token, const QString &packageId, const EntityData &entityData);
    EntityRequest(const QString &token, const QString &packageId, const QByteArray &codeData, const EntityData &entityData);
    EntityRequest(const QString &token, const QByteArray &codeData, const EntityData &entityData);

    EntityRequest(const QString &token, const Enums::PackageType &packageType, bool create = false);
private:
    const RequestType m_requestType = RequestType::Invalid;

    static RequestType getDefaultRequestType(const Enums::PackageType &packageType,
                                             bool create);

    static const QString sc_basePath;
    static const QMap<RequestType, Type> sc_requestsType;
    static const QMap<RequestType, QString> sc_requestsPath;

    bool isTokenRequired() const override;

    static QJsonObject entityDataObject(const EntityData &entityData);
};


#endif // ENTITYREQUEST_H
