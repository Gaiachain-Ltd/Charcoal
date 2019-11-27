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
        GetFilterCount,
        GetFilterTo,
        GetId,
        GetCreatedHarvests,
        GetUnusedLots,
        PutActionId,
        PutActionCode,
        PostNewPackage,
        PostUnusedLot
    };
    Q_ENUM(RequestType)

    struct EntityData {
        Enums::SupplyChainAction action;
        QDateTime timestamp;
        QVariantMap properties;
    };

    EntityRequest(const RequestType &requestType, const QString &token = {});
    EntityRequest(const QString &customPath, const RequestType &requestType, const QString &token = {});

    EntityRequest(const QStringList &packageIds = {});
    EntityRequest(int count, const QDateTime &from, const QString &keyword = {});
    EntityRequest(const QDateTime &from, const QDateTime &to, const QString &keyword = {});
    EntityRequest(const QByteArray &codeData);

    EntityRequest(const QString &token, const QString &packageId, const EntityData &entityData, const QByteArray &codeData = {});
    EntityRequest(const QString &token, const QByteArray &codeData, const EntityData &entityData, bool newPackage);

    EntityRequest(const QString &token, const Enums::PackageType &packageType, bool create = false);
private:
    const RequestType m_requestType = RequestType::Invalid;

    static const QString sc_basePath;
    static const QMap<RequestType, Type> sc_requestsType;
    static const QMap<RequestType, QString> sc_requestsPath;

    static RequestType requestForPackageType(const Enums::PackageType &type, const Type &requestType);
};


#endif // ENTITYREQUEST_H
