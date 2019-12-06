#include "entityrequest.h"

#include <QDebug>
#include <QJsonObject>
#include <QUrlQuery>

#include "../common/tags.h"
#include "../common/dataglobals.h"
#include "../helpers/requestshelper.h"

const QString EntityRequest::sc_basePath = QStringLiteral("/entities/%1");

const QMap<EntityRequest::RequestType, MRestRequest::Type> EntityRequest::sc_requestsType = {
    { EntityRequest::RequestType::GetBatch, Type::Post },   // it's post because of pids data
    { EntityRequest::RequestType::GetFilterLimit, Type::Get },
    { EntityRequest::RequestType::GetFilterTo, Type::Get },
    { EntityRequest::RequestType::GetFilterLastAction, Type::Get },
    { EntityRequest::RequestType::GetUnusedLots, Type::Get },
    { EntityRequest::RequestType::PostNewAction, Type::Post },
    { EntityRequest::RequestType::PostUnusedLot, Type::Post }
};

const QMap<EntityRequest::RequestType, QString> EntityRequest::sc_requestsPath = {
    { EntityRequest::RequestType::GetBatch, sc_basePath.arg(QStringLiteral("batch/")) },
    { EntityRequest::RequestType::GetFilterLimit, sc_basePath.arg(QStringLiteral()) },
    { EntityRequest::RequestType::GetFilterTo, sc_basePath.arg(QStringLiteral()) },
    { EntityRequest::RequestType::GetFilterLastAction, sc_basePath.arg(QStringLiteral("harvests/")) },
    { EntityRequest::RequestType::GetUnusedLots, sc_basePath.arg(QStringLiteral("lots/")) },
    { EntityRequest::RequestType::PostNewAction, sc_basePath.arg(QStringLiteral("new/")) },
    { EntityRequest::RequestType::PostUnusedLot, sc_basePath.arg(QStringLiteral("lots/")) }
};

EntityRequest::EntityRequest(const EntityRequest::RequestType &requestType, const QString &token)
    : BaseRequest(sc_requestsPath.value(requestType), sc_requestsType.value(requestType), token), m_requestType(requestType)
{
    if (m_requestType == RequestType::Invalid) {
        qCWarning(sessionRequest) << "Invalid request provided";
    }
}

EntityRequest::EntityRequest(const QStringList &packageIds)
    : EntityRequest(RequestType::GetBatch)
{
    auto docObj = QJsonObject{ { Tags::pids, (packageIds.isEmpty() ? QJsonValue(QString{})
                                                                   : QJsonValue::fromVariant(packageIds)) } };
    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(int limit, int offset, const QString &keyword)
    : EntityRequest(RequestType::GetFilterLimit)
{
    auto query = QUrlQuery{};
    query.addQueryItem(Tags::limit, QString::number(limit));
    query.addQueryItem(Tags::offset, QString::number(offset));
    if (!keyword.isEmpty()) {
        query.addQueryItem(Tags::keyword, keyword);
    }
    setQuery(query);
}

EntityRequest::EntityRequest(const QDateTime &from, const QDateTime &to, const QString &keyword)
    : EntityRequest(RequestType::GetFilterTo)
{
    auto query = QUrlQuery{};
    query.addQueryItem(Tags::timestampTo, QString::number(static_cast<qint64>(to.toSecsSinceEpoch())) );
    if (from.isValid()) {
        query.addQueryItem(Tags::timestampFrom, QString::number(static_cast<qint64>(from.toSecsSinceEpoch())) );
    }
    if (!keyword.isEmpty()) {
        query.addQueryItem(Tags::keyword, keyword);
    }
    setQuery(query);
}

EntityRequest::EntityRequest(const QString &token, const Enums::SupplyChainAction &lastAction)
    : EntityRequest(RequestType::GetFilterLastAction, token)
{
    auto query = QUrlQuery{};
    query.addQueryItem(Tags::lastAction, RequestsHelper::supplyChainActionToString(lastAction) );
    setQuery(query);
}

EntityRequest::EntityRequest(const QString &token, const QString &packageId, const EntityRequest::EntityData &entityData)
    : EntityRequest(RequestType::PostNewAction, token)
{
    auto docObj = entityDataObject(entityData);
    docObj.insert(Tags::pid, packageId);

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const QString &packageId, const QByteArray &codeData, const EntityRequest::EntityData &entityData)
    : EntityRequest(RequestType::PostNewAction, token)
{
    auto docObj = entityDataObject(entityData);
    docObj.insert(Tags::pid, packageId);
    docObj.insert(Tags::qrCode, codeData.data());

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const QByteArray &codeData, const EntityRequest::EntityData &entityData)
    : EntityRequest(RequestType::PostNewAction, token)
{
    auto docObj = entityDataObject(entityData);
    docObj.insert(Tags::qrCode, codeData.data());

    mRequestDocument.setObject(docObj);
}

EntityRequest::EntityRequest(const QString &token, const Enums::PackageType &packageType, bool create)
    : EntityRequest(packageType == Enums::PackageType::Lot ? (create ? RequestType::PostUnusedLot
                                                                     : RequestType::GetUnusedLots)
                                                           : RequestType::Invalid, token)
{}

bool EntityRequest::isTokenRequired() const
{
    return (BaseRequest::isTokenRequired() &&
            !(m_requestType == RequestType::GetBatch)) ||
            (m_requestType == RequestType::GetFilterLastAction) ||
            (m_requestType == RequestType::GetUnusedLots);
}

QJsonObject EntityRequest::entityDataObject(const EntityRequest::EntityData &entityData)
{
    return {
        { Tags::action,     RequestsHelper::supplyChainActionToString(entityData.action) },
        { Tags::timestamp,  static_cast<qint64>(entityData.timestamp.toSecsSinceEpoch()) },
        { Tags::properties, QJsonObject::fromVariantMap(
                        RequestsHelper::convertProperties(entityData.properties)) }
    };
}
