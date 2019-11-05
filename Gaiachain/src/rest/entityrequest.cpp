#include "entityrequest.h"

#include <QDebug>
#include <QJsonObject>

#include "../common/logs.h"
#include "../common/tags.h"
#include "../common/dataglobals.h"
#include "../helpers/requestshelper.h"

const QString EntityRequest::sc_basePath = QStringLiteral("/entities/%1/");

const QMap<EntityRequest::RequestType, MRestRequest::Type> EntityRequest::sc_requestsType = {
    { EntityRequest::RequestType::GetBatch, Type::Get },
    { EntityRequest::RequestType::GetFilterCount, Type::Get },
    { EntityRequest::RequestType::GetFilterTo, Type::Get },
    { EntityRequest::RequestType::GetId, Type::Get },
    { EntityRequest::RequestType::GetUnused, Type::Get },
    { EntityRequest::RequestType::PutActionId, Type::Put },
    { EntityRequest::RequestType::PutActionCode, Type::Put },
    { EntityRequest::RequestType::PostNewPackage, Type::Post },
    { EntityRequest::RequestType::PostUnused, Type::Post }
};

const QMap<EntityRequest::RequestType, QString> EntityRequest::sc_requestsPath = {
    { EntityRequest::RequestType::GetBatch, sc_basePath.arg(QStringLiteral("batch")) },
    { EntityRequest::RequestType::GetFilterCount, sc_basePath.arg(QStringLiteral("filter")) },
    { EntityRequest::RequestType::GetFilterTo, sc_basePath.arg(QStringLiteral("filter")) },
    { EntityRequest::RequestType::GetId, sc_basePath.arg(QStringLiteral("id")) },
    { EntityRequest::RequestType::GetUnused, sc_basePath.arg(QStringLiteral("unused")) },
    { EntityRequest::RequestType::PutActionId, sc_basePath.arg(QStringLiteral("%1")) },
    { EntityRequest::RequestType::PutActionCode, sc_basePath.arg(QStringLiteral("code/%1")) },
    { EntityRequest::RequestType::PostNewPackage, sc_basePath.arg(QStringLiteral("new/%1")) },
    { EntityRequest::RequestType::PostUnused, sc_basePath.arg(QStringLiteral("unused")) }
};

EntityRequest::EntityRequest(const EntityRequest::RequestType &requestType, const QString &token)
    : EntityRequest(sc_requestsPath.value(requestType), requestType, token)
{}

EntityRequest::EntityRequest(const QString &customPath, const EntityRequest::RequestType &requestType, const QString &token)
    : BaseRequest(customPath, sc_requestsType.value(requestType), token),
      m_requestType(requestType)
{
    if (m_requestType == RequestType::Invalid) {
        qCWarning(sessionRequest) << "Invalid request provided";
    }
}

EntityRequest::EntityRequest(const QStringList &ids)
    : EntityRequest(RequestType::GetBatch)
{
    auto docObj = QJsonObject{ { Tags::ids, ids.isEmpty() ? QJsonValue(StaticValues::all)
                                                          : QJsonValue::fromVariant(ids) } };

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(int count, const QDateTime &from)
    : EntityRequest(RequestType::GetFilterCount)
{
    auto docObj = QJsonObject{ { Tags::count, count } };
    if (from.isValid()) {
        docObj.insert(Tags::timestampFrom, static_cast<qint64>(from.toTime_t()) );
    }

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QDateTime &to, const QDateTime &from)
    : EntityRequest(RequestType::GetFilterTo)
{
    auto docObj = QJsonObject{ { Tags::timestampTo, static_cast<qint64>(to.toTime_t()) } };
    if (from.isValid()) {
        docObj.insert(Tags::timestampFrom, static_cast<qint64>(from.toTime_t()) );
    }

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QByteArray &codeData)
    : EntityRequest(RequestType::GetId)
{
    auto docObj = QJsonObject{ { Tags::code, codeData.data() } };
    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const Enums::PackageType &packageType)
    : EntityRequest(RequestType::GetUnused)
{
    auto docObj = QJsonObject{ { Tags::packageType, RequestsHelper::packageTypeToString(packageType) } };
    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const QString &packageId, const EntityRequest::EntityData &entityData, const QByteArray &codeData)
    : EntityRequest(sc_requestsPath.value(RequestType::PutActionId).arg(packageId), RequestType::PutActionId, token)
{
    auto docObj = QJsonObject{
        { Tags::packageType,    RequestsHelper::packageTypeToString(DataGlobals::packageType(entityData.action)) },
        { Tags::action,         RequestsHelper::supplyChainActionToString(entityData.action) },
        { Tags::timestamp,      static_cast<qint64>(entityData.timestamp.toTime_t()) },
        { Tags::properties,     QJsonObject::fromVariantMap(entityData.properties) }
    };
    if (!codeData.isEmpty()) {
        docObj.insert(Tags::code, codeData.data());
    }
    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const QByteArray &codeData, const EntityRequest::EntityData &entityData, bool newPackage)
    : EntityRequest(sc_requestsPath.value(newPackage ? RequestType::PostNewPackage : RequestType::PutActionCode).arg(codeData.data()),
                    newPackage ? RequestType::PostNewPackage : RequestType::PutActionCode, token)
{
    auto docObj = QJsonObject{
        { Tags::packageType,    RequestsHelper::packageTypeToString(DataGlobals::packageType(entityData.action)) },
        { Tags::action,         RequestsHelper::supplyChainActionToString(entityData.action) },
        { Tags::timestamp,      static_cast<qint64>(entityData.timestamp.toTime_t()) },
        { Tags::properties,     QJsonObject::fromVariantMap(entityData.properties) }
    };
    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const Enums::PackageType &packageType)
    : EntityRequest(RequestType::PostUnused, token)
{
    auto docObj = QJsonObject{ { Tags::packageType, RequestsHelper::packageTypeToString(packageType) } };
    mRequestDocument.setObject(docObj);
}
