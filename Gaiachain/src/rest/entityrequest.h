#ifndef ENTITYREQUEST_H
#define ENTITYREQUEST_H

#include "baserequest.h"

#include "../common/enums.h"

class EntityRequest : public BaseRequest
{
    Q_OBJECT
public:
    enum class RequestType {
        Invalid = -1,
        GetBatch,
        GetFilterLimit,
        GetFilterTo,
        GetFilterLastAction,
        GetUnusedLots,
        PostNewAction,
        PostUnusedLot
    };
    Q_ENUM(RequestType)

    struct EntityData {
        Enums::SupplyChainAction action;
        QDateTime timestamp;
        QVariantMap properties;
    };

    EntityRequest(const RequestType &requestType, const QString &token = {});

    EntityRequest(const QStringList &packageIds = {});
    EntityRequest(int limit, int offset, const QString &keyword = {});
    EntityRequest(const QDateTime &from, const QDateTime &to, const QString &keyword = {});
    EntityRequest(const QString &token, const Enums::SupplyChainAction &lastAction);

    EntityRequest(const QString &token, const QString &packageId, const EntityData &entityData);
    EntityRequest(const QString &token, const QString &packageId, const QByteArray &codeData, const EntityData &entityData);
    EntityRequest(const QString &token, const QByteArray &codeData, const EntityData &entityData);

    EntityRequest(const QString &token, const Enums::PackageType &packageType, bool create = false);
private:
    const RequestType m_requestType = RequestType::Invalid;

    static const QString sc_basePath;
    static const QMap<RequestType, Type> sc_requestsType;
    static const QMap<RequestType, QString> sc_requestsPath;

    bool isTokenRequired() const override;

    static QJsonObject entityDataObject(const EntityData &entityData);
};


#endif // ENTITYREQUEST_H
